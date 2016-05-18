/*
 * Person.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: Sahand
 */


#include "Person.h"

Person::Person(int id, string name, double age,
               char state, InHostDynamics ihd,
               Domain* city, Place* location, vector<Place*> availplaces,
               int inf_var, int inc_var, int rec_var):
    _ihdynamics(ihd),
    _City(city),
    _Location(location),
    _AvailablePlaces(availplaces)
{
	_generator = new default_random_engine(_RandSeed);
    setID(id);
    setAge(age);
	setName(name);
	setState(state);
	_Time = 0;
	_TimeInfected = 0;
    setInfVar(inf_var);
    setIncVar(inc_var);
    setRecVar(rec_var);
	setInfectionPeriod();
    setIncubationPeriod();
    setRecoveryPeriod();
	IsSingleLocation = false;
    setGender('M');
    setLocation(location);
    setDefaultLocation(location);
    uniform_real_distribution<double> LE(77,82);
    setLifeExpectancy(LE(*_generator));
    
}// end constructor


Person::Person(int id, string name, double age,
               char state, InHostDynamics ihd,
               Domain* city,vector<Place*> availplaces,
               int inf_var, int inc_var, int rec_var,
               bool isSingleLocation):
    _ihdynamics(ihd),
    _AvailablePlaces(availplaces)
{
    _generator = new default_random_engine(_RandSeed);
 	setID(id);
    setAge(age);
	setName(name);
	setState(state);
	_Time = 0;
	_TimeInfected = 0;
    setInfVar(inf_var);
    setIncVar(inc_var);
    setRecVar(rec_var);
	setInfectionPeriod();
    setIncubationPeriod();
    setRecoveryPeriod();
	IsSingleLocation = isSingleLocation;
    setLocation(availplaces[0]);
    setDefaultLocation(_Location);
    setGender('M');
    uniform_real_distribution<double> LE(77,82);
    setLifeExpectancy(LE(*_generator));
    }

// Setters
void Person::setID(int id){
	_ID = id;
}
void Person::setAge(double age){
    _Age = age;
}
void Person::setAgeIncrement(double ai){
    _AgeIncrement = ai;
}
void Person::setHasBeenSick(int hbs){
    _HasBeenSick = hbs;
}
void Person::setGender(char g){
    _Gender = g;
}
void Person::setName(string name){
	_Name = name;
}
void Person::setCoordinates(double coordinates[2]){
	for (int ii=0; ii<2; ii++){
		_Coordinates[ii] = coordinates[ii];
	}
}
void Person::setState(char state){
	_State = state;
}
void Person::setLocation(Place* location){
    if (IsSingleLocation==false){
        _Location->removePerson(this);
    }
	_Location = location;
	_Location->addPerson(this);
    
    double xmin = location->Perimeter[0][0];
    double xmax = location->Perimeter[0][1];
    double ymin = location->Perimeter[1][0];
    double ymax = location->Perimeter[1][1];
    
    uniform_real_distribution<double> xdist(xmin, xmax);
    uniform_real_distribution<double> ydist(ymin, ymax);
    
    double x = xdist(*_generator);
    double y = ydist(*_generator);
    double Co[2] = {x,y};
    
    setCoordinates(Co);
    
}

