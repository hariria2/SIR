//
//  Zone.cpp
//  SIR
//
//  Created by Sahand Hariri on 1/10/16.
//  Copyright © 2016 Sahand Hariri. All rights reserved.
//

#include "Zone.h"

using namespace std;

Zone::Zone(int id, double perimeter[2][2], Place* p):_Location(p)
{
    _ID = id;
    setPerimeter(perimeter);
};

void Zone::setID(int id){
    _ID = id;
}
void Zone::setPerimeter(double perimeter[2][2]){
    for (int ii=0; ii<2; ii++){
        for (int jj=0; jj<2; jj++){
            Perimeter[ii][jj] = perimeter[ii][jj];
        }
    }
}
void Zone::setOccupants(list<Person *> p){
    _Occupants = p;
}
void Zone::setNeighborZones(list<Zone *> z){
    _NeighborZones = z;
}
void Zone::setLocation(Place *p){
    _Location = p;
}
void Zone::setNeighbors(){
    int xn = _Location->getXZones();
    int yn = _Location->getYZones();
    
    int gx = getID()%xn;
    int gy = getID()%yn;
    
    int nbrOffset[3] = {-1,0,1};
    int nbrIDx[3];
    int nbrIDy[3];
    
    bool existsx, existsy;
    
    for (int ii=0; ii<3; ii++){
        for (int jj=0; jj<3; jj++){
            nbrIDy[jj] = gy + nbrOffset[jj];
        }
        nbrIDx[ii] = gx + nbrOffset[ii];
    }
    
    if ((_Location->getZones())->size() > 0){
        for (auto z=((_Location->getZones())->cbegin()); z != ((_Location->getZones())->cend()); ++z){
            
            existsx =find(begin(nbrIDx), end(nbrIDx), (*z)->getID()%xn) != end(nbrIDx);
            existsy =find(begin(nbrIDy), end(nbrIDy), (*z)->getID()%yn) != end(nbrIDy);
            if(existsx & existsy){
                addNeighbor(*z);
            }
        }
    }
}

list<Person*> Zone::getOccupants(){
    return _Occupants;
}
list<Zone*> Zone::getNeighborZones(){
    return _NeighborZones;
}
Place* Zone::getLocation(){
    return _Location;
}
int Zone::getID(){
    return _ID;
}

void Zone::addPerson(Person *p){
    _Occupants.push_back(p);
}
void Zone::removePerson(Person* p){
    _Occupants.remove(p);
}
void Zone::addNeighbor(Zone *z){
    _NeighborZones.push_back(z);
}
