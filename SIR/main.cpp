#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Domain.h"
#include "Place.h"
#include "InHostDynamics.h"
#include "Person.h"
#include "Architect.h"
#include "SQLStorage.h"
#include "Source.hpp"
#include <thread>

using namespace std;

// Function prototyping
template <typename T>
vector<T> operator+(const vector<T> &A, const vector<T> &B);
template <typename T>
vector<T> &operator+=(vector<T> &A, const vector<T> &B);

void GenerateHomes(vector<Place*> &homes, int perimeter[2][2], int homesize[2], Domain domain);
void getDefaultCoordinates(Place* location, double co[2]);
void readIslandData(string FileName, Domain *city, vector<Place*> &islands);

void FaroeIslands(double EndTime, double TimeStep, string ver, bool SaveData=false, bool ShowVis=true);
void SingleLocation(double EndTime, double TimeStep, string ver, bool SaveData=true, bool ShowVis=true);

//=================




// ========================= Main ======================

double dt = 1;
double tend = 36000;
const double ageIncrement = dt/365;
string version = "2";
int main(){
	/** 
	 * \brief Brief description.
	 *         Brief description continued.
	 *
	 *  Detailed description starts here.
	 */
	
	FaroeIslands(tend, dt, version, true, false);
	//SingleLocation(tend, dt, version, true, false);
	
	
	return 0;
}
// ========================= End main =================


// ========================= Example Simulations ==============================

void SingleLocation(double EndTime, double TimeStep, string ver, bool SaveData, bool ShowVis){
	int maxdim = 115;
	int Boundary[2][2]   = {{0, maxdim},{0, maxdim}};
	
	int population = 500;
	char state = 'S';
	double VirLev = 0.0;
	
	Domain Main("Room", Boundary);
	double LBoundary[2][2]= {{0, 20},{0, 20}};
	Place *MainPlace = new Place(1,"MainRoom","Home",LBoundary,Main,population);
	vector<Place*> locations;
	locations.push_back(MainPlace);
	
	
	unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	
	normal_distribution<double> ageDist(25,0);
	normal_distribution<double> suDist(3.,0);
	normal_distribution<double> icDist(.7,0);
	normal_distribution<double> betaDist(3.,.5);
	normal_distribution<double> deltaDist(0.3,0);
	normal_distribution<double> PDist(0.2,0);
	normal_distribution<double> CDist(0.2,0);
	normal_distribution<double> ILDist(0.0001,0.0);
	
	vector<Person*> people;
	list<Person*> vpeople;
	
	for (int ii=1; ii <= population; ii++){
		string name = "randomName"+to_string(ii);
		double randage  = ageDist(generator);
		double age = (randage < 1)? 1:floor(randage);
		if (ii == 1){
			VirLev = 0.1;
		} else {
			VirLev = 0;
			state = 'S';
		}
		double randic  = icDist(generator);
		double ict = (randic < 0.5)? 0.5:randic;
		double randsu  = suDist(generator);
		double sus = (randsu < 0.5)? 0.5:randsu;
		InHostDynamics ihd = InHostDynamics(ii, 0.01 ,sus,0.0,VirLev,ict,44, 40, 100);
		double randil = ILDist(generator);
		double il = (randil < 0.0005)? 0.0005:randil;
		ihd.setILRate(il);
		double randbeta  = betaDist(generator);
		double beta = (randbeta < 0.01)? 0.01:randbeta;
		ihd.setBeta(beta);
		double randdelta  = deltaDist(generator);
		double delta = (randdelta < 0.01)? 0.01:randdelta;
		ihd.setDelta(delta);
		double randP  = PDist(generator);
		double P = (randP < 0.01)? 0.01:randP;
		ihd.setP(P);
		double randC  = CDist(generator);
		double C = (randC < 0.01)? 0.01:randC;
		ihd.setC(C);
		
		Person *ip = new Person(ii, name, age, state, ihd,
														&Main, locations,10,10,10, true);
		ip->setAgeIncrement(ageIncrement);
		ip->setMotionStepSize(0.1);
		ip->setTimeStep(dt);
		people.push_back(ip);
		vpeople.push_back(ip);
		
	}
	
	
	double InitialTime = 0;
	//int l = floor((EndTime-InitialTime)/TimeStep);
	//cout << "Enter version number for multi location simulation: ";
	//cin >> ver;
	
	
	if (ShowVis & SaveData){
		
		int xdim = maxdim;
		int ydim = maxdim;
		Visualization* vis = getVisualization(xdim, ydim, true);
		vis->setPlaces(locations);
		vis->setPeople(vpeople);
		
		
		SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
		Architect archie(InitialTime,EndTime,TimeStep, vpeople, vis, "MYSQL", &sqldata);
		
		vis->Init();
		vis->setArchitect(&archie);
		//vis->RenderSplash();
		archie.setDomain(&Main);
		archie.setPlaces(locations);
		archie.setBatchSize(1);
		archie.setSaveIntegerTimes(true);
		archie.Simulate();
	}
	else if (ShowVis){
		int xdim = maxdim;
		int ydim = maxdim;
		Visualization* vis = getVisualization(xdim, ydim, true);
		vis->setPlaces(locations);
		vis->setPeople(vpeople);
		
		
		Architect archie(InitialTime,EndTime,TimeStep, vpeople, vis);
		vis->Init();
		vis->setArchitect(&archie);
		archie.setDomain(&Main);
		archie.setPlaces(locations);
		archie.setBatchSize(1);
		archie.Simulate();
	}
	else {
		SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
		Architect archie(InitialTime,EndTime,TimeStep, vpeople,  "MYSQL", &sqldata);
		archie.setDomain(&Main);
		archie.setPlaces(locations);
		archie.setBatchSize(1);
		archie.setSaveIntegerTimes(true);
		archie.Simulate();
	}
	
	
	
}