void Person::setDefaultLocation(Place* location){
    _DefaultLocation = location;
}
void Person::setTime(double t){
	_Time = t;
}
void Person::setMotionStepSize(double ss){
    _MotionStepSize = ss;
}
void Person::setInfectionPeriod(){
    normal_distribution<double> distribution(_ihdynamics.getAverageInfectionPeriod(),_InfectionVar);
    
    double randnum  = distribution(*_generator);
	randnum = (randnum < 0)? 0:randnum;
	_InfectionPeriod = floor(randnum);
}
void Person::setIncubationPeriod(){
	normal_distribution<double> distribution(_ihdynamics.getAverageIncubationPeriod(),_IncubationVar);
	double randnum  = distribution(*_generator);
	randnum = (randnum < 0)? 0:randnum;
	_IncubationPeriod = floor(randnum);
}
void Person::setRecoveryPeriod(){
    normal_distribution<double> distribution(_ihdynamics.getAverageRecoveryPeriod(),_RecoveryVar);
    double randnum  = distribution(*_generator);
    randnum = (randnum < 0)? 0:randnum;
    _RecoveryPeriod = floor(randnum);
}
void Person::setLifeExpectancy(int le){
    _LifeExpectancy = le;
}
void Person::setInHostDynamics(InHostDynamics ihd){
    _ihdynamics = ihd;
}
void Person::setInfVar(int var){
    _InfectionVar = var;
};
void Person::setIncVar(int var){
    _IncubationVar = var;
};
void Person::setRecVar(int var){
    _RecoveryVar = var;
};
void Person::setTravelerQ(bool tq){
    _TravelerQ = tq;
};
void Person::setNeighbors(list<Person *> *n){
    _Neighbors = n;
}

// Getters
int Person::getID(){
	return _ID;
}
double Person::getAge(){
    return _Age;
}
double Person::getMotionStepSize(){
    return _MotionStepSize;
}
int Person::getHastBeenSick(){
    return _HasBeenSick;
}
char Person::getGender(){
    return _Gender;
}
int Person::getInfectionPeriod(){
    return _InfectionPeriod;
}
int Person::getIncubationPeriod(){
    return _IncubationPeriod;
}
int Person::getRecoveryPeriod(){
    return _RecoveryPeriod;
}
int Person::getInfVar(){
    return _InfectionVar;
}
int Person::getIncVar(){
    return _IncubationVar;
}
int Person::getRecVar(){
    return _RecoveryVar;
}
int Person::getLifeExpectancy(){
    return _LifeExpectancy;
}
Domain* Person::getDomain(){
    return _City;
}
double* Person::getCoordinates(){
    return _Coordinates;
}
double Person::getTime(){
	return _Time;
}
double Person::getTimeOfDeath(){
    return _TimeOfDeath;
}
char Person::getState() const{
    return _State;
}
bool Person::getNewInf(){
    return _NewInf;
}
string Person::getName() const{
    return _Name;
}
Place* Person::getLocation(){
    return _Location;
}
Place* Person::getDeafaultLocation(){
    return _DefaultLocation;
}
vector<Place*> Person::getAvailablePlaces(){
    return _AvailablePlaces;
}
list<Person*>* Person::getNeighbors(){
    return _Neighbors;
}
InHostDynamics Person::getInHostDynamics() const{
    return _ihdynamics;
}
bool Person::getTraverlerQ(){
    return _TravelerQ;
}
string Person::getConnections(){
    return _Connections;
}

void Person::clearConnections(){
    _Connections = "";
}

void Person::Update(){
    if (_State == 'P' | _State == 'I'){
        Move((rand() % 360),_MotionStepSize, "IslandHopper");
    }else{
        Move((rand() % 360),4*_MotionStepSize, "IslandHopper");
    }
    
    if (_State != 'D'){
        UpdateDiseaseWithInHost();
    }
    
    _Age += _AgeIncrement;
    if ((_Age >= 10) & (_Age - 0.1 < 15)){
        if (getState() == 'N'){
            setState('S');
            _ihdynamics.setT(3);
        }
    }
    if (_State != 'D') {
        if (_Age >= _LifeExpectancy){
            Die();
        }
    }
}

