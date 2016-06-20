/*
 * Person.h
 *
 *  Created on: Jan 6, 2015
 *      Author: Sahand
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <iostream>
#include <random>

#include "Place.h"
#include "InHostDynamics.h"

using namespace std;

class Place;

class Person {
public:
	bool IsSingleLocation;
	
	Person(int id, string name, double age,char state, InHostDynamics ihd, Domain* city, Place* Location, vector<Place*> availplaces, int inf_var, int inc_var, int rec_var);
	
	Person(int id, string name, double age, char state, InHostDynamics ihd, Domain* city,vector<Place*> availplaces, int inf_var, int inc_var, int rec_var, bool IsSingleLocation);
	
	~Person();
	// Setters
	void setID(int id);
	void setAge(double a);
	void setAgeIncrement(double ai);
	void setX(int x);
	void setY(int y);
	void setHasBeenSick(int hbs);
	void setGender(char g);
	void setName(string name);
	void setCoordinates(double coordinates[2]);
	void setState(char state);
	void setMotionStepSize(double ss);
	void setLocation(Place* location);
	void setDefaultLocation(Place* location);
	void setAvailPlaces(vector<Place*> availplaces);
	void setTime(double t);
	void setInfectionPeriod();
	void setIncubationPeriod();
	void setRecoveryPeriod();
	void setLifeExpectancy(int le);
	void setInHostDynamics(InHostDynamics ihd);
	void setInfVar(int var);
	void setIncVar(int var);
	void setRecVar(int var);
	void setTravelerQ(bool tq);
	void setNeighbors(list<Person*>* n);
	
	
	// Getters
	int getID();
	double getAge();
	double getMotionStepSize();
	char getGender();
	int getHasBeenSick();
	string getName() const;
	double* getCoordinates();
	char getState() const;
	bool getNewInf();
	Domain* getDomain();
	Place* getLocation();
	Place* getDeafaultLocation();
	vector<Place*> getAvailablePlaces();
	list<Person*>* getNeighbors();
	double getTime();
	double getTimeOfDeath();
	InHostDynamics getInHostDynamics() const;
	int getInfectionPeriod();
	int getIncubationPeriod();
	int getRecoveryPeriod();
	int getInfVar();
	int getIncVar();
	int getRecVar();
	int getLifeExpectancy();
	bool getTraverlerQ();
	string getConnections();
	
	void clearConnections();
	
	void Update();
	
	// Utilities
	double Distance(Person* p);
	//void Move(double theta, double r, string type = "DailyMovement", double demand = 0);
	void Move(double theta, double r, string type = "IslandHopper");
	void UpdateDiseaseWithInHost();
	void Die();
	
	
	
	
	void addSIConnection(int id);
	void addSIConnectionHist(int id);
	void addAllConnection(int id);
	void addAllConnectionHist(int id);
	
	bool operator==(const Person& p) const;
	bool operator=(Person &A) const;
	
private:
	
	list<Person*>* _Neighbors;
	vector<Place*> _AvailablePlaces;
	Domain* _City;
	Place* _Location;
	Place* _DefaultLocation;
	InHostDynamics _ihdynamics;
	
	
	unsigned _RandSeed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine *_generator;
	
	int _ID;
	double _Age;
	double _AgeIncrement;
	string _Name;
	double _Time;
	double _X;
	double _Y;
	double _TimeInfected;
	double _IncubationTime;
	double _RecoveryTime;
	double _TimeOfDeath;
	double _Coordinates[2];
	double _MotionStepSize;
	bool _TravelerQ = false;
	
	
	char _State;
	bool _NewInf=false;
	char _Gender;
	int _HasBeenSick = 0;
	
	
	int _InfectionPeriod;
	int _IncubationPeriod;
	int _RecoveryPeriod;
	int _InfectionVar;
	int _IncubationVar;
	int _RecoveryVar;
	int _LifeExpectancy;
	string _Connections = "";
};

#endif /* PERSON_H_ */
