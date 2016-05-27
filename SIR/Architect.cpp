/*
 * Architect.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: Sahand
 */


#include "Architect.h"
#include "unistd.h"

using namespace std;

Architect::Architect(double t0, double te, double ts,list<Person *> pp,Visualization* vis)
{
    _InitialTime  = t0;
    _EndTime      = te;
    _TimeStep     = ts;
    _CurrentTime  = t0;
    _TimeIndex    = 0;
    _PeoplePtr    = pp;
    setVisualization(vis);
    PopulationData();
    _generator = new default_random_engine(_RandSeed);
    _introtimeDist = new uniform_int_distribution<int>(650, 950);
    
    for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend();ip++){
        (*ip)->setNeighbors(&_PeoplePtr);
    }
    _SQLBatchSize = 0;
    _SaveIntegerTimes = false;
}

Architect::Architect(double t0, double te, double ts,list<Person *> pp, string store, SQLStorage* d):
_sqlDataPtr(d)
{
    _InitialTime  = t0;
    _EndTime      = te;
    _TimeStep     = ts;
    _CurrentTime  = t0;
    _TimeIndex    = 0;
    _PeoplePtr    = pp;
    _Store        = store;
    PopulationData();
    _generator = new default_random_engine(_RandSeed);
    _introtimeDist = new uniform_int_distribution<int>(650, 950);
    for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend();ip++){
        (*ip)->setNeighbors(&_PeoplePtr);
    }
    _SQLBatchSize = 0;
    _SaveIntegerTimes = false;
}

Architect::Architect(double t0, double te, double ts,list<Person *> pp,Visualization* vis, string store, SQLStorage* d):
    _sqlDataPtr(d)
{
    _InitialTime  = t0;
    _EndTime      = te;
    _TimeStep     = ts;
    _CurrentTime  = t0;
    _TimeIndex    = 0;
    _PeoplePtr    = pp;
    _Store        = store;
    setVisualization(vis);
    PopulationData();
    _generator = new default_random_engine(_RandSeed);
    _introtimeDist = new uniform_int_distribution<int>(650, 950);
    for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend();ip++){
        (*ip)->setNeighbors(&_PeoplePtr);
    }
    _SQLBatchSize = 0;
    _SaveIntegerTimes = false;
}



Architect::~Architect() {
    delete _generator;
    delete _introtimeDist;
}

// Setters
void Architect::setDomain(Domain *city){
    _City = city;
};
void Architect::setPlaces(vector<Place *> places){
    _AllPlaces = places;
}
void Architect::setVisualization(Visualization *vis){
    _Visualization = vis;
}
void Architect::setBatchSize(int btchsz){
    _SQLBatchSize=btchsz;
};
void Architect::setSaveIntegerTimes(bool siono){
    _SaveIntegerTimes = siono;
}

// Getters
double Architect::getCurrentTime(){
	return _CurrentTime;
}
double Architect::getTimeStep(){
	return _TimeStep;
}
list<Person*> Architect::getPeople(){
	return _PeoplePtr;
}
vector<Place*> Architect::getPlaces(){
    return _AllPlaces;
}
Visualization* Architect::getVisualization(){
    return _Visualization;
}
double Architect::getMonthlyTime(){
	int hour    = floor(_CurrentTime);
	double min  = _CurrentTime - hour;

	return ((hour % 30) + min);
}
int Architect::getBatchSize(){
    return _SQLBatchSize;
}
int Architect::getS(){
    return _S;
}
int Architect::getI(){
    return _I;
}
int Architect::getP(){
    return _P;
}
int Architect::getR(){
    return _R;
}
int Architect::getD(){
    return _D;
}
int Architect::getN(){
    return _N;
}
bool Architect::getSaveIntegerTimes(){
    return _SaveIntegerTimes;
}
Domain* Architect::getDomain(){
    return _City;
}

