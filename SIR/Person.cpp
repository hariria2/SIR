/*
 * Person.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: Sahand
 */
#include <iostream>
#include <string>
#include <cmath>
#include <list>
#include <vector>
#include <random>
#include <ctime>
#include "Domain.h"
#include "Place.h"
#include "Disease.h"
#include "Person.h"
#include "InHostDynamics.h"

Person::Person(int id, string name, int age,
               char state, Disease dis, InHostDynamics ihd,
               Domain* city,    Place* home,
               Place* school,   Place* work,
               Place* cemetery, Place* location,
               double homeco[2],  double workco[2],
               double schoolco[2],double cemeteryco[2],
               int inf_var, int inc_var, int rec_var)
		:disease(dis),ihdynamics(ihd), City(city),
         Home(home), School(school),
         Work(work), Cemetery(cemetery), Location(location)
{
	setID(id);
    setAge(age);
	setName(name);
	setState(state);
	Time = 0;
	TimeInfected = 0;
    setInfVar(inf_var);
    setIncVar(inc_var);
    setRecVar(rec_var);
	setInfectionPeriod();
    setIncubationPeriod();
    setRecoveryPeriod();
    setHomeCoordinates(homeco);
    setWorkCoordinates(workco);
    setSchoolCoordinates(schoolco);
    setCemeteryCoordinates(cemeteryco);
	IsSingleLocation = false;
    setGender('M');
}// end constructor


Person::Person(int id, string name, int age,
               char state, Disease dis, InHostDynamics ihd,
               Domain* city, Place* home,
               double homeco[2],
               int inf_var, int inc_var, int rec_var,
               bool isSingleLocation)
		:disease(dis), ihdynamics(ihd), City(city), Home(home), Location(home)
{
 	setID(id);
    setAge(age);
	setName(name);
	setState(state);
	Time = 0;
	TimeInfected = 0;
    setInfVar(inf_var);
    setIncVar(inc_var);
    setRecVar(rec_var);
	setInfectionPeriod();
    setIncubationPeriod();
    setRecoveryPeriod();
    setHomeCoordinates(homeco);
	setLocation(Location);
	IsSingleLocation = isSingleLocation;
    setGender('M');
}

// Setters
void Person::setID(int id){
	ID = id;
}
void Person::setAge(int age){
    Age = age;
}
void Person::setGender(char g){
    Gender = g;
}
void Person::setName(string name){
	Name = name;
}
void Person::setCoordinates(double coordinates[2]){
	for (int ii=0; ii<2; ii++){
		Coordinates[ii] = coordinates[ii];
	}
}
void Person::setHomeCoordinates(double homeco[2]){
    for (int ii=0; ii<2; ii++){
        HomeCoordinates[ii] = homeco[ii];
    }
}
void Person::setWorkCoordinates(double workco[2]){
    for (int ii=0; ii<2; ii++){
        WorkCoordinates[ii] = workco[ii];
    }
}
void Person::setSchoolCoordinates(double schoolco[2]){
    for (int ii=0; ii<2; ii++){
        SchoolCoordinates[ii] = schoolco[ii];
    }
}
void Person::setCemeteryCoordinates(double cemeteryco[2]){
    for (int ii=0; ii<2; ii++){
        CemeteryCoordinates[ii] = cemeteryco[ii];
    }
}
void Person::setState(char state){
	State = state;
}
void Person::setLocation(Place* location){
	Location->removePerson(this);
	Location = location;
	Location->addPerson(this);
    
    if (Location == Home){
        setCoordinates(HomeCoordinates);
    }else if (Location == Work){
        setCoordinates(WorkCoordinates);
    }else if (Location == School){
        setCoordinates(SchoolCoordinates);
    }else // Location == Cemetery
    {
        setCoordinates(CemeteryCoordinates);
    }
}
void Person::setTime(double t){
	Time = t;
}
void Person::setInfectionPeriod(){
	unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	normal_distribution<double> distribution(disease.getAverageInfectionPeriod(),InfectionVar);
	double randnum  = distribution(generator);
	randnum = (randnum < 0)? 0:randnum;
	InfectionPeriod = floor(randnum);
}
void Person::setIncubationPeriod(){
	unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	normal_distribution<double> distribution(disease.getAverageIncubationPeriod(),IncubationVar);
	double randnum  = distribution(generator);
	randnum = (randnum < 0)? 0:randnum;
	IncubationPeriod = floor(randnum);
}
void Person::setRecoveryPeriod(){
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<double> distribution(disease.getAverageRecoveryPeriod(),RecoveryVar);
    double randnum  = distribution(generator);
    randnum = (randnum < 0)? 0:randnum;
    RecoveryPeriod = floor(randnum);
}
void Person::setDisease(Disease d){
	disease = d;
}
void Person::setInHostDynamics(InHostDynamics ihd){
    ihdynamics = ihd;
}
void Person::setInfVar(int var){
    InfectionVar = var;
};
void Person::setIncVar(int var){
    IncubationVar = var;
};
void Person::setRecVar(int var){
    RecoveryVar = var;
};