void FaroeIslands(double EndTime, double TimeStep, string ver, bool SaveData, bool ShowVis){
	
	int maxdim = 110;
	int Boundary[2][2]   = {{0, 105},{0, 80}};
	//int Boundary[2][2]   = {{-8, -6},{61, 63}};
	Domain Island("Faroe", Boundary);


	vector<Place*> islands;



	if (ShowVis){
		Source src("/Users/sahand/Research/SIR/Source/");
		src.readGeneralData("GeneralDataVis.csv", &Island);
		src.getCoordinateDataForPlaces();
		src.getPolygonDataForPlaces();
		islands = src.getPlaces();
	}
	else{
		Source src("/Users/sahand/Research/SIR/Source/");
		src.readGeneralData("GeneralData.csv", &Island);
		src.getCoordinateDataForPlaces();
		islands = src.getPlaces();
	}


	
	char state = 'S';
	double VirLev = 0.0;
	
	unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	
	normal_distribution<double> ageDist(40,35);
	normal_distribution<double> suDist(3.5,0.5);            // Susceptibility (S)
	normal_distribution<double> icDist(2,0.1);              // Initial Condition
	normal_distribution<double> betaDist(0.1,0.05);          // Beta (rate of decay of T cells)
	normal_distribution<double> deltaDist(0.1,0.005);        // Delta (rate of decay of I cells)
	normal_distribution<double> PDist(.4,0.05);             // P (rate of growth of Virions)
	normal_distribution<double> CDist(.5,0.05);            // C (rate of decay of Virions)
	normal_distribution<double> ILDist(0.001,0.0001);     // No idea what this does

	normal_distribution<double> sociability(0.2,1);

	vector<Person*> people;
	list<Person*> vpeople;
	
	double randic, ict;
	double randsu, sus;
	double randil, il;
	double randbeta, beta;
	double randdelta, delta;
	double randP, P;
	double randC, C;

	for(auto p=islands.begin(); p!=islands.end();++p){

		for (int ii = 1; ii <= (*p)->getTotalPopulation(); ii++){
			string name = "randomName"+to_string(ii);
			double randage  = ageDist(generator);
			double age = (randage < 1)? 1:floor(randage);
			//if (ii == 1){
			//    VirLev = 0.1;
			//}
			
			randic  = icDist(generator);
			ict     = (randic < 0.5)? 0.5:randic;
			randsu  = suDist(generator);
			sus     = (randsu < 0.5)? 0.5:randsu;
			
			InHostDynamics ihd = InHostDynamics(ii,0.01 ,sus,0.0,VirLev,ict,44, 40, 100);
			
			randil = ILDist(generator);
			il     = (randil < 0.0005)? 0.0005:randil;
			ihd.setILRate(il);
			
			randbeta  = betaDist(generator);
			beta      = (randbeta < 0.01)? 0.01:randbeta;
			ihd.setBeta(beta);
			
			randdelta  = deltaDist(generator);
			delta      = (randdelta < 0.01)? 0.01:randdelta;
			ihd.setDelta(delta);
			
			randP  = PDist(generator);
			P      = (randP < 0.01)? 0.01:randP;
			ihd.setP(P);
			
			randC  = CDist(generator);
			C      = (randC < 0.01)? 0.01:randC;
			ihd.setC(C);

			if (age<10){
				state='B';
				ihd.setT(0);
			}
			
			Person *ip = new Person(ii, name, age, state, ihd,
															&Island, (*p),islands,10,10,10);
			ip->setAgeIncrement(ageIncrement);
			ip->setMotionStepSize(0.1);
			ip->setTimeStep(dt);
			ip->setSociability(sociability(generator));
			people.push_back(ip);
			vpeople.push_back(ip);
			
		}
		
	}
	
	
	//    srand((int) time(NULL));
	//    for (int ii=0; ii < 3; ii++){
	//        people[rand()%(people.size())]->setTravelerQ(true);
	//    }
	
	
	double InitialTime = 0;
	
	
	if (ShowVis & SaveData){
		
		int xdim = maxdim;
		int ydim = maxdim;
		Visualization* vis = getVisualization(xdim, ydim, true);
		vis->setPlaces(islands);
		vis->setPeople(vpeople);
		
		
		SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
		Architect archie(InitialTime,EndTime,TimeStep, vpeople, vis, "MYSQL", &sqldata);
		
		vis->Init();
		vis->setArchitect(&archie);
		//vis->RenderSplash();
		archie.setDomain(&Island);
		archie.setPlaces(islands);
		archie.setSaveIntegerTimes(true);
		archie.setBatchSize(50);
		archie.Simulate();
	}
	else if (ShowVis){
		int xdim = maxdim;
		int ydim = maxdim;
		Visualization* vis = getVisualization(xdim, ydim, true);
		vis->setPlaces(islands);
		vis->setPeople(vpeople);
		
		
		Architect archie(InitialTime,EndTime,TimeStep, vpeople, vis);
		vis->Init();
		vis->setArchitect(&archie);
		archie.setDomain(&Island);
		archie.setPlaces(islands);
		archie.setSaveIntegerTimes(true);
		archie.setBatchSize(50);
		archie.Simulate();
	}
	else {
		cout << "version: " << ver << endl;
		SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
		Architect archie(InitialTime,EndTime,TimeStep, vpeople,  "MYSQL", &sqldata);
		archie.setDomain(&Island);
		archie.setPlaces(islands);
		archie.setSaveIntegerTimes(true);
		archie.setBatchSize(5);
		archie.Simulate();
	}
	
	
}



