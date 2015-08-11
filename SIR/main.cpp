#include <iostream>
#include <iomanip>
#include <sstream> 
#include <string>
#include <list>
#include <vector>
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
void Example4_MultiLocation(bool SaveData=true);

void Ex1_Eig_SingleLocation(bool SaveData=true);
void Ex1_SparseEig_SingleLocation(bool SaveData=true);
//=================

class Rectangle {
    int width, height;
public:
    void set_values (int,int);
    int area() {return width*height;}
    int getHeight(){return height;}
};

void Rectangle::set_values (int x, int y) {
    width = x;
    height = y;
}


// ========================= Main ======================
int main(){
    
    //Example1_SingleLocation();
    Example2_MultiLocation();
    //Example4_MultiLocation();
 	//Ex1_Eig_SingleLocation(true);
    //Ex2_SparseEig_SingleLocation(true);
    return 0;
 }
// ========================= End main =================


// ========================= Example Simulations ==============================
void Example1_SingleLocation(bool SaveData){
    int maxdim = 200;
    
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
    int population = 200;
    
    Disease flu("Flu", 24, 24, 1);
    InHostDynamics ihd(1, 0.1, 0, 0, 0);
    Economy econ(1,0.5,0.5);
    
    vector<Person*> people;
    for (int i=0; i < population; i++){
        string name = "randomName"+to_string(i);
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 0)? 0:floor(randage);
        
        getDefaultCoordinates(&home, hco);
        
        
        Person *p = new Person(i, name, age, 'S', flu, ihd, &myCity, &home, hco, 5,0,0, true);
        
        people.push_back(p);
    };
    (people.front())->setState('I');
    
    
    double InitialTime = 0;
    double EndTime = 20;
    double TimeStep = 1;
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for single location simulation: ";
        cin >> ver;
        string dataFolder = "data_single_v"+ver+"_";
        string movieFolder = "movie_single_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        Architect archie(InitialTime,EndTime,TimeStep, people, econ, "FileSystem", &data);
        archie.Simulate();
    }else{
        Architect archie(InitialTime,EndTime,TimeStep, people, econ);
        archie.Simulate();
    }
}
void Example2_MultiLocation(bool SaveData){
    
    int maxdim = 700;
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
    
    Disease flu("Flu", 24, 30, 70);
    char state = 'S';
    double VirLev = 0.1;
    
    Economy econ(1,0.5,0.5);
    
    
    vector<Person*> people;
    for (int i=1; i <= population; i++){
        
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        
        string name = "randomName"+to_string(i);
        
        
        
        int randHIdx = rand() % homes.size();
        int randWIdx = rand() % works.size();
        int randSIdx = rand() % schools.size();
        int randCIdx = rand() % cemeteries.size();
        
        getDefaultCoordinates(homes[randHIdx], hco);
        getDefaultCoordinates(works[randWIdx], wco);
        getDefaultCoordinates(schools[randSIdx], sco);
        getDefaultCoordinates(cemeteries[randCIdx], cco);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 1)? 1:floor(randage);
        
        if (i == 1){
            VirLev = 0.1;
        } else {
            VirLev = 0;
            state = 'S';
        }
        
        normal_distribution<double> icDist(4,1);
        double randic  = icDist(generator);
        
        double ict = (randic < 0.5)? 0.5:randic;
        
        
        InHostDynamics ihd = InHostDynamics(i,0.05,ict,0,VirLev);
        
        normal_distribution<double> betaDist(1,0);
        double randbeta  = betaDist(generator);
        double beta = (randbeta < 0)? 0:randbeta;
        
        ihd.setBeta(beta);
        
        normal_distribution<double> deltaDist(0.1,0);
        double randdelta  = deltaDist(generator);
        double delta = (randdelta < 0)? 0:randdelta;
        
        ihd.setDelta(delta);
        
        normal_distribution<double> PDist(3,0);
        double randP  = PDist(generator);
        double P = (randP < 0)? 0:randP;
        
        ihd.setP(P);
        
        normal_distribution<double> CDist(0.8,0);
        double randC  = CDist(generator);
        double C = (randC < 0)? 0:randC;
        
        ihd.setC(C);
        
        
        Person *p = new Person(i, name, age, state, flu, ihd, &myCity, homes[randHIdx],
                               schools[randSIdx], works[randWIdx], cemeteries[randCIdx], homes[randHIdx],
                               hco,wco,sco,cco,10,10,10);
        
        if (i%3 == 0){
            p->setLocation(works[randWIdx]);
        }else if (i%2 == 0){
            p->setLocation(homes[randHIdx]);
        }else {
            p->setLocation(schools[randSIdx]);
        }
        people.push_back(p);
    };
    
    
    double InitialTime = 0;
    double EndTime = 100;
    double TimeStep = 1; // TODO Fix the naming of the time files for fractional times.
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for multi location simulation: ";
        cin >> ver;
        string dataFolder = "data_multi_v"+ver+"_";
        string movieFolder = "movie_multi_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
        Architect archie(InitialTime,EndTime,TimeStep, people, econ, "MYSQL", &sqldata);
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

