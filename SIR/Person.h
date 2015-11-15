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
#include "Disease.h"
#include "InHostDynamics.h"

using namespace std;

class Person {      	
public:
	bool IsSingleLocation;

	Person(int id, string name, int age,
           char state, Disease dis, InHostDynamics ihd,
           Domain* city, Place* Location, vector<Place*> availplaces,
           int inf_var, int inc_var, int rec_var);

	Person(int id, string name, int age,
           char state, Disease dis, InHostDynamics ihd,
           Domain* city,vector<Place*> availplaces,
           int inf_var, int inc_var, int rec_var,
           bool IsSingleLocation);

	~Person();
	// Setters
	void setID(int id);
    void setAge(int id);
    void setX(int x);
    void setY(int y);
    void setHasBeenSick(int hbs);
    void setGender(char g);
	void setName(string name);
	void setCoordinates(double coordinates[2]);
    void setState(char state);
	void setLocation(Place* location);
    void setDefaultLocation(Place* location);
    void setAvailPlaces(vector<Place*> availplaces);
	void setTime(double t);
	void setInfectionPeriod();
	void setIncubationPeriod();
    void setRecoveryPeriod();
	void setDisease(Disease d);
    void setInHostDynamics(InHostDynamics ihd);
    void setInfVar(int var);
    void setIncVar(int var);
    void setRecVar(int var);

	// Getters
	int getID();
    int getAge();

    char getGender();
    int getHastBeenSick();
	string getName() const;
	double* getCoordinates();
    char getState() const;
    Domain* getDomain();
	Place* getLocation();
    Place* getDeafaultLocation();
	double getTime();
	Disease getDisease() const;
    InHostDynamics getInHostDynamics() const;
	int getInfectionPeriod();
	int getIncubationPeriod();
    int getRecoveryPeriod();
    int getInfVar();
    int getIncVar();
    int getRecVar();
    list<int> getSIConnections();
    list<int> getSIConnectionsHist();
    list<int> getAllConnections();
    list<int> getAllConnectionsHist();

    
    void Update();
    
	// Utilities
	double Distance(Person* p);
	//void Move(double theta, double r, string type = "DailyMovement", double demand = 0);
    void Move(double theta, double r, string type = "DailyMovement");
	void Move2(double theta, double r);
	void ContractDisease(Disease d);
	void UpdateDisease();
    void UpdateDiseaseWithInHost();
    
    
	bool operator == (const Person& p) const;
    
    void addSIConnection(int id);
    void addSIConnectionHist(int id);
    void addAllConnection(int id);
    void addAllConnectionHist(int id);
    


private:
	int _ID;
    int _Age;
	string _Name;
	double _Time;
    double _X;
    double _Y;
	double _TimeInfected;
	double _IncubationTime;
    double _RecoveryTime;
    double _TimeOfDeath;
	double _Coordinates[2];
    
	Disease _disease;
    
    InHostDynamics _ihdynamics;
    
    vector<Person*> _neigbors;
    
    unsigned _RandSeed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	char _State;
    char _Gender;
    int _HasBeenSick = 0;
	Domain* _City;
	Place* _Location;
    Place* _DefaultLocation;
    vector<Place*> _AvailablePlaces;
	int _InfectionPeriod;
	int _IncubationPeriod;
    int _RecoveryPeriod;
    int _InfectionVar;
    int _IncubationVar;
    int _RecoveryVar;
    list<int> _SIConnections;
    list<int> _SIConnectionsHist;
    list<int> _AllConnections;
    list<int> _AllConnectionsHist;
};

#endif /* PERSON_H_ */
