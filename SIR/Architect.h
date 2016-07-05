#ifndef ARCHITECT_H_
#define ARCHITECT_H_

#include <iostream>
#include <list>
#include "SQLStorage.h"
#include "Domain.h"
#include "Place.h"
#include "Visualization.h"
#include "Person.h"
#include <random>
#include <vector>


using namespace std;

class Visualization;

/**
 * Architect.h
 *
 *  \date Jan 9, 2015
 *
 *  \author Sahand
 *
 * The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
 */

class Architect {

public:
	
	Architect(double t0, double te, double ts,list<Person *> pp,Visualization* vis);
	Architect(double t0, double te, double ts,list<Person *> pp, string store, SQLStorage* d);
	Architect(double t0, double te, double ts,list<Person *> pp,Visualization* vis, string store, SQLStorage* d);
	
	virtual ~Architect();
	
	
	// Setters
	/// setDomain Docs
	void setDomain(Domain *city);
	/// setPlaces Docs
	void setPlaces(vector<Place*> places);
	///setVisualization Docs
	void setVisualization(Visualization* vis);
	void setBatchSize(int btchsz);
	void setSaveIntegerTimes(bool siono);
	
	// Getters
	double getCurrentTime();
	double getTimeStep();
	double getMonthlyTime();
	int getBatchSize();
	int getS();
	int getI();
	int getP();
	int getR();
	int getD();
	int getN();
	bool getSaveIntegerTimes();
	Domain* getDomain();
	list<Person*> getPeople();
	vector<Place*> getPlaces();
	Visualization* getVisualization();
	
	// Utilities
	void IncrementTime();
	/// Loops through time and calls all the relevant functions.
	void Simulate();
	void Update(SQLStorage* dPtr);
	void Update();
	void DisplayTime();
	void PopulationData();
	void AddPerson(Person *p);
	void RemovePerson(Person *p);
	void PrepDB();
	
	void AddPerson(double x, double y);
	void AddPerson(string s);
	Place* LocFromCoo(double x, double y);
	
	void Funeral(Person* p);
	
private:
	list<Person*> _PeoplePtr;
	vector<Place*> _AllPlaces;
	double _InitialTime;
	double _EndTime;
	double _TimeStep;
	double _CurrentTime;
	int _TimeIndex;
	Domain* _City;
	SQLStorage* _sqlDataPtr;
	string _Store = "None";
	Visualization* _Visualization = NULL;
	unsigned _RandSeed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine *_generator;
	uniform_int_distribution<int> *_introtimeDist;
	bool _SaveIntegerTimes;
	int _SQLBatchSize;
	double _BirthRate;
	int _S;
	int _I;
	int _P;
	int _R;
	int _D;
	int _N;
	int _B;
};
#endif /* ARCHITECT_H_ */
