/*
 * Architect.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Sahand
 */

#ifndef ARCHITECT_H_
#define ARCHITECT_H_

#include <iostream>
#include <list>
#include "Storage.h"
#include "SQLStorage.h"
#include "Domain.h"
#include "Place.h"
#include "Economy.h"
#include "Visualization.h"

using namespace std;

class Person;

class Architect {
public:
    Architect(double t0, double tend, double ts, vector<Person *> pp, Economy econ, string store, Storage* d, bool eig);
	Architect(double t0, double tend, double ts, vector<Person *> pp, Economy econ, string store, Storage* d);
    Architect(double t0, double tend, double ts, vector<Person *> pp, Economy econ, string store, SQLStorage* d);
    Architect(double t0, double tend, double ts, vector<Person *> pp, Economy econ, string store, SQLStorage* d, Visualization* vis);
    Architect(double t0, double tend, double ts, vector<Person *> pp, Economy econ, string store="None");
	
    
    virtual ~Architect();
    

	// Setters
    void setDomain(Domain city);
    void setHomes(vector<Place*> homes);
    void setSchools(vector<Place*> schools);
    void setWorks(vector<Place*> works);
    void setCemetaries(vector<Place*> cemetaries);
    void setVisualization(Visualization* vis);
    
	// Getters
	double getCurrentTime();
	double getTimeStep();
	double getDailyTime();
    int getS();
    int getI();
    int getP();
    int getR();
    int getWo();
    int getHo();
    int getSc();
	vector<Person*> getPeople();
    Visualization* getVisualization();

	// Utilities
	void IncrementTime();
	void Simulate();
	void Update(Storage* dPtr);
    void Update(SQLStorage* dPtr);
	void Update();
	void DisplayTime();
	void PopulationData();
    void AddPerson(Person *p);
    void PrepDB();

private:
	double InitialTime;
	double EndTime;
	double TimeStep;
	double CurrentTime;
	int TimeIndex;
    Economy Econ;
	vector<Person*> PeoplePtr;
    Domain City;
    vector<Place*> Homes;
    vector<Place*> Schools;
    vector<Place*> Works;
    vector<Place*> Cemeteries;
    SQLStorage* sqlDataPtr;
	Storage* dataPtr;
	string Store;
    Visualization* _Visualization;
	int S;
	int I;
    int P;
	int R;
    int D;
    int Wo;
    int Sc;
    int Ho;
};
#endif /* ARCHITECT_H_ */
