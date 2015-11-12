/*
 * Architect.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: Sahand
 */


#include "Architect.h"

using namespace std;

Architect::Architect(double t0, double te, double ts,
		vector<Person *> pp, Economy* econ, string store, Storage* d):
	_dataPtr(d),
    _Econ(econ)
{

	_InitialTime  = t0;
	_EndTime      = te;
	_TimeStep     = ts;
	_CurrentTime  = t0;
	_TimeIndex    = 0;
	_PeoplePtr    = pp;
	_Store        = store;
    PopulationData();
}

Architect::Architect(double t0, double te, double ts,
                     vector<Person *> pp, Economy *econ, string store, SQLStorage* d):
    _sqlDataPtr(d),
    _Econ(econ)
{
    
    _InitialTime  = t0;
    _EndTime      = te;
    _TimeStep     = ts;
    _CurrentTime  = t0;
    _TimeIndex    = 0;
    _PeoplePtr    = pp;
    _Store        = store;
    PopulationData();
}

Architect::Architect(double t0, double te, double ts,vector<Person *> pp, string store, SQLStorage* d,Visualization* vis):
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
}
Architect::Architect(double t0, double te, double ts,vector<Person *> pp,Visualization* vis)
{
    _InitialTime  = t0;
    _EndTime      = te;
    _TimeStep     = ts;
    _CurrentTime  = t0;
    _TimeIndex    = 0;
    _PeoplePtr    = pp;
    //_Store        = store;
    setVisualization(vis);
    PopulationData();
}

Architect::Architect(double t0, double te, double ts,
                     vector<Person *> pp, Economy *econ, string store, SQLStorage* d,Visualization* vis):
    _sqlDataPtr(d),
    _Econ(econ)
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
}


Architect::Architect(double t0, double te, double ts, vector<Person *> pp, Economy *econ, string store):
    _Econ(econ)
{
    
    _InitialTime  = t0;
    _EndTime      = te;
    _TimeStep     = ts;
    _CurrentTime  = t0;
    _TimeIndex    = 0;
    _PeoplePtr    = pp;
    _Store        = store;
    PopulationData();
}