// Utilities
void Architect::IncrementTime(){
	_CurrentTime += _TimeStep;
	_TimeIndex++;
}
void Architect::Simulate(){
    
    
    _BirthRate = (_N>0.0)? 0.1:0.3;
    
    bool timeIntegerQ = (_SaveIntegerTimes)? (abs(_CurrentTime - round(_CurrentTime)) < _TimeStep/2.):true;
    cout << timeIntegerQ << endl;
    if (_Store == "MYSQL"){
        
        PrepDB();
        int batchctr = 0;
        string statement="";
        int introtime;
        
        if (_Visualization == NULL){
            
            for (double t = 0; t < _EndTime; t += _TimeStep){  //Main Loop
                
                if (_PeoplePtr.empty()){
                    cout << "You have run out of people. Enjoy playing god without puppets." << endl;
                    return;
                }
                
                int indx  = rand() % (_AllPlaces.size()-1);
                
                introtime = (*_introtimeDist)(*_generator);
                
                if (_CurrentTime != 0 & (fmod(_CurrentTime,introtime)) < 1e-6){
                    
                    double xmin = (_AllPlaces[indx]->Perimeter)[0][0];
                    double xmax = (_AllPlaces[indx]->Perimeter)[0][1];
                    double ymin = (_AllPlaces[indx]->Perimeter)[1][0];
                    double ymax = (_AllPlaces[indx]->Perimeter)[1][1];
                    
                    uniform_real_distribution<double> xdist(xmin, xmax);
                    uniform_real_distribution<double> ydist(ymin, ymax);
                    double x = xdist(*_generator);
                    double y = ydist(*_generator);
                    AddPerson(x,y);
                    cout << "==================>>>>>Traveler arrived<<<<<==============" << endl;
                }
                
                if (timeIntegerQ){//(abs(_CurrentTime - round(_CurrentTime)) < _TimeStep/2.){
                    cout << "time " << _CurrentTime << "!" << endl;
                    
                    if (batchctr < _SQLBatchSize){
                        
                        statement = statement + "(" + "NULL, " +
                        to_string(_CurrentTime) + ", " +
                        to_string(_S) + ", " +
                        to_string(_I) + ", " +
                        to_string(_P) + ", " +
                        to_string(_R) + ", " +
                        to_string(_D) + ", " +
                        to_string(_N) + "),";
                        batchctr++;
                    }
                    else{
                        statement = statement + "(" + "NULL, " +
                        to_string(_CurrentTime) + ", " +
                        to_string(_S) + ", " +
                        to_string(_I) + ", " +
                        to_string(_P) + ", " +
                        to_string(_R) + ", " +
                        to_string(_D) + ", " +
                        to_string(_N) + ")";
                        _sqlDataPtr-> InsertValue("HistoryData",statement, true);
                        batchctr = 0;
                        statement = "";
                    }
                    
                }
                
                Update(_sqlDataPtr);
                
                if (_BirthRate >= 1){
                    for (int i = 0; i<_BirthRate; i++){
                        AddPerson("NewBirth");
                        cout << "============>>>>>New birth<<<<============" << endl;
                    }
                } else if (_BirthRate>0){
                    if (fmod(_CurrentTime/_TimeStep, floor(1/_BirthRate)) == 0 ){
                        AddPerson("NewBirth");
                        cout << "============>>>>>New birth<<<<============" << endl;
                    }
                }
            }

        }
        else {
            while (!glfwWindowShouldClose(_Visualization->getWindow())){
                unsigned long start_s=clock();
        
            _Visualization->Render();
                
                int indx  = rand() % (_AllPlaces.size()-1);
                
                introtime = (*_introtimeDist)(*_generator);
                
                if (_CurrentTime != 0 & (fmod(_CurrentTime,introtime)) < 1e-6){ //(timeIntegerQ){ //
                    
                    double xmin = (_AllPlaces[indx]->Perimeter)[0][0];
                    double xmax = (_AllPlaces[indx]->Perimeter)[0][1];
                    double ymin = (_AllPlaces[indx]->Perimeter)[1][0];
                    double ymax = (_AllPlaces[indx]->Perimeter)[1][1];
                    
                    uniform_real_distribution<double> xdist(xmin, xmax);
                    uniform_real_distribution<double> ydist(ymin, ymax);
                    double x = xdist(*_generator);
                    double y = ydist(*_generator);
                    AddPerson(x,y);
                    cout << "=====================>>>>>Traveler arrived<<<<<==============" << endl;
                }
                
                if (abs(_CurrentTime - round(_CurrentTime)) < _TimeStep/2.){
                    cout << "time " << _CurrentTime << "!" << endl;
                    
                    if (batchctr < 5){
                        
                        statement = statement + "(" + "NULL, " +
                        to_string(_CurrentTime) + ", " +
                        to_string(_S) + ", " +
                        to_string(_I) + ", " +
                        to_string(_P) + ", " +
                        to_string(_R) + ", " +
                        to_string(_D) + ", " +
                        to_string(_N) + "),";
                        batchctr++;
                    }
                    else{
                        statement = statement + "(" + "NULL, " +
                        to_string(_CurrentTime) + ", " +
                        to_string(_S) + ", " +
                        to_string(_I) + ", " +
                        to_string(_P) + ", " +
                        to_string(_R) + ", " +
                        to_string(_D) + ", " +
                        to_string(_N) + ")";
                        _sqlDataPtr-> InsertValue("HistoryData",statement, true);
                        batchctr = 0;
                        statement = "";
                    }
                    
                }
                Update(_sqlDataPtr);
                
                
                
                if (_BirthRate >= 1){
                    for (int i = 0; i<_BirthRate; i++){
                        AddPerson("NewBirth");
                        cout << "============>>>>>New birth<<<<============" << endl;
                    }
                } else if (_BirthRate>0){
                    if (fmod(_CurrentTime/_TimeStep, floor(1/_BirthRate)) == 0 ){
                        AddPerson("NewBirth");
                        cout << "============>>>>>New birth<<<<============" << endl;
                    }
                }
                
                double time = (double)(clock()-start_s)/((double)CLOCKS_PER_SEC);
                if ((time*1000000) < (_TimeStep*1000000)){
                    usleep(static_cast<int>((_TimeStep*1000000) - time*1000000));
                }
            }
        }
    }
	else{
        
        while (!glfwWindowShouldClose(_Visualization->getWindow())){
            unsigned long start_s=clock();
            if (_CurrentTime - floor(_CurrentTime) < _TimeStep){
                cout << "time " << _CurrentTime << "!" << endl;
            }
            
            _Visualization->Render();
            
            Update();
            
            if (_BirthRate >= 1){
                for (int i = 0; i<_BirthRate; i++){
                    AddPerson("NewBirth");
                    cout << "============>>>>>New birth<<<<============" << endl;
                }
            } else {
                if (fmod(_CurrentTime, floor(1/_BirthRate)) == 0 ){
                    AddPerson("NewBirth");
                    cout << "============>>>>>New birth<<<<============" << endl;
                }
            }
            
            double time = (double)(clock()-start_s)/((double)CLOCKS_PER_SEC);
            if ((time*1000000) < (_TimeStep*1000000)){
                usleep(static_cast<int>((_TimeStep*1000000) - time*1000000));
            }
            
        }
	}
    cout << "Simulation Complete. Thank you...!" << endl;
}
void Architect::Update(SQLStorage* data){
    
    string SQLStatement;
    IncrementTime();
    list<Person*> peeps;
    
    for (auto pl = _AllPlaces.begin(); pl != _AllPlaces.end(); pl++){
        peeps = *(*pl)->getOccupants();
    
        for (auto ip = peeps.begin(); ip != peeps.end(); ip++){
            
            
            
            SQLStatement = SQLStatement + "(NULL, " +
            to_string((*ip)->getID()) + ", " +
            to_string((*ip)->getTime()) + ", " +
            to_string((*ip)->getAge()) + ", " +
            to_string((*ip)->getCoordinates()[0]) + ", " +
            to_string((*ip)->getCoordinates()[1]) + ", " +
            to_string(((*ip)->getLocation())->getID()) + ", '" +
            (*ip)->getState() + "', '" +
            (*ip)->getConnections() + "', " +
            to_string((*ip)->getHasBeenSick()) + ", " +
            to_string(((*ip)->getInHostDynamics()).getT()) + ", " +
            to_string(((*ip)->getInHostDynamics()).getI()) + ", " +
            to_string(((*ip)->getInHostDynamics()).getV()) + ", " +
            to_string(((*ip)->getInHostDynamics()).getMaxInfLev())+
            "),";
            
            if ((*ip)->getAge() > 1000 | (*ip)->getAge() < 0){
                cout << (*ip)->getAge()  << endl;
                cout << "this is the id: " << (*ip)->getAge()  << endl;
                int a;
                cin >> a;
            }

            (*ip)->clearConnections();
            
            
            (*ip)->setTime(_CurrentTime);
            (*ip)->Update();
            
            if ((*pl)->getType()=="Cemetery"){
                if (_CurrentTime >= (*ip)->getTimeOfDeath()+15){
                    Funeral(*ip);
                    RemovePerson(*ip);
                    delete(*ip);
                    if (ip == _PeoplePtr.end()){
                        break;
                    } else {
                        ip = ip++;
                    }
                }
            }
            else{
                (*ip)->setNeighbors(&peeps);
                ((*ip)->getInHostDynamics()).setMaxInfLev(0);
            }
        }
    }

    
    SQLStatement.pop_back();
    data -> InsertValue("PersonValues",SQLStatement, true);

    PopulationData();
    
}
void Architect::Update(){
	
    IncrementTime();
    
    
    list<Person*> peeps;
    
    
    for (auto pl = _AllPlaces.cbegin(); pl != _AllPlaces.cend(); pl++){

        peeps = *(*pl)->getOccupants();
        
        for (auto ip = peeps.cbegin(); ip != peeps.cend(); ip++){
            
            
            (*ip)->setTime(_CurrentTime);
            (*ip)->Update();
            if ((*pl)->getType()=="Cemetery"){
                if (_CurrentTime > (*ip)->getTimeOfDeath()+15){
                    Funeral(*ip);
                    RemovePerson(*ip);
                    delete(*ip);
                    if (ip == _PeoplePtr.end()){
                        break;
                    } else {
                        ip = ip++;
                    }
                }
                
            }else{
                (*ip)->setNeighbors(&peeps);
                ((*ip)->getInHostDynamics()).setMaxInfLev(0);
            }
        }
    }
 
    PopulationData();
}
void Architect::DisplayTime(){
	int day   = floor(_CurrentTime/24);
	int mhour = floor(_CurrentTime);
	mhour     = mhour % 24;
	int  hour = mhour % 12;

	int min   = floor((_CurrentTime - floor(_CurrentTime))*60);

	string AmPm = ((mhour < 12)? "AM":"PM");
	cout << "Day " << ((day < 10 )? " ":"") << day <<  ", ";
	cout << ((hour <  10 && hour > 0)? " ":"");
	cout << ((hour ==  0)? 12:hour) << ":";
	cout << ((min  <  10)? "0":"");
	cout << min  << " " << AmPm << endl;

}
void Architect::PopulationData(){
    _S = 0;
    _I = 0;
    _P = 0;
    _R = 0;
    _D = 0;
    _N = 0;
    
    
    for(auto ip = _PeoplePtr.begin(); ip != _PeoplePtr.end(); ++ip) {

        
        if (((*ip)->getState()) == 'I'){
            _I += 1;
            if ((*ip)->getNewInf()){
                _N += 1;
            }
        }
        else if(((*ip)->getState()) == 'P'){
            _P += 1;
        }
        else if(((*ip)->getState()) == 'S'){
            _S += 1;
        }
        else if(((*ip)->getState()) == 'R'){
            _R += 1;
        }
        else if(((*ip)->getState()) == 'D'){
            _D += 1;
        }
        
    }
}
void Architect::RemovePerson(Person *p){
    _PeoplePtr.remove(p);
}
void Architect::PrepDB(){
    // ====================>>>>LocationData<<<========================== //
    // Domain
    
    
    _sqlDataPtr->InsertValue("Location",
                   "NULL, '1', '" +
                   _City->getName() + "', " +
                   "'Domain'"     + ", " +
                   to_string((_City->Boundary)[0][0]) + ", " +
                   to_string((_City->Boundary)[0][1]) + ", " +
                   to_string((_City->Boundary)[1][0]) + ", " +
                   to_string((_City->Boundary)[1][1]));
    for(auto p = _AllPlaces.cbegin(); p != _AllPlaces.cend(); ++p) {
        _sqlDataPtr->InsertValue("Location",
                                 "NULL, '"        +
                                 to_string((*p)->getID()) + "', '" +
                                 (*p)->getName() + "', '" +
                                 (*p)->getType() + "', "  +
                                 to_string(((*p)->Perimeter)[0][0]) + ", " +
                                 to_string(((*p)->Perimeter)[0][1]) + ", " +
                                 to_string(((*p)->Perimeter)[1][0]) + ", " +
                                 to_string(((*p)->Perimeter)[1][1]));
    }
    // =====================>>>End of LocationData<<<========================= //
    
    // =====================>>>People Data<<<================================= //
    unsigned long ps = _PeoplePtr.size();
    
    cout << "Prepping tables for " << ps << " people. Please wait..." << endl;
    for(auto p = _PeoplePtr.cbegin(); p != _PeoplePtr.cend(); ++p) {
        
        _sqlDataPtr->InsertValue("People",
                            "NULL, '"        +
                            (*p)->getName() + "', " +
                            to_string((*p)->getAge()) + ", '"  +
                            (*p)->getGender() + "', " +
                            to_string(((*p)->getDeafaultLocation())->getID()) + ", " +
                            to_string(((*p)->getLocation())->getID()));
        
    }
    
    
    // =====================>>>End of People Data<<<========================== //
    
    
}
Place* Architect::LocFromCoo(double x, double y){
    
    double xmin, xmax, ymin, ymax;
    
    for(auto p = _AllPlaces.cbegin(); p != _AllPlaces.cend(); ++p){
        xmin = (*p)->Perimeter[0][0];
        xmax = (*p)->Perimeter[0][1];
        ymin = (*p)->Perimeter[1][0];
        ymax = (*p)->Perimeter[1][1];
        
        if (x >= xmin & x <= xmax & y >= ymin & y <= ymax) {
            return *p;
        }
    }
    
    cout << "No Homeless people allowed in DiseaseVille!" << endl;
    return _AllPlaces.front();
}
void Architect::AddPerson(Person *p){
    _PeoplePtr.push_back(p);
    PopulationData();
}
void Architect::AddPerson(double x, double y){
    
    unsigned long s = _PeoplePtr.size();
    int id = (int) s + 1;
    Person* p1 = _PeoplePtr.front();
    double dt = (p1->getInHostDynamics()).getdt();
    
    InHostDynamics ihd = InHostDynamics(id,dt, 3, 0, 0.3, 2,44, 40, 100);
    ihd.setBeta(0.2);
    ihd.setDelta(0.03);
    ihd.setP(0.4);
    ihd.setC(0.5);
    ihd.setILRate(0.001);
    
    Place* loc = LocFromCoo(x,y);

    vector<Place*> availPlaces = p1->getAvailablePlaces();
    //vector<Place*> availPlaces = _PeoplePtr[randPIdx]->getAvailablePlaces();
    
    Person* p = new Person(id, "Alplego", 20, 'S', ihd, _City, loc, availPlaces, 1,1,1);
    
    p->setTravelerQ(false);
    for (auto l= availPlaces.cbegin(); l != availPlaces.cend(); l++){
        if ((*l)->getType()=="Home"){
            p->setDefaultLocation(*l);
        }
    }
    p->setTime(_CurrentTime);
    p->setAgeIncrement(_TimeStep/365);
    p->setNeighbors(&_PeoplePtr);
    
    double coo[2] = {x,y};
    p->setCoordinates(coo);
    p->setTime(_CurrentTime);
    //p->setHasBeenSick(1);
    p->setMotionStepSize(0.05);
    if (_Store == "MYSQL"){
        _sqlDataPtr->InsertValue("People",
                                 "NULL, '" +
                                 p->getName() + "', "+
                                 to_string(p->getAge()) + ", '" +
                                 p->getGender() + "', " +
                                 to_string((p->getDeafaultLocation())->getID()) + ", " +
                                 to_string((p->getLocation())->getID()));
    }

    p->setTravelerQ(false);
    AddPerson(p);
    if (_Visualization != NULL) {
        _Visualization->AddPerson(p);
    }
    
}
void Architect::AddPerson(string NewBirth){
    
    int indx;
    if ((_AllPlaces.size()-1)>1){
        indx  = rand() % (_AllPlaces.size()-1); // -1 is here to exclude cemetery from the possible location of birth
    }
    else{
        indx = 0;
    }
    double xmin = (_AllPlaces[indx]->Perimeter)[0][0];
    double xmax = (_AllPlaces[indx]->Perimeter)[0][1];
    double ymin = (_AllPlaces[indx]->Perimeter)[1][0];
    double ymax = (_AllPlaces[indx]->Perimeter)[1][1];
    
    uniform_real_distribution<double> xdist(xmin, xmax);
    uniform_real_distribution<double> ydist(ymin, ymax);
    double x = xdist(*_generator);
    double y = ydist(*_generator);
    
    unsigned long s = _PeoplePtr.size();
    int id = (int) s + 1;
    Person* p1 = _PeoplePtr.front();
    double dt = (p1->getInHostDynamics()).getdt();
    
    InHostDynamics ihd = InHostDynamics(id,dt, 0, 0, 0, 2,44, 40, 100);
    ihd.setBeta(1.5);
    ihd.setDelta(0.7);
    ihd.setP(0.4);
    ihd.setC(1.0);
    ihd.setILRate(0.001);
    
    Place* loc = LocFromCoo(x,y);
    vector<Place*> availPlaces = p1->getAvailablePlaces();
    //vector<Place*> availPlaces = _PeoplePtr[randPIdx]->getAvailablePlaces();
    
    Person* p = new Person(id, "Alplego", 0, 'N', ihd, _City, loc, availPlaces, 1,1,1);
    
    for (auto l= availPlaces.cbegin(); l != availPlaces.cend(); l++){
        if ((*l)->getType()=="Home"){
            p->setDefaultLocation(*l);
        }
    }
    double coo[2] = {x,y};
    p->setCoordinates(coo);
    p->setTime(_CurrentTime);
    p->setAgeIncrement(_TimeStep/365);
    p->setNeighbors(&_PeoplePtr);
    //p->setTravelerQ(true);
    //p->setHasBeenSick(1);
    if (_Store == "MYSQL"){
        _sqlDataPtr->InsertValue("People",
                                 "NULL, '" +
                                 p->getName() + "', "+
                                 to_string(p->getAge()) + ", '" +
                                 p->getGender() + "', " +
                                 to_string((p->getDeafaultLocation())->getID()) + ", " +
                                 to_string((p->getLocation())->getID()));
    }
    AddPerson(p);
    if (_Visualization != NULL) {
        _Visualization->AddPerson(p);
    }
    
}
void Architect::Funeral(Person* p){
    
    if (_Visualization == NULL) {
        (p->getLocation())->removePerson(p);
    }
    else {
        (p->getLocation())->removePerson(p);
        _Visualization->removePerson(p);
    }
}