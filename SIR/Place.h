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
#include "Domain.h"

using namespace std;

class Person;

class Place {


public:
	double Perimeter[2][2];
	Place(int id, string name, string type, double perimeter[2][2], Domain location);
	~Place();
	void setID(int x);
	void setName(string x);
	void setType(string x);
	void setPerimeter(double Perimeter[2][2]);
    void setInfectionRadius(int r);
	void addPerson(Person* p);
	void removePerson(Person* p);
    

	int getID();
	string getName();
	string getType();
	string getLocation();
    int getInfectionRadius();
	list<Person*>* getOccupants();
	bool operator == (const Place& p) const;

private:
	int _ID;
    int _InfectionRadius;
	string _Name;
	string _Type;
	Domain _Location;
	list<Person*> _Occupants;

};

#endif /* PLACE_H_ */
