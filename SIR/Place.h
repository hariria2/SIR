/*
 * Place.h
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */

#ifndef PLACE_H_
#define PLACE_H_

#include <iostream>
#include <list>
#include <unordered_map>
#include "Domain.h"
#include "Person.h"

using namespace std;

class Person;

class Place {


public:
	double Perimeter[2][2];
    //unordered_map<string, double> _DistanceMatrix;
    double _DistanceMatrix[30000][30000];
    
	Place(int id, string name, string type, double perimeter[2][2], Domain location, int pop);
	~Place();
	void setID(int x);
	void setName(string x);
	void setType(string x);
	void setPerimeter(double Perimeter[2][2]);
    void setInfectionRadius(int r);
    void setDistanceMatrix();
    void setTotalPopulation(int pop);
    
	void addPerson(Person* p);
	void removePerson(Person* p);
    

	int getID();
    int getTotalPopulation();
	string getName();
	string getType();
	string getLocation();
    int getInfectionRadius();
	list<Person*>* getOccupants();
	bool operator == (const Place& p) const;

    double Distance(double x1, double y1, double x2, double y2);
    
private:
    int _TotalPopulation;
	list<Person*> _Occupants;
    int _ID;
    int _InfectionRadius;
	string _Name;
	string _Type;
	Domain _Location;
	

};

#endif /* PLACE_H_ */
