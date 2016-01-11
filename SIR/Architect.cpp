/*
 * Architect.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: Sahand
 */


#include "Architect.h"

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
    _introtimeDist = new uniform_int_distribution<int>(600, 1000);
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
    _introtimeDist = new uniform_int_distribution<int>(600, 1000);
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
    _introtimeDist = new uniform_int_distribution<int>(600, 1000);
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
    for (auto p = _AllPlaces.cbegin(); p != _AllPlaces.cend(); ++p){
        (*p)->setZoneNeighborhood();
    }
}
void Architect::setVisualization(Visualization *vis){
    _Visualization = vis;
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
Domain* Architect::getDomain(){
    return _City;
}

// Utilities
void Architect::IncrementTime(){
	_CurrentTime += _TimeStep;
	_TimeIndex++;
}
void Architect::Simulate(){
    
	if (_Store == "FileSystem"){
		_dataPtr->citySave();
		_dataPtr->homeSave();
		_dataPtr->workSave();
		_dataPtr->schoolSave();
        _dataPtr->cemeterySave();
		for (double t = 0; t < _EndTime; t += _TimeStep){
			Update(_dataPtr);
		}
        _dataPtr->writeSIR();
	}
    else if (_Store == "MYSQL"){
        PrepDB();
        if (_Visualization == NULL){
            //_sqlDataPtr->StartTransaction();
            int batchctr = 0;
            string statement="";
            int introtime;
            for (double t = 0; t < _EndTime; t += _TimeStep){
                int indx  = rand() % (_AllPlaces.size()-2);
                
                introtime = (*_introtimeDist)(*_generator);
                
                if (_CurrentTime != 0 & (fmod(_CurrentTime,introtime)) < 1e-6){
                    //int indx = rand() % _AllPlaces.size();
                    double xmin = (_AllPlaces[indx]->Perimeter)[0][0];
                    double xmax = (_AllPlaces[indx]->Perimeter)[0][1];
                    double ymin = (_AllPlaces[indx]->Perimeter)[1][0];
                    double ymax = (_AllPlaces[indx]->Perimeter)[1][1];
                    
                    uniform_real_distribution<double> xdist(xmin, xmax);
                    uniform_real_distribution<double> ydist(ymin, ymax);
                    double x = xdist(*_generator);
                    double y = ydist(*_generator);
                    AddPerson(x,y);
                    cout << "Person Added" << endl;
                }
                
                if (abs(_CurrentTime - round(_CurrentTime)) < _TimeStep/2.){
                    cout << "time " << _CurrentTime << "!" << endl;
                    
                    if (batchctr < 500){
                        
                        statement = statement + "(" + "NULL, " +
                        to_string(_CurrentTime) + ", " +
                        to_string(_S) + ", " +
                        to_string(_I) + ", " +
                        to_string(_P) + ", " +
                        to_string(_R) + ", " +
                        to_string(_D) + "),";
                        batchctr++;
                    }
                    else{
                        statement = statement + "(" + "NULL, " +
                        to_string(_CurrentTime) + ", " +
                        to_string(_S) + ", " +
                        to_string(_I) + ", " +
                        to_string(_P) + ", " +
                        to_string(_R) + ", " +
                        to_string(_D) + ")";
                        _sqlDataPtr-> InsertValue("HistoryData",statement, true);
                        batchctr = 0;
                        statement = "";
                        }
                    
                }
                Update();
                if (_CurrentTime != 0 & (fmod(_CurrentTime,5)) < 1e-3){
                    AddPerson("NewBirth");
                }
            }
            
            //_sqlDataPtr->EndTransaction();
        }
        else {
            while (!glfwWindowShouldClose(_Visualization->getWindow())){
                unsigned long start_s=clock();
                if (_CurrentTime - floor(_CurrentTime) < _TimeStep){
                    cout << "time " << _CurrentTime << "!" << endl;
                }
            
                _Visualization->Render();
            
                _sqlDataPtr-> InsertValue("HistoryData",
                                      "NULL, " +
                                      to_string(_CurrentTime) + ", " +
                                      to_string(_S) + ", " +
                                      to_string(_I) + ", " +
                                      to_string(_P) + ", " +
                                      to_string(_R) + ", " +
                                      to_string(_D)
                                      );
            
                Update(_sqlDataPtr);
                if (_CurrentTime != 0 & (fmod(_CurrentTime,5)) < 1e-3){
                    AddPerson("NewBirth");
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
            if (_CurrentTime != 0 & (fmod(_CurrentTime,5)) < 1e-3){
                AddPerson("NewBirth");
            }
            double time = (double)(clock()-start_s)/((double)CLOCKS_PER_SEC);
            if ((time*1000000) < (_TimeStep*1000000)){
                usleep(static_cast<int>((_TimeStep*1000000) - time*1000000));
            }
            
        }

	}
    cout << "Simulation Complete. Thank you...!" << endl;
}
void Architect::Update(Storage* data){
    
	data->saveSIR(_TimeIndex, _CurrentTime, _S, _I, _P, _R, _D);
	data->startMovieSave(_CurrentTime);
	
    //Econ.computeGDP(PeoplePtr, Econ.getGDP());
    //_Econ->getParameters(_PeoplePtr);
    _Econ->Update(_TimeStep);
    IncrementTime();
    for (auto p = _PeoplePtr.cbegin(); p != _PeoplePtr.cend(); p++){
        (*p)->Update();
    }
	/*  did not erase yet because this block has storage code
    for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend(); ++ip){
        ((*ip)->getInHostDynamics()).setMaxInfLev(0);
		data->movieSave((*ip)->getID(),
                        (*ip)->getName(),
                        (*ip)->getTime(),
                        (*ip)->getCoordinates(),
                       ((*ip)->getLocation())->getName(),
                        (*ip)->getState(),
                        (*ip)->getHastBeenSick(),
                       ((*ip)->getInHostDynamics()).getT(),
                       ((*ip)->getInHostDynamics()).getI(),
                       ((*ip)->getInHostDynamics()).getV(),
                       ((*ip)->getInHostDynamics()).getMaxInfLev(),
                        ((*ip)->getSIConnections()),
                        ((*ip)->getSIConnectionsHist()),
                        ((*ip)->getAllConnections()),
                        ((*ip)->getAllConnectionsHist()));
        
		(*ip)->setTime(_CurrentTime);
        (*ip)->Update();
	}
	*/
     data->endMovieSave();
    PopulationData();
}
void Architect::Update(SQLStorage* data){
    vector<Person*> econList;
    string SQLStatement;
    IncrementTime();
    for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend();ip++){
        
        //if ((*ip)->getState() != 'I' || (*ip)->getState() != 'D') {
        //    econList.push_back(*ip);
        //}
        //(*ip)->setZone();
        ((*ip)->getInHostDynamics()).setMaxInfLev(0);
        SQLStatement = SQLStatement + "(NULL, " +
        to_string((*ip)->getID()) + ", " +
        to_string((*ip)->getTime()) + ", " +
        to_string((*ip)->getAge()) + ", " +
        to_string((*ip)->getCoordinates()[0]) + ", " +
        to_string((*ip)->getCoordinates()[1]) + ", " +
        to_string(((*ip)->getLocation())->getID()) + ", '" +
        (*ip)->getState() + "', " +
        to_string((*ip)->getHastBeenSick()) + ", " +
        to_string(((*ip)->getInHostDynamics()).getT()) + ", " +
        to_string(((*ip)->getInHostDynamics()).getI()) + ", " +
        to_string(((*ip)->getInHostDynamics()).getV()) + ", " +
        to_string(((*ip)->getInHostDynamics()).getMaxInfLev())+
        "),";
        
        (*ip)->setTime(_CurrentTime);
        (*ip)->Update();
        
        if ((*ip)->getState()=='D' & (*ip)->getAge() >= (*ip)->getTimeOfDeath()+5){
            Funeral(*ip);
            delete (*ip);
            ip=_PeoplePtr.erase(ip);
        }
    }
    
    SQLStatement.pop_back();
    data -> InsertValue("PersonValues",SQLStatement, true);
    
    //_Econ.computeGDP(econList, Econ.getGDP());
    //_Econ->getParameters(econList);
    //_Econ.Update(TimeStep);
    PopulationData();
    
}
void Architect::Update(){
	
    //_Econ->computeGDP(_PeoplePtr, _Econ->getGDP());
    IncrementTime();
	for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend(); ++ip){
        //(*ip)->setZone();
        (*ip)->setTime(_CurrentTime);
        ((*ip)->getInHostDynamics()).setMaxInfLev(0);
        (*ip)->Update();
        if ((*ip)->getState()=='D' & (*ip)->getAge() >= (*ip)->getLifeExpectancy()+20){
            Funeral(*ip);
            delete (*ip);
            ip=_PeoplePtr.erase(ip);
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
    
    for(auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend(); ++ip) {
        if (((*ip)->getState()) == 'I'){
            _I += 1;
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
        else{
            _D += 1;
        }
        
    }
}
void Architect::AddPerson(Person *p){
    _PeoplePtr.push_back(p);
    PopulationData();
}
void Architect::RemovePerson(Person *p){
    _PeoplePtr.remove(p);
}
void Architect::PrepDB(){
    // ====================>>>>LocationData<<<========================== //
    // Domain
    _sqlDataPtr->InsertValue("Location",
                   "NULL, '" +
                   _City->getName() + "', " +
                   "'Domain'"     + ", " +
                   to_string((_City->Boundary)[0][0]) + ", " +
                   to_string((_City->Boundary)[0][1]) + ", " +
                   to_string((_City->Boundary)[1][0]) + ", " +
                   to_string((_City->Boundary)[1][1]));
    for(auto p = _AllPlaces.cbegin(); p != _AllPlaces.cend(); ++p) {
        _sqlDataPtr->InsertValue("Location",
                                 "NULL, '"        +
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
void Architect::AddPerson(double x, double y){
    
    unsigned long s = _PeoplePtr.size();
    int id = (int) s + 1;
    Person* p1 = _PeoplePtr.front();
    double dt = (p1->getInHostDynamics()).getdt();
    
    InHostDynamics ihd = InHostDynamics(id,dt, 3, 0, 0.3, 2);
    ihd.setBeta(0.2);
    ihd.setDelta(0.03);
    ihd.setP(0.4);
    ihd.setC(0.5);
    ihd.setILRate(0.001);
    
    Disease dis = (p1->getDisease());
    Place* loc = LocFromCoo(x,y);
    vector<Place*> availPlaces = p1->getAvailablePlaces();
    //vector<Place*> availPlaces = _PeoplePtr[randPIdx]->getAvailablePlaces();
    
    Person* p = new Person(id, "Alplego", 20, 'S', dis, ihd, _City, loc, availPlaces, 1,1,1);
    
    p->setTravelerQ(true);
    for (auto l= availPlaces.cbegin(); l != availPlaces.cend(); l++){
        if ((*l)->getType()=="Home"){
            p->setDefaultLocation(*l);
        }
    }
    double coo[2] = {x,y};
    p->setCoordinates(coo);
    p->setTime(_CurrentTime);
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
void Architect::AddPerson(string NewBirth){
    
    
    
    int indx  = rand() % (_AllPlaces.size()-2); // -1 is here to exclude cemetery from the possible location of birth
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
    
    InHostDynamics ihd = InHostDynamics(id,dt, 0, 0, 0, 2);
    ihd.setBeta(0.2);
    ihd.setDelta(0.03);
    ihd.setP(0.4);
    ihd.setC(0.5);
    ihd.setILRate(0.001);
    
    Disease dis = (p1->getDisease());
    Place* loc = LocFromCoo(x,y);
    vector<Place*> availPlaces = p1->getAvailablePlaces();
    //vector<Place*> availPlaces = _PeoplePtr[randPIdx]->getAvailablePlaces();
    
    Person* p = new Person(id, "Alplego", 0, 'N', dis, ihd, _City, loc, availPlaces, 1,1,1);
    
    for (auto l= availPlaces.cbegin(); l != availPlaces.cend(); l++){
        if ((*l)->getType()=="Home"){
            p->setDefaultLocation(*l);
        }
    }
    double coo[2] = {x,y};
    p->setCoordinates(coo);
    p->setTime(_CurrentTime);
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