void Example4_MultiLocation(bool SaveData){
    
    int maxdim = 700;
    int cityBoundary[2][2]   = {{0, maxdim},{0, maxdim}};
    Domain myCity("DiseasVille", cityBoundary);
    
    vector<Place*> homes;
    vector<Place*> works;
    vector<Place*> schools;
    vector<Place*> cemeteries;
    
    //readCityData(&myCity, homes, works, schools, cemeteries);
    
    int h1p[2][2] = {{50, 250},{50, 300}};
    int hp[2][2];
    int w1p[2][2] = {{50, 250},{350, 600}};
    int wp[2][2];
    int s1p[2][2] = {{300, 600},{100, 600}};
    int sp[2][2];
    int c1p[2][2] = {{250, 450},{625, 650}};
    
    
    
    ostringstream convert;
    string hnum;
    int a = 0;
    while (a < 1){
        for (int ii = 1; ii < 2; ii++){
            for (int jj = 0; jj < 2; jj++){
                hp[0][jj] = h1p[0][jj];
                hp[ii][jj] = h1p[ii][jj] + a*360;
            }
        }
        convert << a;
        hnum = convert.str();
        Place *h = new Place(1, "Home"+hnum, "Home", hp, myCity);
        homes.push_back(h);
        convert.str(string());
        ++a;
    }
    
    ostringstream wconvert;
    string wnum;
    int b = 0;
    while (b < 1){
        for (int ii = 1; ii < 2; ii++){
            for (int jj = 0; jj < 2; jj++){
                wp[0][jj] = w1p[0][jj];
                wp[ii][jj] = w1p[ii][jj] + b*110;
            }
        }
        convert << b;
        wnum = wconvert.str();
        Place *w = new Place(1, "Work"+wnum, "Work", wp, myCity);
        works.push_back(w);
        wconvert.str(string());
        b++;
    }
    
    ostringstream sconvert;
    string snum;
    int c = 0;
    while (c < 1){
        for (int ii = 0; ii < 2; ii++){
            for (int jj = 0; jj < 2; jj++){
                sp[ii][jj] = s1p[ii][jj] + c*110;
            }
        }
        sconvert << c;
        snum = wconvert.str();
        Place *s = new Place(1, "School"+wnum, "School", sp, myCity);
        schools.push_back(s);
        wconvert.str(string());
        c++;
    }
    
    Place *ce = new Place(1, "Cemetery", "Cemetery", c1p, myCity);
    cemeteries.push_back(ce);
    
    double hco[2];
    double wco[2];
    double sco[2];
    double cco[2];
    
    int population = 700;
    
    Disease flu("Flu", 24, 30, 70);
    char state = 'S';
    double VirLev = 0.1;
    
    Economy econ(1,0.9,0.1);
    
    
    vector<Person*> people;
    for (int i=1; i <= population; i++){
        
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        
        string name = "randomName"+to_string(i);
        
        int randHIdx = rand() % homes.size();
        int randWIdx = rand() % works.size();
        int randSIdx = rand() % schools.size();
        int randCIdx = rand() % cemeteries.size();
        
        getDefaultCoordinates(homes[randHIdx], hco);
        getDefaultCoordinates(works[randWIdx], wco);
        getDefaultCoordinates(schools[randSIdx], sco);
        getDefaultCoordinates(cemeteries[randCIdx], cco);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 1)? 1:floor(randage);
        
        if (i == 1){
            VirLev = 0.1;
            // state = 'I';
        } else {
            VirLev = 0;
            state = 'S';
        }
        
        normal_distribution<double> icDist(4,1);
        double randic  = icDist(generator);
        
        double ict = (randic < 0.5)? 0.5:randic;
        
        
        InHostDynamics ihd = InHostDynamics(i,0.05,ict,0,VirLev);
        
        normal_distribution<double> betaDist(1,0);
        double randbeta  = betaDist(generator);
        double beta = (randbeta < 0)? 0:randbeta;
        
        ihd.setBeta(beta);
        
        normal_distribution<double> deltaDist(0.1,0);
        double randdelta  = deltaDist(generator);
        double delta = (randdelta < 0)? 0:randdelta;
        
        ihd.setDelta(delta);
        
        normal_distribution<double> PDist(3,0);
        double randP  = PDist(generator);
        double P = (randP < 0)? 0:randP;
        
        ihd.setP(P);
        
        normal_distribution<double> CDist(0.8,0);
        double randC  = CDist(generator);
        double C = (randC < 0)? 0:randC;
        
        ihd.setC(C);
        
        
        Person *p = new Person(i, name, age, state, flu, ihd, &myCity, homes[randHIdx],
                               schools[randSIdx], works[randWIdx], cemeteries[randCIdx], works[randWIdx],
                               hco,wco,sco,cco,10,10,10);
        
        if (i%3 == 0){
            p->setLocation(works[randWIdx]);
        }else if (i%2 == 0){
            p->setLocation(homes[randHIdx]);
        }else {
            p->setLocation(schools[randSIdx]);
        }
        people.push_back(p);
    };
   
    
    double InitialTime = 0;
    double EndTime = 100;
    double TimeStep = 1; // TODO Fix the naming of the time files for fractional times.
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for multi location simulation: ";
        cin >> ver;
        string dataFolder = "data_multi_v"+ver+"_";
        string movieFolder = "movie_multi_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
        Architect archie(InitialTime,EndTime,TimeStep, people, econ, "MYSQL", &sqldata);
        //Architect archie(InitialTime,EndTime,TimeStep, people, "FileSystem", &data);
        
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

