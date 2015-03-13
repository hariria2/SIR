/*
 * Architect.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Sahand
 */

#ifndef ARCHITECT_H_
#define ARCHITECT_H_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Storage.h"

using namespace std;

class Person;

class Architect {
public:
	Architect(double t0, double tend, double ts, vector<Person *> pp, bool store, Storage* d);
    Architect(double t0, double tend, double ts, vector<Person *> pp, bool store=false);
	virtual ~Architect();

	// Setters

	// Getters
	double getCurrentTime();
	double getTimeStep();
	double getDailyTime();
	vector<Person*> getPeople();

	// Utilities
	void IncrementTime();
	void Simulate();
	void Update(double t, Storage* dPtr);
	void Update(double t);
	void DisplayTime();
	void PopulationData();

private:
	double InitialTime;
	double EndTime;
	double TimeStep;
	double CurrentTime;
	int TimeIndex;
	vector<Person*> PeoplePtr;
	Storage* dataPtr;
	bool Store;
	int S;
	int I;
    int P;
	int R;
    int D;
};
#endif /* ARCHITECT_H_ */
