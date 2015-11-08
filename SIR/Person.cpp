/*
 * Person.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: Sahand
 */


#include "Person.h"

Person::Person(int id, string name, int age,
               char state, Disease dis, InHostDynamics ihd,
               Domain* city,    Place* home,
               Place* school,   Place* work,
               Place* cemetery, Place* location,
               double homeco[2],  double workco[2],
               double schoolco[2],double cemeteryco[2],
               int inf_var, int inc_var, int rec_var):
    _disease(dis),
    _ihdynamics(ihd),
    _City(city),
    _Home(home),
    _School(school),
    _Work(work),
    _Cemetery(cemetery),
    _Location(location)
{
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
               bool isSingleLocation):
    _disease(dis),
    _ihdynamics(ihd),
    _City(city),
    _Home(home),
    _Location(home)
{
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
    setHomeCoordinates(homeco);
	setLocation(_Location);
	IsSingleLocation = isSingleLocation;
    setGender('M');
}

// Setters
void Person::setID(int id){
	_ID = id;
}
void Person::setAge(int age){
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
void Person::setHomeCoordinates(double homeco[2]){
    for (int ii=0; ii<2; ii++){
        _HomeCoordinates[ii] = homeco[ii];
    }
}
void Person::setWorkCoordinates(double workco[2]){
    for (int ii=0; ii<2; ii++){
        _WorkCoordinates[ii] = workco[ii];
    }
}
void Person::setSchoolCoordinates(double schoolco[2]){
    for (int ii=0; ii<2; ii++){
        _SchoolCoordinates[ii] = schoolco[ii];
    }
}
void Person::setCemeteryCoordinates(double cemeteryco[2]){
    for (int ii=0; ii<2; ii++){
        _CemeteryCoordinates[ii] = cemeteryco[ii];
    }
}
void Person::setState(char state){
	_State = state;
}
void Person::setLocation(Place* location){
	_Location->removePerson(this);
	_Location = location;
	_Location->addPerson(this);
    
    if (_Location == _Home){
        setCoordinates(_HomeCoordinates);
    }else if (_Location == _Work){
        setCoordinates(_WorkCoordinates);
    }else if (_Location == _School){
        setCoordinates(_SchoolCoordinates);
    }else // Location == Cemetery
    {
        setCoordinates(_CemeteryCoordinates);
    }
}
void Person::setTime(double t){
	_Time = t;
}
void Person::setInfectionPeriod(){
	unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	normal_distribution<double> distribution(_disease.getAverageInfectionPeriod(),_InfectionVar);
	double randnum  = distribution(generator);
	randnum = (randnum < 0)? 0:randnum;
	_InfectionPeriod = floor(randnum);
}
void Person::setIncubationPeriod(){
	unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	normal_distribution<double> distribution(_disease.getAverageIncubationPeriod(),_IncubationVar);
	double randnum  = distribution(generator);
	randnum = (randnum < 0)? 0:randnum;
	_IncubationPeriod = floor(randnum);
}
void Person::setRecoveryPeriod(){
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<double> distribution(_disease.getAverageRecoveryPeriod(),_RecoveryVar);
    double randnum  = distribution(generator);
    randnum = (randnum < 0)? 0:randnum;
    _RecoveryPeriod = floor(randnum);
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

// Getters
int Person::getID(){
	return _ID;
}
int Person::getAge(){
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
Domain* Person::getDomain(){
    return _City;
}
Place* Person::getHome(){
    return _Home;
}
double* Person::getCoordinates(){
	 return _Coordinates;
}
double* Person::getHomeCoordinates(){
    return _HomeCoordinates;
}
double* Person::getWorkCoordinates(){
    return _WorkCoordinates;
}
double* Person::getSchoolCoordinates(){
    return _SchoolCoordinates;
}
double* Person::getCemeteryCoordinates(){
    return _CemeteryCoordinates;
}
double Person::getTime(){
	return _Time;
}
char Person::getState() const{
    return _State;
}
string Person::getName() const{
    return _Name;
}
Place* Person::getLocation(){
    return _Location;
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

// Utilities
void Person::Move(double theta, double r, string motionType, double demand) {
	int hour    = floor(_Time);
	double min  = _Time - hour;
	double DailyTime = ((hour % 24) + min);

    if (getState() == 'D') {
        setLocation(_Cemetery);
        return;
    }
    
    default_random_engine generator(_RandSeed);

    normal_distribution<double> WSTimeD(8,0.2);
    double  WSTime = WSTimeD(generator);
    normal_distribution<double> WETimeD(22,0.2);
    double  WETime = WETimeD(generator);
    
    
    int WSHour = floor(WSTime);
    double WSMin = WSTime - WSHour;
    double WSDailyTime = ((WSHour % 24)+WSMin);
    
    int WEHour = floor(WETime);;
    double WEMin = WETime - WEHour;;
    double WEDailyTime = ((WEHour % 24)+WEMin);
    
    
    if (motionType == "DailyMovement"){
        
        if (DailyTime <= WSDailyTime || DailyTime >= WEDailyTime){
            if (!(_Location->getType() == "Home")){
                setLocation(_Home);
            }
        }
        else{
            //ihdynamics.getI()<demand
            
            if (_Location->getType() == "Home" && _ihdynamics.getI()<demand){
                (_Age>22)? setLocation(_Work):setLocation(_School);
            }
        }
        
        
    }else if (motionType == "Travel"){
        if (getID() == 1){
            if (_Time == 10){
                setLocation(_Work);
            }
            if (_Time == 20){
                setLocation(_School);
            }
        }

        if (getID() == 9){
            if (_Time == 30){
                setLocation(_Home);
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
}
void Person::Move2(double theta, double r){
    
    if (getState() == 'D') {
        return;
    }
    
	double x = _Coordinates[0] + r*cos(theta);
	double y = _Coordinates[1] + r*sin(theta);
	double xmin = _Location->Perimeter[0][0];
	double xmax = _Location->Perimeter[0][1];
	double ymin = _Location->Perimeter[1][0];
	double ymax = _Location->Perimeter[1][1];

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

	_Coordinates[0] = x;
	_Coordinates[1] = y;
}
void Person::UpdateDisease() {
    
    // Note that SIConnectionsHist does not get cleared here.
    _SIConnections.clear();
    // Note that AllConnectionsHist does not get cleared here.
    _AllConnections.clear();
    
	
    
    list<Person*>* peeps = _Location->getOccupants();
    int criticalDistance = 23;
    
    for(auto ip = peeps->cbegin(); ip != peeps->cend(); ++ip){
        if (Distance(*ip) < criticalDistance){
            _neigbors.push_back(*ip);
            addAllConnection((*ip)->getID());
            addAllConnectionHist((*ip)->getID());
        }
    }
    
	if (getState() == 'S'){
		for(auto ip = peeps->cbegin(); ip != peeps->cend(); ++ip){
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
            setState('D');
            _TimeOfDeath = _Time;
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
    
    list<Person*>* peeps = _Location->getOccupants();

    int criticalDistance = 5;
    
    for(auto ip = peeps->cbegin(); ip != peeps->cend(); ++ip){
        
        if (Distance(*ip) < criticalDistance){
            if (getID() != ((*ip)->getID())){
                _neigbors.push_back(*ip);
                addAllConnection((*ip)->getID());
                addAllConnectionHist((*ip)->getID());
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
    _ihdynamics.setNE(0.001*totalVirion);
    _ihdynamics.Simulate();
    
    if ((getState()=='R') & !(getHastBeenSick())) {
        if (_ihdynamics.getT() > 2.8){
            setState('S');
        }
    }
    else if (getState() == 'S'){
        if (_ihdynamics.getV() > 0.01){
            setState('I');
        }
    }
    else if (getState() == 'I'){
        if (_Location->getName() == "Home"){
            _ihdynamics.setDelta(1.0*_ihdynamics.getDelta());
        }else {
            _ihdynamics.setDelta(1.0*_ihdynamics.getDelta());
        }
        
        if (_ihdynamics.getI() > 0.2 & _ihdynamics.getI() < 3 & _HasBeenSick==0){
            setState('P');
            _ihdynamics.HasBeenSick = 1;
        }else if (_ihdynamics.getI() > 2.5){
            setState('D');
        }
        else if (_ihdynamics.getI() < 0.01 & _HasBeenSick == 1){
            setState('R');
        }
    }else if (getState() == 'P'){
        if (_ihdynamics.getI() + 0.01 < _ihdynamics.getMaxInfLev()){
            setHasBeenSick(1);
        }
        if (_Location->getName() == "Home"){
            _ihdynamics.setDelta(1.0*_ihdynamics.getDelta());
        }else {
            _ihdynamics.setDelta(1.0*_ihdynamics.getDelta());
        }
        if (_ihdynamics.getI() > 3){
            setState('D');
        }
        else if (_ihdynamics.getI() < 0.2){
            setState('I');
        }
        
    }else if (getState() == 'R'){
        if (_ihdynamics.getI() < 0.1){
            //ihdynamics.setT(ihdynamics.getTi());
            setHasBeenSick(0);
            _ihdynamics.HasBeenSick = 1;
        }
        
    }
    
}
double Person::Distance(Person* p){
    
    double p1x = _Coordinates[0];
    double p1y = _Coordinates[1];
    
    double p2x = (p->getCoordinates())[0];
    double p2y = (p->getCoordinates())[1];
    
    return sqrt(pow((p2x-p1x),2) + pow((p2y - p1y),2));
}

//Not sure what this function is doing. Deprecated???!!!!
void Person::UpdateWithinHost(list<Person*> ngbrs){
    
    for(auto ip = ngbrs.cbegin(); ip != ngbrs.cend(); ++ip){
        
    }
    
    _ihdynamics.setdt(_Time);
    // everything gets set
    _ihdynamics.Simulate();
    
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

Person::~Person(){

}
