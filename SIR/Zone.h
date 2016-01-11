//
//  Zone.hpp
//  SIR
//
//  Created by Sahand Hariri on 1/10/16.
//  Copyright © 2016 Sahand Hariri. All rights reserved.
//

#ifndef Zone_hpp
#define Zone_hpp

#include <stdio.h>
#include <list>
#include "Place.h"

using namespace std;

class Place;
class Person;

class Zone{
public:
    double Perimeter[2][2];
    
    Zone(int id, double perimeter[2][2], Place* p);
    
    void setID(int id);
    void setPerimeter(double perimeter[2][2]);
    void setOccupants(list<Person*> p);
    void setNeighborZones(list<Zone*> z);
    void setLocation(Place* p);
    void setNeighbors();
    
    int getID();
    list<Person*> getOccupants();
    list<Zone*> getNeighborZones();
    Place* getLocation();
    
    void addPerson(Person* p);
    void removePerson(Person* p);
    void addNeighbor(Zone* z);
    
    ~Zone();
    
private:
    int _ID;
    Place* _Location;
    list<Person*> _Occupants;
    list<Zone*> _NeighborZones;
    
};

#endif /* Zone_hpp */