// Getters
int Person::getID(){
	return ID;
}
int Person::getAge(){
    return Age;
}
char Person::getGender(){
    return Gender;
}
int Person::getInfectionPeriod(){
    return InfectionPeriod;
}
int Person::getIncubationPeriod(){
    return IncubationPeriod;
}
int Person::getRecoveryPeriod(){
    return RecoveryPeriod;
}
int Person::getInfVar(){
    return InfectionVar;
}
int Person::getIncVar(){
    return IncubationVar;
}
int Person::getRecVar(){
    return RecoveryVar;
}
Domain* Person::getDomain(){
    return City;
}
Place* Person::getHome(){
    return Home;
}
double* Person::getCoordinates(){
	 return Coordinates;
}
double* Person::getHomeCoordinates(){
    return HomeCoordinates;
}
double* Person::getWorkCoordinates(){
    return WorkCoordinates;
}
double* Person::getSchoolCoordinates(){
    return SchoolCoordinates;
}
double* Person::getCemeteryCoordinates(){
    return CemeteryCoordinates;
}
double Person::getTime(){
	return Time;
}
char Person::getState() const{
    return State;
}
string Person::getName() const{
    return Name;
}
Place* Person::getLocation(){
    return Location;
}
Disease Person::getDisease() const{
    return disease;
}
InHostDynamics Person::getInHostDynamics() const{
    return ihdynamics;
}
list<int> Person::getSIConnections(){
    return SIConnections;
}
list<int> Person::getSIConnectionsHist(){
    return SIConnectionsHist;
}
list<int> Person::getAllConnections(){
    return AllConnections;
}
list<int> Person::getAllConnectionsHist(){
    return AllConnectionsHist;
}

