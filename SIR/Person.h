#ifndef PERSON_H_
#define PERSON_H_

#include <iostream>
#include <random>
#include <map>

#include "Place.h"
#include "InHostDynamics.h"

using namespace std;

class Place;


/**
 * Person.h
 *
 * \date   Jan 6, 2015
 * \author Sahand
 *
 * # Person
 * + This class descripes all the properties and behaviors of each agent.
 *
 * \todo Fix up the constructors. 
 *	+ No more need for SingleLocation (I think).
 *	+ No more need for the last three parameters.
 */

class Person {

public:

	bool IsSingleLocation;
	/// This is one of the constructors for the class.
	Person(int id, string name, double age,char state, InHostDynamics ihd, Domain* city, Place* Location, vector<Place*> availplaces);
	
	~Person();
	// Setters
	void setID(int id);
	void setX(double x);
	void setY(double y);
	void setAge(double a);
	void setAgeIncrement(double ai);
	void setTimeStep(double ts);
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
	void setLifeExpectancy(double le);
	void setInHostDynamics(InHostDynamics ihd);
	void setInfVar(int var);
	void setIncVar(int var);
	void setRecVar(int var);
	void setTravelerQ(bool tq);
	void setNeighbors(list<Person*>* n);
	void setAgeInteraction();
	void setAgeGroup();
	void setSociability(double s);
	
	
	// Getters
	int getID();
	double getX();
	double getY();
	double getTimeStep();
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
	double getLifeExpectancy();
	bool getTraverlerQ();
	string getConnections();
	char getAgeGroup();
	double getSociability();
	
	void clearConnections();
	
	void Update();
	
	// Utilities
	double Distance(Person* p);
	/// This function returns distances in  x and y directions. 
	void CartesianDistance(Person* p, double* r);
	//void Move(double theta, double r, string type = "DailyMovement", double demand = 0);
	void computeMotionEffect(double* distVector, char ag, double* r);
	void InteractWithOthers();
	/// Tell people how to move.
	void Move();
	void Move(double x, double y, string type = "IslandHopper");
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

	/// Unique ID ascribed to each person when it is created.
	int _ID;
	double _Age;
	double _AgeIncrement;
	double _LifeExpectancy;
	char _AgeGroup;
	string _Name;
	double _Time;
	double _TimeStep;
	double _X;
	double _Y;
	/// The value by which the person moves in x direction becasue of interaction with others
	double _MotionBiasX;
	/// The value by which the person moves in y direction becasue of interaction with others
	double _MotionBiasY;
	/// The attractive/repulsive force exerted on th eperson from others in x direction.
	double _AttractionForceX;
	/// The attractive/repulsive force exerted on th eperson from others in x direction.
	double _AttractionForceY;
	/// Amount of virions collected form interacting with all the other people.
	double _TotalVirion;
	double _TimeInfected;
	double _IncubationTime;
	double _RecoveryTime;
	double _TimeOfDeath;
	double _Coordinates[2];
	/// The magnitude of motion in the random direction before the bias is added.
	double _MotionStepSize;
	/// Multiplied the motion bias depicting how social/anti-social a person is.
	double _Sociability;
	/// This determines if a person can travel across islands. 
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

	string _Connections = "";

	/// The level of interaction of two individuals based on their age groups.
	map<string,double> _AgeInteraction; /**< Detailed description. */

};

#endif /* PERSON_H_ */
