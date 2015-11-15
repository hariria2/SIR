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
    
    Architect(double t0, double te, double ts,vector<Person *> pp,Visualization* vis);
    Architect(double t0, double te, double ts,vector<Person *> pp,Visualization* vis, string store, SQLStorage* d);
    
    virtual ~Architect();
    

	// Setters
    void setDomain(Domain *city);
    void setPlaces(vector<Place*> places);
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
    Domain* getDomain();
	vector<Person*> getPeople();
    vector<Place*> getPlaces();
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
    Economy* _Econ;
	vector<Person*> _PeoplePtr;
    Domain* _City;
    vector<Place*> _AllPlaces;
    SQLStorage* _sqlDataPtr;
	Storage* _dataPtr;
	string _Store = "None";
    Visualization* _Visualization;
	int _S;
	int _I;
    int _P;
	int _R;
    int _D;
};
#endif /* ARCHITECT_H_ */
