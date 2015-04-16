
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
#include "Person.h"
#include "Architect.h"
#include "Storage.h"
 
using namespace std;

// Function prototyping
void GenerateHomes(vector<Place*> &homes, int perimeter[2][2], int homesize[2], Domain domain);
void getDefaultCoordinates(Place* location, double co[2]);
void readCityData(Domain *city, vector<Place*> &homes, vector<Place*> &works, vector<Place*> &schools, vector<Place*> &cemeteries);
void readPeopleData();
void generateSourceData();

void Example1_SingleLocation(bool SaveData=true);
void Example1_MultiLocation(bool SaveData=true);
void Example2_MultiLocation(bool SaveData=true);

void Ex1_Eig_SingleLocation(bool SaveData=true);
//=================

// ========================= Main ======================
int main(){
    
    //Example1_SingleLocation();
    //Example1_MultiLocation();
    //Example2_MultiLocation();
 	Ex1_Eig_SingleLocation(true);
    return 0;
 }
// ========================= End main =================


// ========================= Example Simulations ==============================
void Example1_SingleLocation(bool SaveData){
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
    
    vector<Person*> people;
    for (int i=0; i < population; i++){
        string name = "randomName"+to_string(i);
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 0)? 0:floor(randage);
        
        getDefaultCoordinates(&home, hco);
        Person *p = new Person(i, name, age, 'S', flu, &myCity, &home, hco, 25,40,15, true);

        people.push_back(p);
    };
    (people.front())->setState('I');
    
    double InitialTime = 0;
    double EndTime = 200;
    double TimeStep = 1;
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for single location simulation: ";
        cin >> ver;
        string dataFolder = "data_single_v"+ver+"_";
        string movieFolder = "movie_single_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        Architect archie(InitialTime,EndTime,TimeStep, people, true, &data);
        archie.Simulate();
        data.writeSIR();
    }else{
        Architect archie(InitialTime,EndTime,TimeStep, people);
        archie.Simulate();
    }
}
void Example1_MultiLocation(bool SaveData){
    
    int maxdim = 2000;
    
    // Setting up parameters
    int cityBoundary[2][2]   = {{0, maxdim},{0, maxdim}};
    int homeBoundary[2][2]   = {{1400,1900},{100, 1900}};
    int homeBoundary1[2][2]  = {{1400,1900},{100, 800}};
    int homeBoundary2[2][2]  = {{1400,1900},{900, 1900}};
    int schoolBoundary[2][2] = {{500 ,1000},{100,  800}};
    int workBoundary[2][2]   = {{100  ,800},{1200,1600}};
    int cemeBoundary[2][2]   = {{100, 400},{100, 400}};
    
    
    // Instantiate Classes
    Domain myCity("DiseasVille", cityBoundary);
    
    Place school(1, "DiseaseHigh", "School", schoolBoundary, myCity);
    Place home(1, "home", "Home", homeBoundary,myCity);
    Place home1(1, "home", "Home", homeBoundary1,myCity);
    Place home2(1, "home", "Home", homeBoundary2,myCity);
    Place work(1, "work", "Work", workBoundary, myCity);
    Place cemetery(1, "cemetery", "Cemetery", cemeBoundary,myCity);
    
    double hco[2];
    double wco[2];
    double sco[2];
    double cco[2];
    
    vector<Place*> homes;
    vector<Place*> works;
    vector<Place*> schools;
    vector<Place*> cemeteries;

    //GenerateHomes(homes,homeBoundary,hsize,myCity);
    
    //homes.push_back(&home);
    homes.push_back(&home1);
    //homes.push_back(&home2);
    works.push_back(&work);
    schools.push_back(&school);
    cemeteries.push_back(&cemetery);
    
    int population = 1000;
    
    Disease flu("Flu", 24, 34, 40);
    
    vector<Person*> people;
    for (int i=0; i < population; i++){
        string name = "randomName"+to_string(i);
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        normal_distribution<double> distribution(10,3);
        double randnum  = distribution(generator);
        int a = (randnum < 0)? 0:floor(randnum);
        a = (a % 2) + 1;
        getDefaultCoordinates(homes.front(), hco);
        getDefaultCoordinates(&work, wco);
        getDefaultCoordinates(&school, sco);
        getDefaultCoordinates(&cemetery, cco);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 0)? 0:floor(randage);
        
        Person *p = new Person(i, name, age, 'S', flu, &myCity, homes.front(),
                               &school, &work, &cemetery, homes.front(),
                               hco,wco,sco,cco,5,5,5);
        
        p->setLocation(homes.front());
        (a==1)? p->setLocation(homes.front()):p->setLocation(homes.back());
        people.push_back(p);
    };
    (people.front())->setState('I');
    
    double InitialTime = 0;
    double EndTime = 200;
    double TimeStep = 1; // TODO Fix the naming of the time files for fractional times.
    int l = floor((EndTime-InitialTime)/TimeStep);

    if (SaveData) {
        string ver;
        cout << "Enter version number for single location simulation: ";
        cin >> ver;
        string dataFolder = "data_multi_v"+ver+"_";
        string movieFolder = "movie_multi_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        Architect archie(InitialTime,EndTime,TimeStep, people, true, &data);
        archie.Simulate();
        data.writeSIR();
    }else{
        Architect archie(InitialTime,EndTime,TimeStep, people);
        archie.Simulate();
    }
}
void Example2_MultiLocation(bool SaveData){
    
    int maxdim = 2000;
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
    
    int population = 2000;
    
    Disease flu("Flu", 24, 30, 30);
    
    vector<Person*> people;
    for (int i=0; i < population; i++){
        
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
        int age = (randage < 0)? 0:floor(randage);
        
        Person *p = new Person(i, name, age, 'S', flu, &myCity, homes[randHIdx],
                               schools[randSIdx], works[randWIdx], cemeteries[randCIdx], homes[randHIdx],
                               hco,wco,sco,cco,10,10,10);
        
        p->setLocation(homes[randHIdx]);
        people.push_back(p);
    };
    (people.front())->setState('I');
    
    double InitialTime = 0;
    double EndTime = 400;
    double TimeStep = 1; // TODO Fix the naming of the time files for fractional times.
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for single location simulation: ";
        cin >> ver;
        string dataFolder = "data_multi_v"+ver+"_";
        string movieFolder = "movie_multi_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        Architect archie(InitialTime,EndTime,TimeStep, people, true, &data);
        archie.Simulate();
        data.writeSIR();
    }else{
        Architect archie(InitialTime,EndTime,TimeStep, people);
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
    
    vector<Person*> people;
    for (int i=0; i < population; i++){
        string name = "randomName"+to_string(i);
        unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        
        normal_distribution<double> ageDist(25,20);
        double randage  = ageDist(generator);
        int age = (randage < 0)? 0:floor(randage);
        
        getDefaultCoordinates(&home, hco);
        Person *p = new Person(i, name, age, 'S', flu, &myCity, &home, hco, 25,40,15, true);
        
        people.push_back(p);
    };
    //(people.front())->setState('I');
    
    double InitialTime = 0;
    double EndTime = 50;
    double TimeStep = 1;
    int l = floor((EndTime-InitialTime)/TimeStep);
    
    
    if (SaveData) {
        string ver;
        cout << "Enter version number for single location simulation: ";
        cin >> ver;
        string dataFolder = "data_single_v"+ver+"_";
        string movieFolder = "movie_single_v"+ver+"_";
        Storage data(l, &myCity, homes, works, schools, cemeteries, dataFolder,movieFolder);
        Architect archie(InitialTime,EndTime,TimeStep, people, true, &data);
        archie.Simulate();
        data.writeSIR();
    }else{
        Architect archie(InitialTime,EndTime,TimeStep, people);
        archie.Simulate();
        cout << archie.getS() << endl;
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
    string cityFileName = "../Source/cityPlan.csv";
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
            
            getline(cityFile, symax, '\r');
            int ymax = atoi(symax.c_str());
            cout << "ymax: " << ymax << endl;
            
            
            
            int boundary[2][2] = {{xmin, xmax},{ymin, ymax}};

            if (type == "Home"){
                cout << "Im here" << endl;
                Place *h = new Place(ID, name, type, boundary, *city);
                homes.push_back(h);
            }else if (type == "Work"){
                Place *w = new Place(ID, name, type, boundary, *city);
                works.push_back(w);
            }else if (type == "School"){
                Place *s = new Place(ID, name, type, boundary, *city);
                schools.push_back(s);
            }else if (type == "Cemetery"){
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
// ========================= End utilities ======================================