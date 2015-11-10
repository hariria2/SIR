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
void Architect::setHomes(vector<Place*> homes){
    _Homes = homes;
};
void Architect::setSchools(vector<Place*> schools){
    _Schools = schools;
};
void Architect::setWorks(vector<Place*> works){
    _Works = works;
};
void Architect::setCemetaries(vector<Place*> cemeteries){
    _Cemeteries = cemeteries;
};
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
int Architect::getSc(){
    return _Sc;
}
int Architect::getHo(){
    return _Ho;
}
int Architect::getWo(){
    return _Wo;
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
            
            
            //double time = (double)(clock()-start_s)/((double)CLOCKS_PER_SEC);
            //if ((time*1000000) < (_TimeStep*1000000)){
            //    usleep(static_cast<int>((_TimeStep*1000000) - time*1000000));
            //}
            
        }
        
    }
	else{
		for (double t = 0; t < _EndTime; t += _TimeStep){
			Update();
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
		if ((*ip)->IsSingleLocation) {
			(*ip)->Move2((rand() % 360),200);
		}else{
			(*ip)->Move((rand() % 360),5);
		}
		(*ip)->UpdateDisease();
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
    _Wo = 0;
    _Sc = 0;
    _Ho = 0;
    
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
        if (((*ip)->getLocation())->getType() == "Work"){
            _Wo += 1;
        }
        else if(((*ip)->getLocation())->getType() == "Home"){
            _Ho += 1;
        }
        else if(((*ip)->getLocation())->getType() == "School"){
            _Sc += 1;
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
    
    for(auto c = _Cemeteries.cbegin(); c != _Cemeteries.cend(); ++c){
        xmin = (*c)->Perimeter[0][0];
        xmax = (*c)->Perimeter[0][1];
        ymin = (*c)->Perimeter[1][0];
        ymax = (*c)->Perimeter[1][1];
        
        if (x >= xmin & x <= xmax & y >= ymin & y <= ymax) {
            return *c;
        }
    }
    
    for(auto s = _Schools.cbegin(); s != _Schools.cend(); ++s){
        xmin = (*s)->Perimeter[0][0];
        xmax = (*s)->Perimeter[0][1];
        ymin = (*s)->Perimeter[1][0];
        ymax = (*s)->Perimeter[1][1];
        
        if (x >= xmin & x <= xmax & y >= ymin & y <= ymax) {
            return *s;
        }
    }
    
    for(auto w = _Works.cbegin(); w != _Works.cend(); ++w){
        xmin = (*w)->Perimeter[0][0];
        xmax = (*w)->Perimeter[0][1];
        ymin = (*w)->Perimeter[1][0];
        ymax = (*w)->Perimeter[1][1];
        
        if (x >= xmin & x <= xmax & y >= ymin & y <= ymax) {
            return *w;
        }
    }
    
    for(auto h = _Homes.cbegin(); h != _Homes.cend(); ++h){
        xmin = (*h)->Perimeter[0][0];
        xmax = (*h)->Perimeter[0][1];
        ymin = (*h)->Perimeter[1][0];
        ymax = (*h)->Perimeter[1][1];
        
        if (x >= xmin & x <= xmax & y >= ymin & y <= ymax) {
            return *h;
        }
    }
    
    cout << "No Homeless people allowed in DiseaseVille!" << endl;
    
    return _Homes.front();
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
    Place* school;
    Place* home;
    Place* work;
    Place* cemetery;
    double schoolco[2];
    double homeco[2];
    double workco[2];
    double cemeteryco[2];
    
    if (loc->getType() == "School"){
        school = loc;
        home = _Homes.front();
        work = _Works.front();
        cemetery = _Cemeteries.front();
        
        schoolco[0] = x;
        schoolco[1] = y;
        
        homeco[0] = (double) *(home->Perimeter[0]);
        homeco[1] = (double) *(home->Perimeter[1]);
        
        workco[0] = (double) *(work->Perimeter[0]);
        workco[1] = (double) *(work->Perimeter[1]);
        
        cemeteryco[0] = (double) *(cemetery->Perimeter[0]);
        cemeteryco[1] = (double) *(cemetery->Perimeter[1]);
        
    }
    else if (loc->getType() == "Work"){
        school = _Schools.front();
        home = _Homes.front();
        work = loc;
        cemetery = _Cemeteries.front();
        
        schoolco[0] = (double) *(school->Perimeter[0]);
        schoolco[1] = (double) *(school->Perimeter[0]);
        
        homeco[0] = (double) *(home->Perimeter[0]);
        homeco[1] = (double) *(home->Perimeter[1]);
        
        workco[0] = x;
        workco[1] = y;
        
        cemeteryco[0] = (double) *(cemetery->Perimeter[0]);
        cemeteryco[1] = (double) *(cemetery->Perimeter[1]);
        
    }
    else if (loc->getType() == "Home"){
        school = _Schools.front();
        home = loc;
        work = _Works.front();
        cemetery = _Cemeteries.front();
        
        homeco[0] = x;
        homeco[1] = y;
        
        schoolco[0] = (double) *(school->Perimeter[0]);
        schoolco[1] = (double) *(school->Perimeter[1]);
        
        workco[0] = (double) *(work->Perimeter[0]);
        workco[1] = (double) *(work->Perimeter[1]);
        
        cemeteryco[0] = (double) *(cemetery->Perimeter[0]);
        cemeteryco[1] = (double) *(cemetery->Perimeter[1]);
        
    }
    else if (loc->getType() == "Cemetery"){
        school = _Schools.front();
        home = _Homes.front();
        work = _Works.front();
        cemetery = loc;
        
        cemeteryco[0] = x;
        cemeteryco[1] = y;
        
        homeco[0] = (double) *(home->Perimeter[0]);
        homeco[1] = (double) *(home->Perimeter[1]);
        
        workco[0] = (double) *(work->Perimeter[0]);
        workco[1] = (double) *(work->Perimeter[1]);
        
        schoolco[0] = (double) *(school->Perimeter[0]);
        schoolco[1] = (double) *(school->Perimeter[1]);
        
    }
    
    Person* p = new Person(id, "Alplego", 20, 'S', dis, ihd, _City, home, school, work, cemetery, loc, homeco, workco, schoolco, cemeteryco,1,1,1);
    
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