#include "Person.h"

Person::Person(int id, string name, double age, char state, InHostDynamics ihd, Domain* city, Place* location, vector<Place*> availplaces, int inf_var, int inc_var, int rec_var):
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
	_TimeStep = 1;
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
	setAgeInteraction();
	setAgeGroup();
	
}// end constructor


Person::Person(int id, string name, double age, char state, InHostDynamics ihd, Domain* city,vector<Place*> availplaces, int inf_var, int inc_var, int rec_var, bool isSingleLocation):
_ihdynamics(ihd),
_AvailablePlaces(availplaces)
{
	_generator = new default_random_engine(_RandSeed);
	setID(id);
	setAge(age);
	setName(name);
	setState(state);
	_Time = 0;
	_TimeStep = 1;
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
	setAgeInteraction();
	setAgeGroup();
}

// Setters
void Person::setID(int id){
	_ID = id;
}
void Person::setX(double x){
	_X = x;
}
void Person::setY(double y){
	_Y = y;
}
void Person::setTimeStep(double ts){
	_TimeStep = ts;
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
	setX(_Coordinates[0]);
	setY(_Coordinates[1]);
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
void Person::setAgeInteraction(){
	_AgeInteraction["CC"]=0.01;
	_AgeInteraction["TT"]=1;
	_AgeInteraction["YY"]=0.6;
	_AgeInteraction["AA"]=1;
	_AgeInteraction["SS"]=.4;
	_AgeInteraction["CY"]=1;
	_AgeInteraction["AC"]=.0;
	_AgeInteraction["SC"]=0.;
}
void Person::setAgeGroup(){
	if (_Age < 10)
	{
		_AgeGroup = 'C';
	}
	else if (_Age < 20)
	{
		_AgeGroup = 'T';
	}
	else if (_Age < 40)
	{
		_AgeGroup = 'Y';
	}
	else if (_Age < 65)
	{
		_AgeGroup = 'A';
	}
	else
	{
		_AgeGroup = 'S';
	}
}

// Getters
int Person::getID(){
	return _ID;
}
double Person::getX(){
	return _X;
}
double Person::getY(){
	return _Y;
}
double Person::getTimeStep(){
	return _TimeStep;
}
double Person::getAge(){
	return _Age;
}
double Person::getMotionStepSize(){
	return _MotionStepSize;
}
int Person::getHasBeenSick(){
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
char Person::getAgeGroup(){
	return _AgeGroup;
}
string Person::getConnections(){
	return _Connections;
}

void Person::clearConnections(){
	_Connections = "";
}

void Person::Update(){
	/**
	 * \callergraph
	 * \todo
	 */

	_Age += _AgeIncrement;

	if (_State != 'D'){
		InteractWithOthers();
		Move();
		UpdateDiseaseWithInHost();

		if ((_Age >= 10) & (_Age - 0.1 < 15)){
			if (getState() == 'N'){
				setState('S');
				_ihdynamics.setT(3);
			}
		}

		if (_Age >= _LifeExpectancy){
			Die();
		}
	}
}

// Utilities
void Person::InteractWithOthers(){
	double criticalDistance  = 20;
	double criticalDistanceD = 2;
	
	double motionBias[2];
	double r[4];
	double dist;
	double theta;

	_TotalVirion = 0;
	_MotionBiasX = 0;
	_MotionBiasY = 0;

	for(auto ip = _Neighbors->cbegin(); ip != _Neighbors->cend(); ++ip){
		CartesianDistance(*ip,r);
		dist  = sqrt(r[0]*r[0]+r[1]*r[1]);//r[0];
		theta = atan2(r[1],r[0]);//r[1];

		if (dist != 0 & dist < criticalDistance){
			computeMotionEffect(r,(*ip)->getAgeGroup(),motionBias);
			_MotionBiasX += motionBias[0];
			_MotionBiasY += motionBias[1];

			//_Connections.append("," + to_string((*ip)->getID()));
		}
		if (dist != 0 & dist < criticalDistanceD){
			_TotalVirion += ((*ip)->_ihdynamics.getV())/(dist*dist);
		}
	}
}
void Person::computeMotionEffect(double* distVector, char ag, double * r){

	string interType = string()+_AgeGroup+ag;
	sort(interType.begin(),interType.end());

	double G = _AgeInteraction[interType];
	double critDist = 1;

	r[0] = (abs(distVector[0])<critDist)? 0:distVector[2]*pow(_TimeStep,2)*(G/pow(abs(distVector[0]),2));//*cos(distVector[1]);
	r[1] = (abs(distVector[1])<critDist)? 0:distVector[3]*pow(_TimeStep,2)*(G/pow(abs(distVector[1]),2));//*sin(distVector[1]);
}
void Person::Move(){

	normal_distribution<double> xdist(_MotionBiasX,_MotionStepSize);
	normal_distribution<double> ydist(_MotionBiasY,_MotionStepSize);
	double rx = xdist(*_generator);
	double ry = ydist(*_generator);

	double x = rx; //+ _MotionBiasX;
	double y = ry; //+ _MotionBiasY;


	if (_State == 'P' | _State == 'I'){
		Move(x,y, "IslandHopper");
	}else{
		Move(x/2,y/2, "IslandHopper");
	}
}
void Person::Move(double xr, double yr, string motionType){
	/**
	 * \callgraph
	 */

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

	
	double x = _X + xr; // r*cos(theta);
	double y = _Y + yr; //*sin(theta);
	double co[2] = {x,y};

	if(_Location->ContainsQ(x, y)){
		setCoordinates(co);
	}
	else{
		x = _X - xr;
		y = _Y - yr;
		co[0] = x;
		co[1] = y;
		setCoordinates(co);
	}

}
void Person::UpdateDiseaseWithInHost(){
	

	
	_ihdynamics.setT0(_Time);
	_ihdynamics.setNE(_TotalVirion);
	_ihdynamics.Simulate();
	
	if ((getState()=='R') & !(getHasBeenSick())) {
		if (_ihdynamics.getT() > 1.8){
			//setState('S');
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
	
	//_neigbors.clear();
}
void Person::CartesianDistance(Person* p, double *r){
	// order matters here.
	double xdiff = p->getX() - _X;
	double ydiff = p->getY() - _Y;

	r[0] = xdiff; //sqrt(pow(xdiff,2) + pow(ydiff,2));
	r[1] = ydiff; //atan2(ydiff, xdiff);
	r[2] = (xdiff > 0)? 1:-1;
	r[3] = (ydiff > 0)? 1:-1;
};
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
	Move(0, 0, "IslandHopper");
}


bool Person::operator=(Person &A) const{
	return (this->getState() == A.getState());
}

bool Person::operator == (const Person& p) const {
	return (p._ID == this->_ID);
}
Person::~Person(){
	delete _generator;
	//delete this;
}
