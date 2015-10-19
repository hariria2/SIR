#include <iostream>
#include <iomanip>
#include <sstream> 
#include <string>
//#include <list>
//#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <ctime>
#include "Domain.h"
#include "Place.h"
#include "Disease.h"
#include "InHostDynamics.h"
#include "Person.h"
#include "Architect.h"
#include "Storage.h"
#include "SQLStorage.h"
#include "Economy.h"
#include <sys/stat.h>
#include <sys/types.h>
 
using namespace std;

// Function prototyping
void GenerateHomes(vector<Place*> &homes, int perimeter[2][2], int homesize[2], Domain domain);
void getDefaultCoordinates(Place* location, double co[2]);
void readCityData(Domain *city, vector<Place*> &homes, vector<Place*> &works, vector<Place*> &schools, vector<Place*> &cemeteries);
void readPeopleData();
void generateSourceData();
void matchPeople(vector<Person*> &p1, vector<Person*> &p2, char s);
void updatePop(vector<Person*> p1, char s);

void Example1_SingleLocation(bool SaveData=true);
void Example2_MultiLocation(bool SaveData=true);

//=================



// ========================= Main ======================
int main(){
    
    Example1_SingleLocation();
    //Example2_MultiLocation();
    return 0;
 }
// ========================= End main =================


