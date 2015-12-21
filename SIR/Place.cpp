/*
 * Place.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */

#include "Place.h"

using namespace std;


Place::Place(int id, string name, string type, double perimeter[2][2], Domain location):
    _Location(location)
{
	setID(id);
	setName(name);
	setType(type);
	setPerimeter(perimeter);
    //_Occupants = *new list<Person*>;
}// end constructor

// setters
void Place::setID(int id){
	_ID = id;
}
void Place::setName(string name){
	_Name = name;
}
void Place::setType(string type){
	_Type = type;
}
void Place::setPerimeter(double perimeter[2][2]){
	for (int ii=0; ii<2; ii++){
		for (int jj=0; jj<2; jj++){
			Perimeter[ii][jj] = perimeter[ii][jj];
		}
	}
}
void Place::addPerson(Person* p){
	_Occupants.push_back(p);
}
void Place::removePerson(Person* p){
	_Occupants.remove(p);
}

// getters
int Place::getID(){
	return _ID;
}
string Place::getName(){
	return _Name;
}
string Place::getType(){
	return _Type;
}
string Place::getLocation(){
	return _Location.getName();
}
list<Person*> Place::getOccupants(){
	return _Occupants;
}

Place::~Place(){
    //delete _Occupants;
}