// Utilities
void Person::Move(double theta, double r, string motionType) {
	int hour    = floor(Time);
	double min  = Time - hour;
	double DailyTime = ((hour % 24) + min);

    if (getState() == 'D') {
        setLocation(Cemetery);
        return;
    }
    
    if (motionType == "DailyMovement"){
        
        if (DailyTime <= 8 || DailyTime >= 18){
            if (!(Location->getType() == "Home")){
                setLocation(Home);
            }
        }
        else{
            
            if (Location->getType() == "Home" && getState() != 'P'){
                (Age>22)? setLocation(Work):setLocation(School);
            }
        }
        
    }else if (motionType == "Travel"){
        if (getID() == 1){
            if (Time == 10){
                setLocation(Work);
            }
            if (Time == 20){
                setLocation(School);
            }
        }
//        if (getID() == 2){
//            if (Time == 10){
//                setLocation(Work);
//            }
//            if (Time == 20){
//                setLocation(School);
//            }
//        }
        if (getID() == 9){
            if (Time == 30){
                setLocation(Home);
            }
        }
//        if (getID() == 3){
//            if (Time == 25){
//                setLocation(Home);
//            }
//        }
//        if (getID() == 202){
//            if (Time == 50){
//                setLocation(School);
//            }
//        }
//        if (getID() == 631){
//            if (Time == 50){
//                setLocation(School);
//            }
//        }
    }
    
    
	double x = Coordinates[0] + r*cos(theta);
	double y = Coordinates[1] + r*sin(theta);
	double xmin = Location->Perimeter[0][0];
	double xmax = Location->Perimeter[0][1];
	double ymin = Location->Perimeter[1][0];
	double ymax = Location->Perimeter[1][1];

    if (x < xmin){
        x = xmin;// + abs(x);
    }
    else if (x > xmax){
        x = xmax;// - abs(x-xmax);
    }
    
    if (y <  ymin){
        y = ymin;// + abs(y);
    }
    else if (y > ymax){
        y = ymax;// - abs(y-ymax);
    }

	Coordinates[0] = x;
	Coordinates[1] = y;
}
void Person::Move2(double theta, double r){
    
    if (getState() == 'D') {
        return;
    }
    
	double x = Coordinates[0] + r*cos(theta);
	double y = Coordinates[1] + r*sin(theta);
	double xmin = Location->Perimeter[0][0];
	double xmax = Location->Perimeter[0][1];
	double ymin = Location->Perimeter[1][0];
	double ymax = Location->Perimeter[1][1];

	if (x < xmin){
		x = xmin + abs(x);
	}
	else if (x > xmax){
		x = xmax - abs(x-xmax);
	}

	if (y <  ymin){
		y = ymin + abs(y);
	}
	else if (y > ymax){
		y = ymax - abs(y-ymax);
	}

	Coordinates[0] = x;
	Coordinates[1] = y;
}
void Person::UpdateDisease() {
    
    // Note that SIConnectionsHist does not get cleared here.
    SIConnections.clear();
    // Note that AllConnectionsHist does not get cleared here.
    AllConnections.clear();
    
	
    
    list<Person*> peeps = Location->getOccupants();
    int criticalDistance = 23;
    
    for(auto ip = peeps.cbegin(); ip != peeps.cend(); ++ip){
        if (Distance(*ip) < criticalDistance){
            neigbors.push_back(*ip);
            addAllConnection((*ip)->getID());
            addAllConnectionHist((*ip)->getID());
        }
    }
    
	if (getState() == 'S'){
		for(auto ip = peeps.cbegin(); ip != peeps.cend(); ++ip){
			if ((*ip)->getState() == 'P' | (*ip)->getState() == 'I'){
				if (Distance(*ip) < criticalDistance){
					IncubationTime = Time;
					setDisease((*ip)->getDisease());
                    setState('I');
                    addSIConnection((*ip)->getID());
                    addSIConnectionHist((*ip)->getID());
				}
			}
		}
    }else if (getState() == 'I'){
        double incubationPeriod = Time - IncubationTime;

        if (incubationPeriod >= IncubationPeriod){
           	TimeInfected = Time;
            setState('P');
        }
    }else if (getState()=='P'){
		double infectedTime = Time - TimeInfected;
		if (infectedTime >= InfectionPeriod & infectedTime <= RecoveryPeriod){
			setState('R');
            RecoveryTime = Time;
        }else if(infectedTime > RecoveryPeriod){
            setState('D');
            TimeOfDeath = Time;
        }
    //}else if(getState()=='R'){
     //   if (RecoveryTime >= 24){
      //      setState('S');
       // }
    }else{
        return;
    }
}
void Person::UpdateDiseaseWithInHost() {
    
    list<Person*> peeps = Location->getOccupants();
    int criticalDistance = 25;
    
    for(auto ip = peeps.cbegin(); ip != peeps.cend(); ++ip){
        if (Distance(*ip) < criticalDistance){
            if (getID() != ((*ip)->getID())){
                neigbors.push_back(*ip);
                addAllConnection((*ip)->getID());
                addAllConnectionHist((*ip)->getID());
            }
        }
    }
    double totalVirion = 0;
//    double pV;
    double dist;
    for(auto ip = neigbors.cbegin(); ip != neigbors.cend(); ++ip){
        dist = Distance(*ip);
        if (dist != 0){
            totalVirion += ((*ip)->ihdynamics.getV())/pow(dist,2);
        }
    }
    ihdynamics.setT0(Time);
//    ihdynamics.setV(pV+1000*totalVirion);
    ihdynamics.setNE(0.001*totalVirion);
    ihdynamics.Simulate();
    
    if (getState() == 'S'){
        if (ihdynamics.getV() > 0.01){
            setState('I');
        }
    }
    
    if (getState() == 'I'){
        if (ihdynamics.getI() > 0.3){
            setState('P');
        }else if (ihdynamics.getV() < 0.01){
            setState('R');
        }
    }else if (getState() == 'P'){
        if (ihdynamics.getI() < 0.3){
            setState('I');
        }
    }
    
    
    
}
double Person::Distance(Person* p){
    
    double p1x = Coordinates[0];
    double p1y = Coordinates[1];
    
    double p2x = (p->getCoordinates())[0];
    double p2y = (p->getCoordinates())[1];
    
    return sqrt(pow((p2x-p1x),2) + pow((p2y - p1y),2));
}
void Person::UpdateWithinHost(list<Person*> ngbrs){
    
    for(auto ip = ngbrs.cbegin(); ip != ngbrs.cend(); ++ip){
        
    }
    ihdynamics.setdt(Time);
    // everything gets set
    
    ihdynamics.Simulate();
    
}

bool Person::operator == (const Person& p) const {
	return (p.ID == this->ID);
}
void Person::addSIConnection(int id){
    SIConnections.push_back(id);
    SIConnections.sort();
    SIConnections.unique();
}
void Person::addSIConnectionHist(int id){
    SIConnectionsHist.push_back(id);
    SIConnectionsHist.sort();
    SIConnectionsHist.unique();
}
void Person::addAllConnection(int id){
    AllConnections.push_back(id);
    AllConnections.sort();
    AllConnections.unique();
}
void Person::addAllConnectionHist(int id){
    AllConnectionsHist.push_back(id);
    AllConnectionsHist.sort();
    AllConnectionsHist.unique();
}

Person::~Person(){

}