// ========================= Example Simulations ==============================
void Example1_SingleLocation(bool SaveData){
    int maxdim = 700;
    
    // Setting up parameters
    int cityBoundary[2][2]   = {{0, maxdim},{0, maxdim}};
    int homeBoundary[2][2]   = {{0, maxdim},{0, maxdim}};
    
    
    // Instantiate Classes
    Domain myCity("DiseasVille", cityBoundary);
    Place home(1, "home", "Home", homeBoundary,myCity);
    
    double hco[2];
    
    vector<Place*> homes;
    vector<Place*> works;
    vector<Place*> schools;
    vector<Place*> cemeteries;
    
    
    homes.push_back(&home);
    int population = 500;
    
    Disease flu("Flu", 24, 24, 1);
    char state = 'S';
    
    //============Economy==================//
    double A     = 2; //0.0025;
    double alpha = 0.5;
    double beta  = 0.5;
    double labor = 0;
    double health = 0;
    double y0 = 500; //A*pow(labor,alpha)*pow(health,beta);
    
    Economy econ(A,alpha,beta, y0, labor, health);
    //=======================================//
    
    
    //============InHostDynamics=============//
    normal_distribution<double> icDist(3,0.2);
    normal_distribution<double> betaDist(0.05,0);
    normal_distribution<double> deltaDist(3./50,0);
    normal_distribution<double> PDist(3,0);
    normal_distribution<double> CDist(0.8,0);
    double VirLev = 0.0;
    //=======================================//
    
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    
    
    
    vector<Person*> people;
    normal_distribution<double> ageDist(25,20);
    double randage;
    for (int i=0; i < population; i++){
        string name = "randomName"+to_string(i);
        
        
        
        randage  = ageDist(generator);
        int age = (randage < 0)? 0:floor(randage);
        
        getDefaultCoordinates(&home, hco);
        
        //=============InHostDynamics=================//
        if (i == 1){
            VirLev = 0.1;
        } else {
            VirLev = 0;
            state = 'S';
        }
        double randic  = icDist(generator);
        double ict = (randic < 0.5)? 0.5:randic;
        InHostDynamics ihd = InHostDynamics(i,0.05,ict,0.0,VirLev);
        double randbeta  = betaDist(generator);
        double beta = (randbeta < 0)? 0:randbeta;
        ihd.setBeta(beta);
        double randdelta  = deltaDist(generator);
        double delta = (randdelta < 0)? 0:randdelta;
        ihd.setDelta(delta);
        double randP  = PDist(generator);
        double P = (randP < 0)? 0:randP;
        ihd.setP(P);
        double randC  = CDist(generator);
        double C = (randC < 0)? 0:randC;
        ihd.setC(C);
        //====================================================//
        
        
        Person *p = new Person(i, name, age, 'S', flu, ihd, &myCity, &home, hco, 5,0,0, true);
        
        people.push_back(p);
    };
    (people.front())->setState('I');
    
    
    double InitialTime = 0;
    double EndTime = 20;
    double TimeStep = 0.05;
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for single location simulation: ";
        cin >> ver;
        string dataFolder = "data_single_v"+ver+"_";
        string movieFolder = "movie_single_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
        int xdim = maxdim+200;
        int ydim = maxdim;
        Visualization vis(xdim,ydim);
        vis.setPlaces(homes);
        vis.setPlaces(schools);
        vis.setPlaces(works);
        vis.setPlaces(cemeteries);
        vis.setPeople(people);
        Architect archie(InitialTime,EndTime,TimeStep, people, econ, "MYSQL", &sqldata, &vis);
        archie.Simulate();
    }else{
        Architect archie(InitialTime,EndTime,TimeStep, people, econ);
        archie.Simulate();
    }
}
void Example2_MultiLocation(bool SaveData){
    
    int maxdim = 1000;
    int cityBoundary[2][2]   = {{0, maxdim},{0, maxdim}};
    Domain myCity("DiseasVille", cityBoundary);
    
    vector<Place*> homes;
    vector<Place*> works;
    vector<Place*> schools;
    vector<Place*> cemeteries;
    
    readCityData(&myCity, homes, works, schools, cemeteries);
    
    
    
    
    double hco[2];
    double wco[2];
    double sco[2];
    double cco[2];
    
    int population = 700;
    
    Disease flu("Flu", 44, 40, 100);
    char state = 'S';
    double VirLev = 0.0;
    
    
    double A     = 2; //0.0025;
    double alpha = 0.5;
    double beta  = 0.5;
    
    double labor = 0;
    double health = 0;
    
    double y0 = 500; //A*pow(labor,alpha)*pow(health,beta);
    
    cout << "y0: " << y0 << endl;
    
    Economy econ(A,alpha,beta, y0, labor, health);
    
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    normal_distribution<double> ageDist(25,20);

    
    normal_distribution<double> icDist(3,0.2);
    normal_distribution<double> betaDist(0.05,0);
    normal_distribution<double> deltaDist(3./50,0);
    normal_distribution<double> PDist(3,0);
    normal_distribution<double> CDist(0.8,0);
    
    vector<Person*> people;
    for (int i=1; i <= population; i++){
        
        
        
        string name = "randomName"+to_string(i);
        
        
        
        int randHIdx = rand() % homes.size();
        int randWIdx = rand() % works.size();
        int randSIdx = rand() % schools.size();
        int randCIdx = rand() % cemeteries.size();
        
        getDefaultCoordinates(homes[randHIdx], hco);
        getDefaultCoordinates(works[randWIdx], wco);
        getDefaultCoordinates(schools[randSIdx], sco);
        getDefaultCoordinates(cemeteries[randCIdx], cco);
        
        double randage  = ageDist(generator);
        int age = (randage < 1)? 1:floor(randage);
        
        if (i == 1){
            VirLev = 0.1;
        } else {
            VirLev = 0;
            state = 'S';
        }
        
        
        double randic  = icDist(generator);
        double ict = (randic < 0.5)? 0.5:randic;
        
        
        InHostDynamics ihd = InHostDynamics(i,0.05,ict,0.0,VirLev);
        
    
        double randbeta  = betaDist(generator);
        double beta = (randbeta < 0)? 0:randbeta;
        
        
        ihd.setBeta(beta);
        

        double randdelta  = deltaDist(generator);
        double delta = (randdelta < 0)? 0:randdelta;
        
        ihd.setDelta(delta);
        
        
        double randP  = PDist(generator);
        double P = (randP < 0)? 0:randP;
        
        ihd.setP(P);
        
        
        double randC  = CDist(generator);
        double C = (randC < 0)? 0:randC;
        
        ihd.setC(C);
        
        
        Person *p = new Person(i, name, age, state, flu, ihd, &myCity, homes[randHIdx],
                               schools[randSIdx], works[randWIdx], cemeteries[randCIdx], homes[randHIdx],
                               hco,wco,sco,cco,10,10,10);
        p->setLocation(homes[randHIdx]);
        people.push_back(p);
    };
    
    
    double InitialTime = 0;
    double EndTime = 200;
    double TimeStep = 0.05; // TODO Fix the naming of the time files for fractional times.
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for multi location simulation: ";
        cin >> ver;
        string dataFolder = "data_multi_v"+ver+"_";
        string movieFolder = "movie_multi_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
        int xdim = maxdim+200;
        int ydim = maxdim;
        Visualization vis(xdim,ydim);
        vis.setPlaces(homes);
        vis.setPlaces(schools);
        vis.setPlaces(works);
        vis.setPlaces(cemeteries);
        vis.setPeople(people);
        Architect archie(InitialTime,EndTime,TimeStep, people, econ, "MYSQL", &sqldata, &vis);
        //Architect archie(InitialTime,EndTime,TimeStep, people, econ, "FileSystem", &data);
        
        archie.setDomain(myCity);
        archie.setHomes(homes);
        archie.setSchools(schools);
        archie.setWorks(works);
        archie.setCemetaries(cemeteries);
        archie.Simulate();
        
    }else{
        Architect archie(InitialTime,EndTime,TimeStep, people, econ);
        
        archie.Simulate();
    }
}

// ========================= End Examples ======================================