void Ex1_Eig_SingleLocation(bool SaveData){
    int maxdim = 2000;
    
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
    int population = 2000;
    
    Disease flu("Flu", 25, 40, 200);
    InHostDynamics ihd(1, 0.1, 0, 0, 0);
    Economy econ(1,0.5,0.5);
    
    vector<Person*> people;
    for (int i=0; i < population; i++){
        string name = "randomName"+to_string(i);
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 0)? 0:floor(randage);
        
        getDefaultCoordinates(&home, hco);
        Person *p = new Person(i, name, age, 'S', flu, ihd, &myCity, &home, hco, 25,40,15, true);
        people.push_back(p);
    };
    (people.front())->setState('I');
    
    vector<Person*> people1;
    vector<Person*> people2;
    vector<Person*> people3;
    
    double InitialTime = 0;
    double EndTime  = 200;
    double TimeStep =  1;
    //int l = floor((EndTime-InitialTime)/TimeStep);
 
    
    Architect archie(InitialTime,EndTime,TimeStep, people, econ);

    string ver;
    cout << "Enter version number for single location simulation: ";
    cin >> ver;
    string dataFolderName = "eigdata_single_v"+ver+"_";
    
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    string timeStamp = to_string((now->tm_mon + 1)) + "-" +
    to_string(now->tm_mday) + "-" +
    to_string((now->tm_year + 1900));
    string eigdatafilename = "../Data/"+dataFolderName+timeStamp+"/eigHistoryData.dat";
    
    string folder = "../Data/"+dataFolderName+timeStamp;
    
    mode_t process_mask = umask(0);
    mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    umask(process_mask);
    cout << eigdatafilename << endl;
    
    ofstream eigdatafile;
    eigdatafile.open(eigdatafilename, ios_base::out|ios_base::app);
    
    eigdatafile << "t,";
    
    eigdatafile << "x_1,";
    eigdatafile << "x_2,";
    eigdatafile << "x_3,";
    
    eigdatafile << "xj_1,";
    eigdatafile << "xj_2,";
    eigdatafile << "xj_3,";
    eigdatafile << "xj_4,";
    eigdatafile << "xj_5,";
    eigdatafile << "xj_6,";
    eigdatafile << "xj_7,";
    eigdatafile << "xj_8,";
    eigdatafile << "xj_9,";
    
    eigdatafile << "xbar_1,";
    eigdatafile << "xbar_2,";
    eigdatafile << "xbar_3,";
    
    eigdatafile << "xbarj_1,";
    eigdatafile << "xbarj_2,";
    eigdatafile << "xbarj_3,";
    eigdatafile << "xbarj_4,";
    eigdatafile << "xbarj_5,";
    eigdatafile << "xbarj_6,";
    eigdatafile << "xbarj_7,";
    eigdatafile << "xbarj_8,";
    eigdatafile << "xbarj_9,";
    eigdatafile << endl;
    
    
    
    
    for (int tt = InitialTime; tt < EndTime; tt+=TimeStep){
        cout << "Time " << tt << " of " << EndTime << endl;
        matchPeople(people, people1, 'S');
        Architect archie1(archie.getCurrentTime(),EndTime,TimeStep,people1, econ);
        matchPeople(people, people2,'P');
        Architect archie2(archie.getCurrentTime(),EndTime,TimeStep,people2, econ);
        matchPeople(people, people3,'R');
        Architect archie3(archie.getCurrentTime(),EndTime,TimeStep,people3, econ);
        
        eigdatafile << tt << ",";
        eigdatafile << archie.getS() << ",";
        eigdatafile << archie.getI()+archie.getP() << ",";
        eigdatafile << archie.getR() << ",";
        
        eigdatafile << archie1.getS() << ",";
        eigdatafile << archie1.getI()+archie1.getP() << ",";
        eigdatafile << archie1.getR() << ",";
        eigdatafile << archie2.getS() << ",";
        eigdatafile << archie2.getI()+archie2.getP() << ",";
        eigdatafile << archie2.getR() << ",";
        eigdatafile << archie3.getS() << ",";
        eigdatafile << archie3.getI()+archie3.getP()<< ",";
        eigdatafile << archie3.getR() << ",";
        
        archie.Update(tt);
        archie1.Update(tt);
        archie2.Update(tt);
        archie3.Update(tt);
        
        eigdatafile << archie.getS() << ",";
        eigdatafile << archie.getI()+archie.getP() << ",";
        eigdatafile << archie.getR() << ",";
        
        eigdatafile << archie1.getS() << ",";
        eigdatafile << archie1.getI()+archie1.getP() << ",";
        eigdatafile << archie1.getR() << ",";
        
        eigdatafile << archie2.getS() << ",";
        eigdatafile << archie2.getI()+archie2.getP() << ",";
        eigdatafile << archie2.getR() << ",";
        
        eigdatafile << archie3.getS() << ",";
        eigdatafile << archie3.getI()+archie3.getP() << ",";
        eigdatafile << archie3.getR();
        
        eigdatafile << endl;

        people1.clear();
        people2.clear();
        people3.clear();
        
        
        
    }
    eigdatafile.close();
}
void Ex1_SparseEig_SingleLocation(bool SaveData){
    int maxdim = 2000;
    
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
    int population = 2000;
    
    Disease flu("Flu", 25, 15, 24);
    InHostDynamics ihd(1, 0.1, 0, 0, 0);
    Economy econ(1, 0.5, 0.5);
    
    
    vector<Person*> people;
    for (int i=0; i < population; i++){
        string name = "randomName"+to_string(i);
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 0)? 0:floor(randage);
        
        getDefaultCoordinates(&home, hco);
        Person *p = new Person(i, name, age, 'S', flu, ihd, &myCity, &home, hco, 5,10,5, true);
        people.push_back(p);
    };
    (people.front())->setState('I');
    
    vector<Person*> people1;
    vector<Person*> people2;
    vector<Person*> people3;
    
    double InitialTime = 0;
    double EndTime  = 100;
    double TimeStep =  1;
    //int l = floor((EndTime-InitialTime)/TimeStep);
    
    
    Architect archie(InitialTime,EndTime,TimeStep, people, econ);
    
    string ver;
    cout << "Enter version number for single location simulation: ";
    cin >> ver;
    string dataFolderName = "eigdata_single_v"+ver+"_";
    
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    string timeStamp = to_string((now->tm_mon + 1)) + "-" +
    to_string(now->tm_mday) + "-" +
    to_string((now->tm_year + 1900));
    string eigdatafilename = "../Data/"+dataFolderName+timeStamp+"/eigHistoryData.dat";
    
    string folder = "../Data/"+dataFolderName+timeStamp;
    
    mode_t process_mask = umask(0);
    mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    umask(process_mask);
    cout << eigdatafilename << endl;
    
    ofstream eigdatafile;
    eigdatafile.open(eigdatafilename, ios_base::out|ios_base::app);
    
    eigdatafile << "t,";
    
    eigdatafile << "x_1,";
    eigdatafile << "x_2,";
    eigdatafile << "x_3,";
    
    eigdatafile << "xj_1,";
    eigdatafile << "xj_2,";
    eigdatafile << "xj_3,";
    eigdatafile << "xj_4,";
    eigdatafile << "xj_5,";
    eigdatafile << "xj_6,";
    eigdatafile << "xj_7,";
    eigdatafile << "xj_8,";
    eigdatafile << "xj_9,";
    
    eigdatafile << "xbar_1,";
    eigdatafile << "xbar_2,";
    eigdatafile << "xbar_3,";
    
    eigdatafile << "xbarj_1,";
    eigdatafile << "xbarj_2,";
    eigdatafile << "xbarj_3,";
    eigdatafile << "xbarj_4,";
    eigdatafile << "xbarj_5,";
    eigdatafile << "xbarj_6,";
    eigdatafile << "xbarj_7,";
    eigdatafile << "xbarj_8,";
    eigdatafile << "xbarj_9,";
    eigdatafile << endl;
    
    
    
    
    for (int tt = InitialTime; tt < EndTime; tt+=TimeStep){
        cout << "Time " << tt << " of " << EndTime << endl;
        
        
        Person *p1 = new Person(archie.getPeople().size()+1, "dumbo", 5, 'S', flu, ihd, &myCity, &home, hco, 5,0,0, true);
        Architect archie1 = archie;
        archie1.AddPerson(p1);
        
        
        Person *p2 = new Person(archie.getPeople().size()+1, "dumbo", 5, 'I', flu, ihd, &myCity, &home, hco, 5,0,0, true);
        Architect archie2 = archie;
        archie2.AddPerson(p2);
        
        Person *p3 = new Person(archie.getPeople().size()+1, "dumbo", 5, 'R', flu, ihd, &myCity, &home, hco, 5,0,0, true);
        Architect archie3 = archie;
        archie3.AddPerson(p3);
        
        
        eigdatafile << tt << ",";
        eigdatafile << archie.getS() << ",";
        eigdatafile << archie.getI()+archie.getP() << ",";
        eigdatafile << archie.getR() << ",";
        
        eigdatafile << archie1.getS() << ",";
        eigdatafile << archie1.getI()+archie1.getP() << ",";
        eigdatafile << archie1.getR() << ",";
        eigdatafile << archie2.getS() << ",";
        eigdatafile << archie2.getI()+archie2.getP() << ",";
        eigdatafile << archie2.getR() << ",";
        eigdatafile << archie3.getS() << ",";
        eigdatafile << archie3.getI()+archie3.getP()<< ",";
        eigdatafile << archie3.getR() << ",";
        
        archie.Update(tt);
        archie1.Update(tt);
        archie2.Update(tt);
        archie3.Update(tt);
        
        eigdatafile << archie.getS() << ",";
        eigdatafile << archie.getI()+archie.getP() << ",";
        eigdatafile << archie.getR() << ",";
        
        eigdatafile << archie1.getS() << ",";
        eigdatafile << archie1.getI()+archie1.getP() << ",";
        eigdatafile << archie1.getR() << ",";
        
        eigdatafile << archie2.getS() << ",";
        eigdatafile << archie2.getI()+archie2.getP() << ",";
        eigdatafile << archie2.getR() << ",";
        
        eigdatafile << archie3.getS() << ",";
        eigdatafile << archie3.getI()+archie3.getP() << ",";
        eigdatafile << archie3.getR();
        
        eigdatafile << endl;
        
        
        
    }
    eigdatafile.close();
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
            cout << "ID: " << sID << endl;
            
            getline(cityFile, name, ',');
            cout << "Name: " << name << endl;

            getline(cityFile, type, ',');
            cout << "Type: " << type << endl;
            
            getline(cityFile, sxmin, ',');
            int xmin = atoi(sxmin.c_str());
            cout << "xmin: " << xmin << endl;
            
            getline(cityFile, sxmax, ',');
            int xmax = atoi(sxmax.c_str());
            cout << "xmax: " << xmax << endl;
            
            getline(cityFile, symin, ',');
            int ymin = atoi(symin.c_str());
            cout << "ymin: " << ymin << endl;
            
            getline(cityFile, symax, '\n');
            int ymax = atoi(symax.c_str());
            cout << "ymax: " << ymax << endl;
            
            
            
            int boundary[2][2] = {{xmin, xmax},{ymin, ymax}};
            string test = "Home";
            
            if (type=="Home"){
                cout << "Im here" << endl;
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