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
    setZones();
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
void Place::setInfectionRadius(int r){
    _InfectionRadius = r;
}
void Place::addPerson(Person* p){
	_Occupants.push_back(p);
}
void Place::addZone(Zone *z){
    _Zones.push_back(z);
}
void Place::removePerson(Person* p){
	_Occupants.remove(p);
}

void Place::setZones(){
    double X = Perimeter[0][1];
    double Y = Perimeter[1][1];
    
    double ZX = 0;
    double ZY = 0;
    double p[2][2];
    int id = 1;
    
    while (ZY < Y) {
        _XZones = 0;
        _YZones += 1;
        p[1][0] = ZY; p[1][1] = ZY+_InfectionRadius;
        while (ZX < X) {
            _XZones += 1;
            p[0][0] = ZX; p[0][1] = ZX+_InfectionRadius;
            Zone *z = new Zone(id, p, this);
            ZX += _InfectionRadius;
            id++;
            addZone(z);
        }
        ZX = 0;
        ZY += _InfectionRadius;
    }
}
void Place::setZoneNeighborhood(){
    for (auto z=_Zones.cbegin(); z != _Zones.cend(); ++z) {
        (*z)->setNeighbors();
    }

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
list<Zone*>* Place::getZones(){
    return &_Zones;
}
list<Person*> Place::getOccupants(){
	return _Occupants;
}
int Place::getXZones(){
    return _XZones;
}
int Place::getYZones(){
    return _YZones;
}


//Place::~Place(){
    //delete _Occupants;
//}
