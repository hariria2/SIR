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

Person::Person(int id, string name, int age,
               char state, Disease dis,
               Domain* city,    Place* home,
               Place* school,   Place* work,
               Place* cemetery, Place* location,
               double homeco[2],  double workco[2],
               double schoolco[2],double cemeteryco[2],
               int inf_var, int inc_var, int rec_var)
		:disease(dis), City(city),
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
}// end constructor


Person::Person(int id, string name, int age,
               char state, Disease dis,
               Domain* city, Place* home,
               double homeco[2],
               int inf_var, int inc_var, int rec_var,
               bool isSingleLocation)
		:disease(dis), City(city), Home(home), Location(home)
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
}

// Setters
void Person::setID(int id){
	ID = id;
}
void Person::setAge(int age){
    Age = age;
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

// Utilities
void Person::Move(double theta, double r) {
	int hour    = floor(Time);
	double min  = Time - hour;
	double DailyTime = ((hour % 24) + min);

    if (getState() == 'D') {
        setLocation(Cemetery);
        return;
    }
    
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
	double x = Coordinates[0] + r*cos(theta);
	double y = Coordinates[1] + r*sin(theta);
	double xmin = Location->Perimeter[0][0];
	double xmax = Location->Perimeter[0][1];
	double ymin = Location->Perimeter[1][0];
	double ymax = Location->Perimeter[1][1];

	if (x < xmin){
		x = xmin;
	}
	else if (x > xmax){
		x = xmax;
	}

	if (y <  ymin){
		y = ymin;
	}
	else if (y > ymax){
		y = ymax;
	}

	Coordinates[0] = x;
	Coordinates[1] = y;
}
void Person::Move2(double theta, double r){
    
	double x = Coordinates[0] + r*cos(theta);
	double y = Coordinates[1] + r*sin(theta);
	double xmin = Location->Perimeter[0][0];
	double xmax = Location->Perimeter[0][1];
	double ymin = Location->Perimeter[1][0];
	double ymax = Location->Perimeter[1][1];

	if (x < xmin){
		x = xmin;
	}
	else if (x > xmax){
		x = xmax;
	}

	if (y <  ymin){
		y = ymin;
	}
	else if (y > ymax){
		y = ymax;
	}

	Coordinates[0] = x;
	Coordinates[1] = y;
}
void Person::UpdateDisease() {
	list<Person*> peeps = Location->getOccupants();
	if (getState() == 'S'){
		for(auto ip = peeps.cbegin(); ip != peeps.cend(); ++ip){
			if ((*ip)->getState() == 'P' | (*ip)->getState() == 'I'){
				if (Distance(*ip) < 10){
					IncubationTime = Time;
					setDisease((*ip)->getDisease());
                    setState('I');
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
double Person::Distance(Person* p){
    
    double p1x = Coordinates[0];
    double p1y = Coordinates[1];
    
    double p2x = (p->getCoordinates())[0];
    double p2y = (p->getCoordinates())[1];
    
    return sqrt(pow((p2x-p1x),2) + pow((p2y - p1y),2));
}
bool Person::operator == (const Person& p) const {
	return (p.ID == this->ID);
}

Person::~Person(){

}