Architect::~Architect() {

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

// Getters
double Architect::getCurrentTime(){
	return _CurrentTime;
}
double Architect::getTimeStep(){
	return _TimeStep;
}
vector<Person*> Architect::getPeople(){
	return _PeoplePtr;
}
vector<Place*> Architect::getPlaces(){
    return _AllPlaces;
}
Visualization* Architect::getVisualization(){
    return _Visualization;
}

double Architect::getDailyTime(){
	int hour    = floor(_CurrentTime);
	double min  = _CurrentTime - hour;

	return ((hour % 24) + min);
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
                                     to_string(_D) + ", " +
                                     to_string(_Ho) + ", " +
                                     to_string(_Wo) + ", " +
                                     to_string(_Sc) + ", " +
                                     to_string(_Econ->getGDP()) + ", " +
                                     to_string(_Econ->getDemand())
                                     );
            Update(_sqlDataPtr);
            
            double time = (double)(clock()-start_s)/((double)CLOCKS_PER_SEC);
            if ((time*1000000) < (_TimeStep*1000000)){
                usleep(static_cast<int>((_TimeStep*1000000) - time*1000000));
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
        
            //double time = (double)(clock()-start_s)/((double)CLOCKS_PER_SEC);
            //if ((time*1000000) < (_TimeStep*1000000)){
            //    usleep(static_cast<int>((_TimeStep*1000000) - time*1000000));
            //}
            
        }
	}
    cout << "Simulation Complete. Thank you...!" << endl;
}
void Architect::Update(Storage* data){
    
	data->saveSIR(_TimeIndex, _CurrentTime, _S, _I, _P, _R, _D);
	data->startMovieSave(_CurrentTime);
	
    //Econ.computeGDP(PeoplePtr, Econ.getGDP());
    _Econ->getParameters(_PeoplePtr);
    _Econ->Update(_TimeStep);
    IncrementTime();
	
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
		if ((*ip)->IsSingleLocation) {
			(*ip)->Move2((rand() % 360),1);
		}else{
			(*ip)->Move((rand() % 360),1, "Travel");
		}
		(*ip)->UpdateDiseaseWithInHost();
	}
	data->endMovieSave();
    PopulationData();
}
void Architect::Update(SQLStorage* data){
    vector<Person*> econList;
    string SQLStatement;
    
    IncrementTime();
    for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend(); ++ip){
        
        
        if ((*ip)->getState() != 'I' || (*ip)->getState() != 'D') {
            econList.push_back(*ip);
        }
        
        ((*ip)->getInHostDynamics()).setMaxInfLev(0);
        
        
        SQLStatement = SQLStatement + "(NULL, " +
        to_string((*ip)->getID()) + ", " +
        to_string((*ip)->getTime()) + ", " +
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
        
        if ((*ip)->IsSingleLocation) {
            (*ip)->Move2(rand()%360+1 + 1,1);
        }else{
            //(*ip)->Move(rand()%360+1,1, "DailyMovement",_Econ->getDemand());
            (*ip)->Move(rand()%360+1,1, "DailyMovement");
        }
        if ((*ip)->getState() != 'D'){
            (*ip)->UpdateDiseaseWithInHost();
        }

    }
    SQLStatement.pop_back();
    data -> InsertValue("PersonValues",SQLStatement, true);
    
    //_Econ.computeGDP(econList, Econ.getGDP());
    _Econ->getParameters(econList);
    //_Econ.Update(TimeStep);
    PopulationData();
    
}
void Architect::Update(){
	
    _Econ->computeGDP(_PeoplePtr, _Econ->getGDP());
    IncrementTime();
	for (auto ip = _PeoplePtr.cbegin(); ip != _PeoplePtr.cend(); ++ip){
		(*ip)->setTime(_CurrentTime);
        ((*ip)->getInHostDynamics()).setMaxInfLev(0);
		if ((*ip)->IsSingleLocation) {
			(*ip)->Move2((rand() % 360),1);
		}else{
			(*ip)->Move((rand() % 360),1);
		}
        if ((*ip)->getState() != 'D'){
            (*ip)->UpdateDiseaseWithInHost();
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
    
    // Homes
    for(auto h = _Homes.cbegin(); h != _Homes.cend(); ++h) {
        _sqlDataPtr->InsertValue("Location",
                       "NULL, '"        +
                       (*h)->getName() + "', '" +
                       (*h)->getType() + "', "  +
                       to_string(((*h)->Perimeter)[0][0]) + ", " +
                       to_string(((*h)->Perimeter)[0][1]) + ", " +
                       to_string(((*h)->Perimeter)[1][0]) + ", " +
                       to_string(((*h)->Perimeter)[1][1]));
    }
    // Works
    for(auto h = _Works.cbegin(); h != _Works.cend(); ++h) {
        _sqlDataPtr->InsertValue("Location",
                       "NULL, '"        +
                       (*h)->getName() + "', '" +
                       (*h)->getType() + "', "  +
                       to_string(((*h)->Perimeter)[0][0]) + ", " +
                       to_string(((*h)->Perimeter)[0][1]) + ", " +
                       to_string(((*h)->Perimeter)[1][0]) + ", " +
                       to_string(((*h)->Perimeter)[1][1]));
    }
    // Schools
    for(auto h = _Schools.cbegin(); h != _Schools.cend(); ++h) {
        _sqlDataPtr->InsertValue("Location",
                       "NULL, '"        +
                       (*h)->getName() + "', '" +
                       (*h)->getType() + "', "  +
                       to_string(((*h)->Perimeter)[0][0]) + ", " +
                       to_string(((*h)->Perimeter)[0][1]) + ", " +
                       to_string(((*h)->Perimeter)[1][0]) + ", " +
                       to_string(((*h)->Perimeter)[1][1]));
    }
    // Cemeteries
    for(auto h = _Cemeteries.cbegin(); h != _Cemeteries.cend(); ++h) {
        _sqlDataPtr->InsertValue("Location",
                       "NULL, '"       +
                       (*h)->getName() + "', '" +
                       (*h)->getType() + "', "  +
                       to_string(((*h)->Perimeter)[0][0]) + ", " +
                       to_string(((*h)->Perimeter)[0][1]) + ", " +
                       to_string(((*h)->Perimeter)[1][0]) + ", " +
                       to_string(((*h)->Perimeter)[1][1]));
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
                                to_string(((*p)->getHome())->getID()) + ", " +
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
    
    InHostDynamics ihd = InHostDynamics(id,dt, 3, 0, 0.1, 3);
    ihd.setBeta(0.4);
    ihd.setDelta(0.02);
    ihd.setP(0.4);
    ihd.setC(0.5);
    ihd.setILRate(0.01);
    
    Disease dis = (p1->getDisease());
    Place* loc = LocFromCoo(x,y);
    vector<Place*> availPlaces = _AllPlaces;
    
    Person* p = new Person(id, "Alplego", 20, 'S', dis, ihd, _City, loc, availPlaces, 1,1,1);
    
    double coo[2] = {x,y};
    p->setCoordinates(coo);
    p->setTime(_CurrentTime);
    //p->setHasBeenSick(1);
    _sqlDataPtr->InsertValue("People",
                            "NULL, '" +
                            p->getName() + "', "+
                            to_string(p->getAge()) + ", '" +
                            p->getGender() + "', " +
                            to_string((p->getHome())->getID()) + ", " +
                            to_string((p->getLocation())->getID()));
    p->setLocation(loc);
    AddPerson(p);
    _Visualization->AddPerson(p);
}