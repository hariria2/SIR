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
    _InfectionRadius = 40;
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
void Place::setInfectionRadius(int r){
    _InfectionRadius = r;
}


void Place::setDistanceMatrix(){
    double x1, x2, y1, y2;
    int id1, id2;
    //string key1, key2;
    double val;
    for (auto p1=_Occupants.cbegin(); p1 != _Occupants.cend(); ++p1) {
        auto p2 = p1; ++p2;
        for (; p2 != _Occupants.cend();++p2){
            id1 = (*p1)->getID();
            id2 = (*p1)->getID();
            x1 = ((*p1)->getCoordinates())[0];
            y1 = ((*p1)->getCoordinates())[1];
            x2 = ((*p2)->getCoordinates())[0];
            y2 = ((*p2)->getCoordinates())[1];
            //key1 = to_string(id1)+to_string(id2);
            //key2 = to_string(id1)+to_string(id2);
            val = Distance(x1,y1,x2,y2);
            _DistanceMatrix[id1][id2] = val;
            _DistanceMatrix[id2][id1] = val;
        }
    }
}

void Place::addPerson(Person* p){
	(_Occupants).push_back(p);
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

int Place::getInfectionRadius(){
    return _InfectionRadius;
}
list<Person*>* Place::getOccupants(){
    return &_Occupants;
}

double Place::Distance(double x1, double y1, double x2, double y2){
    return sqrt(pow((x2-x1),2) + pow((y2 - y1),2));
}


Place::~Place(){
}
