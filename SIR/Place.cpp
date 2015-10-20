/*
 * Place.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */

#include "Place.h"

using namespace std;


Place::Place(int id, string name, string type, int perimeter[2][2], Domain location)
	: Location(location)
{
	setID(id);
	setName(name);
	setType(type);
	setPerimeter(perimeter);
    Occupants = new list<Person*>;
}// end constructor

// setters
void Place::setID(int id){
	ID = id;
}
void Place::setName(string name){
	Name = name;
}
void Place::setType(string type){
	Type = type;
}
void Place::setPerimeter(int perimeter[2][2]){
	for (int ii=0; ii<2; ii++){
		for (int jj=0; jj<2; jj++){
			Perimeter[ii][jj] = perimeter[ii][jj];
		}
	}
}
void Place::addPerson(Person* p){
	Occupants->push_back(p);
}
void Place::removePerson(Person* p){
	Occupants->remove(p);
}

// getters
int Place::getID(){
	return ID;
}
string Place::getName(){
	return Name;
}
string Place::getType(){
	return Type;
}
string Place::getLocation(){
	return Location.getName();
}
list<Person*>* Place::getOccupants(){
	return Occupants;
}

Place::~Place(){

}
