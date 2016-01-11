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
#include "Zone.h"

using namespace std;

class Person;
class Zone;

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
    void addZone(Zone* z);
	void removePerson(Person* p);
    void setZones();
    void setZoneNeighborhood();
    

	int getID();
	string getName();
	string getType();
	string getLocation();
    int getInfectionRadius();
    list<Zone*>* getZones();
	list<Person*> getOccupants();
    int getXZones();
    int getYZones();
	bool operator == (const Place& p) const;

private:
	int _ID;
    int _InfectionRadius;
	string _Name;
	string _Type;
	Domain _Location;
    list<Zone*> _Zones;
	list<Person*> _Occupants;
    int _XZones;
    int _YZones;

};

#endif /* PLACE_H_ */
