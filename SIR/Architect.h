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

class Visualization;

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
    int getD();
    int getWo();
    int getHo();
    int getSc();
    Domain* getDomain();
    
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
    
    void AddPerson(double x, double y);
    Place* LocFromCoo(double x, double y);

private:
	double _InitialTime;
	double _EndTime;
	double _TimeStep;
	double _CurrentTime;
	int _TimeIndex;
    Economy _Econ;
	vector<Person*> _PeoplePtr;
    Domain _City;
    vector<Place*> _Homes;
    vector<Place*> _Schools;
    vector<Place*> _Works;
    vector<Place*> _Cemeteries;
    SQLStorage* _sqlDataPtr;
	Storage* _dataPtr;
	string _Store;
    Visualization* _Visualization;
	int _S;
	int _I;
    int _P;
	int _R;
    int _D;
    int _Wo;
    int _Sc;
    int _Ho;
};
#endif /* ARCHITECT_H_ */