// ========================= End Examples ======================================

// ========================= Utilities ===========================================

template <typename T>
vector<T> operator+(const vector<T> &A, const vector<T> &B)
{
	std::vector<T> AB;
	AB.reserve( A.size() + B.size() );                // preallocate memory
	AB.insert( AB.end(), A.begin(), A.end() );        // add A;
	AB.insert( AB.end(), B.begin(), B.end() );        // add B;
	return AB;
}

template <typename T>
vector<T> &operator+=(vector<T> &A, const vector<T> &B)
{
	A.reserve( A.size() + B.size() );                // preallocate memory without erasing original data
	A.insert( A.end(), B.begin(), B.end() );         // add B;
	return A;                                        // here A could be named AB
}

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
 
	double perim[2][2];
	for (int i=0; i<total; i++){
		for (int n=0;n<2;n++){
			perim[0][n] = homedimx[i][n];
			perim[1][n] = homedimy[i][n];
		}
		Place* p = new Place(i, "home", "Home", perim, domain,1);
		homes.push_back(p);
	}
}
void getDefaultCoordinates(Place* location, double co[2]){
	
	double x = fmod(rand(),(location->Perimeter[0][1] - location->Perimeter[0][0] + 1)) + location->Perimeter[0][0];
	double y = fmod(rand(),(location->Perimeter[1][1] - location->Perimeter[1][0] + 1)) + location->Perimeter[1][0];
	co[0] = x;
	co[1] = y;
}
void readIslandData(string FileName, Domain *city, vector<Place*> &islands){
	
	ifstream File;
	File.open(FileName, ios_base::in);
	
	if (File.is_open()){
		cout << "Island File opened correctly." << endl;
		
		
		string sID;
		string name;
		string type;
		string sxmin;
		string sxmax;
		string symin;
		string symax;
		string spop;
		
		while (File.good()){
			getline(File, sID, ',');
			int ID = atoi(sID.c_str());
			
			getline(File, name, ',');
			
			getline(File, type, ',');
			
			getline(File, sxmin, ',');
			double xmin = atof(sxmin.c_str());
			
			getline(File, sxmax, ',');
			double xmax = atof(sxmax.c_str());
			
			getline(File, symin, ',');
			double ymin = atof(symin.c_str());
			
			getline(File, symax, ',');
			double ymax = atof(symax.c_str());
			
			getline(File, spop, '\r');
			int pop = atof(spop.c_str());
			
			
			if (File.eof()) break;
			
			double boundary[2][2] = {{xmin, xmax},{ymin, ymax}};
			cout << "Population " << pop << endl;
			Place *h = new Place(ID, name, type, boundary, *city, pop);
			islands.push_back(h);
			
		}
		cout << "Finished reading the file. Will close now. " << endl;
		File.close();
	} else {
		cout << "File did not open correctly." << endl;
	}
};
// ========================= End utilities ======================================