// ========================= Utilities ===========================================
void GenerateHomes(vector<Place*> &homes, int perimeter[2][2], int homesize[2], Domain domain){
 
	int xdist  = homesize[1];
	int ydist  = homesize[2];
 
	int xrange = perimeter[0][1] - perimeter[0][0];
	int yrange = perimeter[1][1] - perimeter[1][0];
 
	int xnum = floor((xrange/xdist)/2);
	int ynum = floor((yrange/ydist)/2);
 
	int total = xnum*ynum;
	int homedimx[total][2];
	int homedimy[total][2];
 
	int x[2] = {perimeter[0][0],perimeter[0][0]+xdist};
	int y[2] = {static_cast<int>(perimeter[1][0]+floor(ydist/2)),static_cast<int>(perimeter[1][0]+ydist+floor(ydist/2))};
	int n = 0;
 
	for (int i = 0; i < xnum; i++){
        for ( int j = 0; j < ynum; j++){
            homedimx[n][0] = x[0];
            homedimx[n][1] = x[1];
            homedimy[n][0] = y[0];
            homedimy[n][1] = y[1];
            for (int i = 0; i < 2; i++){
                y[i] = y[i] + 2*ydist;
            }
            n += 1;
        }
        y[0] = perimeter[1][0]+floor(ydist/2);
        y[1] = perimeter[1][0]+ydist+floor(ydist/2);
        for (int i = 0; i < 2; i++){
            x[i] = x[i] + 2*xdist;
        }
 
    }
 
	int perim[2][2];
	for (int i=0; i<total; i++){
        for (int n=0;n<2;n++){
            perim[0][n] = homedimx[i][n];
            perim[1][n] = homedimy[i][n];
        }
 Place* p = new Place(i, "home", "Home", perim, domain);
 homes.push_back(p);
	}
 }
void getDefaultCoordinates(Place* location, double co[2]){
    
    double x = rand() % (location->Perimeter[0][1] - location->Perimeter[0][0] + 1) + location->Perimeter[0][0];
    double y = rand() % (location->Perimeter[1][1] - location->Perimeter[1][0] + 1) + location->Perimeter[1][0];
    co[0] = x;
    co[1] = y;
}
void readCityData(Domain *city, vector<Place*> &homes, vector<Place*> &works, vector<Place*> &schools, vector<Place*> &cemeteries){
    string cityFileName = "../Source/CityPlan.csv";
    ifstream cityFile;
    cityFile.open(cityFileName, ios_base::in);
    
    if (cityFile.is_open()){
        cout << "File opened correctly. The contents are: " << endl;
     
        string sID;
        string name;
        string type;
        string sxmin;
        string sxmax;
        string symin;
        string symax;
      
        while (cityFile.good()){
            
            
            getline(cityFile, sID, ',');
            int ID = atoi(sID.c_str());
            
            getline(cityFile, name, ',');
            
            getline(cityFile, type, ',');
            
            getline(cityFile, sxmin, ',');
            int xmin = atoi(sxmin.c_str());
            
            getline(cityFile, sxmax, ',');
            int xmax = atoi(sxmax.c_str());
            
            getline(cityFile, symin, ',');
            int ymin = atoi(symin.c_str());
            
            getline(cityFile, symax, '\n');
            int ymax = atoi(symax.c_str());
            
            
            int boundary[2][2] = {{xmin, xmax},{ymin, ymax}};
            string test = "Home";
            
            if (type=="Home"){
                Place *h = new Place(ID, name, type, boundary, *city);
                homes.push_back(h);
            }else if (type=="Work"){
                Place *w = new Place(ID, name, type, boundary, *city);
                works.push_back(w);
            }else if (type=="School"){
                Place *s = new Place(ID, name, type, boundary, *city);
                schools.push_back(s);
            }else if (type=="Cemetery"){
                Place *c = new Place(ID, name, type, boundary, *city);
                cemeteries.push_back(c);
            }
            
        }
        cout << "Finished reading the file. Will close now. " << endl;
        cityFile.close();
    } else {
        cout << "File did not open correctly." << endl;
    }
};
void readPeopleData(){
    
}
void generateSourceData(){
    
}
void matchPeople(vector<Person*> &p1, vector<Person*> &p2, char s){
    
    for (auto ip = p1.cbegin(); ip != p1.cend(); ++ip){
        
        Person *p = new Person(
        (*ip) -> getID(),
        (*ip) -> getName(),
        (*ip) -> getAge(),
        (*ip) -> getState(),
        (*ip) -> getDisease(),
        (*ip) -> getInHostDynamics(),
        (*ip) -> getDomain(),
        (*ip) -> getHome(),
        (*ip) -> getHomeCoordinates(),
        (*ip) -> getInfVar(),
        (*ip) -> getIncVar(),
        (*ip) -> getRecVar(), true);
        
        p2.push_back(p);
    }
    string name = "randomName"+to_string(p1.size()+1);
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    
    normal_distribution<double> ageDist(25,20);
    double randage  = ageDist(generator);
    int age = (randage < 0)? 0:floor(randage);
    
    getDefaultCoordinates((p1.front())->getHome(), (p1.front())->getHomeCoordinates());
    Person *np = new Person(p1.size()+1,
                           name,
                           age,
                           s,
                           (p1.front()->getDisease()),
                           (p1.front()->getInHostDynamics()),
                           (p1.front()->getDomain()),
                           (p1.front())->getHome(),
                           (p1.front())->getHomeCoordinates(),
                           (p1.front())-> getInfVar(),
                           (p1.front())-> getIncVar(),
                           (p1.front())-> getRecVar(), true);
    p2.push_back(np);
    
}
void updatePop(vector<Person*> p1, char s){
    for (auto ip = p1.cbegin(); ip != p1.cend(); ++ip){
        if ((*ip)->getState() != s){
            (*ip)->setState(s);
            break;
        }
    }
}
// ========================= End utilities ======================================