// Utilities
void Person::Move(double theta, double r, string motionType){
	//int hour    = floor(_Time);
	//double min  = _Time - hour;
	//double DailyTime = ((hour % 24) + min);
    
    if (_State == 'D') {
        if (_Location->getType()=="Cemetery"){
            return;
        }
        for (auto L = _AvailablePlaces.cbegin(); L != _AvailablePlaces.cend(); L++){
            if ((*L)->getType()=="Cemetery") {
                setLocation(*L);
                return;
            }
        }
    }
    
    
    
    if (_TravelerQ){
       /* int lid = rand() % (_AvailablePlaces.size()-2) + 1;
        for (auto L = _AvailablePlaces.cbegin(); L != _AvailablePlaces.cend(); L++){
            if (((*L)->getID()==lid) & (*L)->getName() != "Cemetery"){
                setLocation(*L);
            }
        }*/
        
    }
    
	double x = _Coordinates[0] + r*cos(theta);
	double y = _Coordinates[1] + r*sin(theta);
	
    double xmin = _Location->Perimeter[0][0];
	double xmax = _Location->Perimeter[0][1];
	double ymin = _Location->Perimeter[1][0];
	double ymax = _Location->Perimeter[1][1];

     
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

	_Coordinates[0] = x;
	_Coordinates[1] = y;

}
void Person::UpdateDiseaseWithInHost(){
    
    //list<Person*>* peeps = _Location->getOccupants();
    
    double criticalDistance = 38;
    
    /*
    for(auto ip = peeps->cbegin(); ip != peeps->cend(); ++ip){
        
        if (Distance(*ip) < criticalDistance){
            if (getID() != ((*ip)->getID())){
                _Neighbors->push_back(*ip);
            }
        }
    }
    */
    
    double totalVirion = 0;
    
    double dist;
    
    
    for(auto ip = _Neighbors->cbegin(); ip != _Neighbors->cend(); ++ip){
        dist = Distance(*ip);
        if (dist != 0 & dist < criticalDistance){
            totalVirion += ((*ip)->_ihdynamics.getV())/dist;
            //_Connections.append("," + to_string((*ip)->getID()));
        }
    }
    
    _ihdynamics.setT0(_Time);
    _ihdynamics.setNE(0.01*totalVirion);
    _ihdynamics.Simulate();
    
    if ((getState()=='R') & !(getHastBeenSick())) {
        if (_ihdynamics.getT() > 1.8){
            setState('S');
        }
    }
    else if (getState() == 'S'){
        if (_ihdynamics.getV() > 0.2){
            setState('I');
            _NewInf = true;
        }
    }
    else if (getState() == 'I'){
        _NewInf = false;
        if (_ihdynamics.getI() > 0.3 & _ihdynamics.getI() < 3 & _HasBeenSick==0){
            setState('P');
            setHasBeenSick(1);
            _ihdynamics.HasBeenSick = 1;
        }else if (_ihdynamics.getI() > 2.6){
            Die();
        }
        else if (_ihdynamics.getI() < 0.1 & _HasBeenSick == 1){
            setState('R');
            setHasBeenSick(0);
            _ihdynamics.HasBeenSick = 1;
        }
    }
    else if (getState() == 'P'){
        _NewInf = false;
        if (_ihdynamics.getI() + 0.01 < _ihdynamics.getMaxInfLev()){
            setHasBeenSick(1);
            _ihdynamics.HasBeenSick = 1;
        }
        if (_ihdynamics.getI() > 2.6){
            Die();
        }
        else if (_ihdynamics.getI() < 0.2){
            setState('I');
        }
    }
    else if (getState() == 'R'){
        _NewInf = false;
        if (_ihdynamics.getI() < 0.1){
            setHasBeenSick(0);
            _ihdynamics.HasBeenSick = 0;
        }
    }

    //_neigbors.clear();
}
double Person::Distance(Person* p){
    
    double p1x = _Coordinates[0];
    double p1y = _Coordinates[1];
    
    double p2x = (p->getCoordinates())[0];
    double p2y = (p->getCoordinates())[1];
    
    return (pow((p2x-p1x),2) + pow((p2y - p1y),2));
}
void Person::Die(){
    setState('D');
    _TimeOfDeath = _Time;
}

bool Person::operator == (const Person& p) const {
	return (p._ID == this->_ID);
}
Person::~Person(){
    delete _generator;
}
