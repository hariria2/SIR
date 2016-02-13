/*
 * Person.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: Sahand
 */


#include "Person.h"

Person::Person(int id, string name, double age,
               char state, Disease dis, InHostDynamics ihd,
               Domain* city, Place* location, vector<Place*> availplaces,
               int inf_var, int inc_var, int rec_var):
    _disease(dis),
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
    initZone();
    
}// end constructor


Person::Person(int id, string name, double age,
               char state, Disease dis, InHostDynamics ihd,
               Domain* city,vector<Place*> availplaces,
               int inf_var, int inc_var, int rec_var,
               bool isSingleLocation):
    _disease(dis),
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
	setLocation(_Location);
	IsSingleLocation = isSingleLocation;
    setGender('M');
    uniform_real_distribution<double> LE(77,82);
    setLifeExpectancy(LE(*_generator));
    initZone();
    }

// Setters
void Person::setID(int id){
	_ID = id;
}
void Person::setAge(double age){
    _Age = age;
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
	_Location->removePerson(this);
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
void Person::initZone(){
    
    Zone* zone = ZoneFromCo(_Coordinates[0],_Coordinates[1]);
    _Zone = zone;
    _Zone->addPerson(this);
    
}
void Person::setZone(){

    if (_Zone->getOccupants().size() != 0){
        _Zone->removePerson(this);
    }
    Zone* zone = ZoneFromCo(_Coordinates[0],_Coordinates[1]);
    _Zone = zone;
    _Zone->addPerson(this);
    
}

void Person::setDefaultLocation(Place* location){
    _DefaultLocation = location;
}
void Person::setTime(double t){
	_Time = t;
}
void Person::setInfectionPeriod(){
    normal_distribution<double> distribution(_disease.getAverageInfectionPeriod(),_InfectionVar);
    
    double randnum  = distribution(*_generator);
	randnum = (randnum < 0)? 0:randnum;
	_InfectionPeriod = floor(randnum);
}
void Person::setIncubationPeriod(){
	normal_distribution<double> distribution(_disease.getAverageIncubationPeriod(),_IncubationVar);
	double randnum  = distribution(*_generator);
	randnum = (randnum < 0)? 0:randnum;
	_IncubationPeriod = floor(randnum);
}
void Person::setRecoveryPeriod(){
    normal_distribution<double> distribution(_disease.getAverageRecoveryPeriod(),_RecoveryVar);
    double randnum  = distribution(*_generator);
    randnum = (randnum < 0)? 0:randnum;
    _RecoveryPeriod = floor(randnum);
}
void Person::setLifeExpectancy(int le){
    _LifeExpectancy = le;
}
void Person::setDisease(Disease d){
	_disease = d;
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

// Getters
int Person::getID(){
	return _ID;
}
double Person::getAge(){
    return _Age;
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
Disease Person::getDisease() const{
    return _disease;
}
InHostDynamics Person::getInHostDynamics() const{
    return _ihdynamics;
}
list<int> Person::getSIConnections(){
    return _SIConnections;
}
list<int> Person::getSIConnectionsHist(){
    return _SIConnectionsHist;
}
list<int> Person::getAllConnections(){
    return _AllConnections;
}
list<int> Person::getAllConnectionsHist(){
    return _AllConnectionsHist;
}
bool Person::getTraverlerQ(){
    return _TravelerQ;
}

void Person::Update(){
    Move((rand() % 360),0.5, "IslandHopper");
    if (_State != 'D'){
        UpdateDiseaseWithInHost();
    }
    _Age += 0.0027/0.1;
    if ((_Age >= 10) & (_Age - 0.1 < 10)){
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
//void Person::Move(double theta, double r, string motionType, double demand)
void Person::Move(double theta, double r, string motionType){
	int hour    = floor(_Time);
	double min  = _Time - hour;
	double DailyTime = ((hour % 24) + min);

    if (getState() == 'D') {
        if (getLocation()->getType()=="Cemetery"){
            return;
        }
        for (auto L = _AvailablePlaces.cbegin(); L != _AvailablePlaces.cend(); L++){
            if ((*L)->getType()=="Cemetery") {
                setLocation(*L);
                return;
            }
        }
    }
    
    normal_distribution<double> WSTimeD(8,0.2);
    double  WSTime = WSTimeD(*_generator);
    normal_distribution<double> WETimeD(22,0.2);
    double  WETime = WETimeD(*_generator);
    
    
    int WSHour = floor(WSTime);
    double WSMin = WSTime - WSHour;
    double WSDailyTime = ((WSHour % 24)+WSMin);
    
    int WEHour = floor(WETime);;
    double WEMin = WETime - WEHour;;
    double WEDailyTime = ((WEHour % 24)+WEMin);
    
    
    
    if (motionType == "DailyMovement"){
        
        if (DailyTime <= WSDailyTime || DailyTime >= WEDailyTime){
            if (_Location->getType() != "Home"){
                setLocation(_DefaultLocation);
            }
        } else {
            if (_Location->getType() == "Home"){
                for (auto L = _AvailablePlaces.cbegin(); L != _AvailablePlaces.cend(); L++){
                    
                    if ((*L)->getType()=="School"){
                        if (_Age < 22) {
                            if (getState() != 'I' && getState()!='P'){
                                setLocation(*L);
                            }
                        }
                    }else if ((*L)->getType()=="Work"){
                        if (_Age >= 22) {
                            if (getState() != 'I' && getState()!='P'){
                                setLocation(*L);
                            }
                        }
                    }
                }
            }
        }
    }
    else if (motionType == "IslandHopper"){
        
        if (_Time != 0 & (fmod(_Time,10)) < 1e-3){
            if (_TravelerQ){
                int lid = rand() % (_AvailablePlaces.size()-2) + 1;
                for (auto L = _AvailablePlaces.cbegin(); L != _AvailablePlaces.cend(); L++){
                    if (((*L)->getID()==lid) & (*L)->getName() != "Cemetery"){
                        setLocation(*L);
                    }
                }

            }
        }
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
    setZone();
}
void Person::UpdateDisease() {
    
    // Note that SIConnectionsHist does not get cleared here.
    _SIConnections.clear();
    // Note that AllConnectionsHist does not get cleared here.
    _AllConnections.clear();
    
	
    
    list<Person*> peeps = _Location->getOccupants();
    int criticalDistance = 23;
    
    for(auto ip = peeps.cbegin(); ip != peeps.cend(); ++ip){
        if (Distance(*ip) < criticalDistance){
            _neigbors.push_back(*ip);
            addAllConnection((*ip)->getID());
            addAllConnectionHist((*ip)->getID());
        }
    }
    
	if (getState() == 'S'){
		for(auto ip = peeps.cbegin(); ip != peeps.cend(); ++ip){
			if ((*ip)->getState() == 'P' | (*ip)->getState() == 'I'){
				if (Distance(*ip) < criticalDistance){
					_IncubationTime = _Time;
					setDisease((*ip)->getDisease());
                    setState('I');
                    addSIConnection((*ip)->getID());
                    addSIConnectionHist((*ip)->getID());
				}
			}
		}
    }else if (getState() == 'I'){
        double incubationPeriod = _Time - _IncubationTime;

        if (incubationPeriod >= _IncubationPeriod){
           	_TimeInfected = _Time;
            setState('P');
        }
    }else if (getState()=='P'){
		double infectedTime = _Time - _TimeInfected;
		if (infectedTime >= _InfectionPeriod & infectedTime <= _RecoveryPeriod){
			setState('R');
            _RecoveryTime = _Time;
        }else if(infectedTime > _RecoveryPeriod){
            Die();
        }
    //}else if(getState()=='R'){
     //   if (RecoveryTime >= 24){
      //      setState('S');
       // }
    }else{
        return;
    }
}
void Person::UpdateDiseaseWithInHost(){
    
    list<Person*> peeps = _Location->getOccupants();
    
    /*list<Zone*> zn = _Zone->getNeighborZones();
    //list<Person*> peeps; // = _Zone->getOccupants();
    //list<Person*> np;
    for (auto z=zn.cbegin(); z != zn.cend(); ++z){

        np = (*z)->getOccupants();
        if (np.size() > 0){
            peeps.insert(peeps.cend(),np.cbegin(),np.cend());
        }
     }
    */
    
    double criticalDistance = 20;
    
    for(auto ip = peeps.cbegin(); ip != peeps.cend(); ++ip){
        
        if (getID() != ((*ip)->getID())){
            _neigbors.push_back(*ip);
            //addAllConnection((*ip)->getID());
            //addAllConnectionHist((*ip)->getID());
        }
        
        if (Distance(*ip) < criticalDistance){
            if (getID() != ((*ip)->getID())){
                _neigbors.push_back(*ip);
                //addAllConnection((*ip)->getID());
                //addAllConnectionHist((*ip)->getID());
            }
        }
    }
    double totalVirion = 0;

    
    double dist;
    
    for(auto ip = _neigbors.cbegin(); ip != _neigbors.cend(); ++ip){
        dist = Distance(*ip);
        if (dist != 0){
            totalVirion += ((*ip)->_ihdynamics.getV())/pow(dist,2);
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
        }else if (_ihdynamics.getI() > 2.9){
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
        if (_ihdynamics.getI() > 2.9){
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
//    if (_ihdynamics.getI() + 0.01 < _ihdynamics.getMaxInfLev()){
//        setHasBeenSick(1);
//        _ihdynamics.HasBeenSick = 1;
//    }

    _neigbors.clear();
}
double Person::Distance(Person* p){
    
    double p1x = _Coordinates[0];
    double p1y = _Coordinates[1];
    
    double p2x = (p->getCoordinates())[0];
    double p2y = (p->getCoordinates())[1];
    
    return sqrt(pow((p2x-p1x),2) + pow((p2y - p1y),2));
}
void Person::Die(){
    setState('D');
    _TimeOfDeath = _Time;
}

bool Person::operator == (const Person& p) const {
	return (p._ID == this->_ID);
}
void Person::addSIConnection(int id){
    _SIConnections.push_back(id);
    _SIConnections.sort();
    _SIConnections.unique();
}
void Person::addSIConnectionHist(int id){
    _SIConnectionsHist.push_back(id);
    _SIConnectionsHist.sort();
    _SIConnectionsHist.unique();
}
void Person::addAllConnection(int id){
    _AllConnections.push_back(id);
    _AllConnections.sort();
    _AllConnections.unique();
}
void Person::addAllConnectionHist(int id){
    _AllConnectionsHist.push_back(id);
    _AllConnectionsHist.sort();
    _AllConnectionsHist.unique();
}

Zone* Person::ZoneFromCo(double x, double y){
    
    double xmin, xmax, ymin, ymax;
    
    for(auto z = (_Location->getZones())->cbegin(); z != (_Location->getZones())->cend(); ++z){
        
        xmin = (*z)->Perimeter[0][0];
        xmax = (*z)->Perimeter[0][1];
        ymin = (*z)->Perimeter[1][0];
        ymax = (*z)->Perimeter[1][1];
        if (x >= xmin & x <= xmax & y >= ymin & y <= ymax) {
            return *z;
        }
    }
    
    return _Location->getZones()->front();
}


Person::~Person(){
    delete _generator;
}
