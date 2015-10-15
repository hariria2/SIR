/*
 * Architect.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: Sahand
 */

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <cmath>
#include "Architect.h"
#include "Person.h"
#include "Storage.h"
#include "Economy.h"

using namespace std;

Architect::Architect(double t0, double te, double ts,
		vector<Person *> pp, Economy econ, string store, Storage* d):
	dataPtr(d), Econ(econ)
{

	InitialTime  = t0;
	EndTime      = te;
	TimeStep     = ts;
	CurrentTime  = t0;
	TimeIndex    = 0;
	PeoplePtr    = pp;
	Store        = store;
    PopulationData();
}

Architect::Architect(double t0, double te, double ts,
                     vector<Person *> pp, Economy econ, string store, SQLStorage* d):
sqlDataPtr(d), Econ(econ)
{
    
    InitialTime  = t0;
    EndTime      = te;
    TimeStep     = ts;
    CurrentTime  = t0;
    TimeIndex    = 0;
    PeoplePtr    = pp;
    Store        = store;
    PopulationData();
}

Architect::Architect(double t0, double te, double ts,
                     vector<Person *> pp, Economy econ, string store, SQLStorage* d,Visualization* vis):
sqlDataPtr(d), Econ(econ)
{
    
    InitialTime  = t0;
    EndTime      = te;
    TimeStep     = ts;
    CurrentTime  = t0;
    TimeIndex    = 0;
    PeoplePtr    = pp;
    Store        = store;
    setVisualization(vis);
    PopulationData();
}


Architect::Architect(double t0, double te, double ts, vector<Person *> pp, Economy econ, string store):Econ(econ)
{
    
    InitialTime  = t0;
    EndTime      = te;
    TimeStep     = ts;
    CurrentTime  = t0;
    TimeIndex    = 0;
    PeoplePtr    = pp;
    Store        = store;
    PopulationData();
}



Architect::~Architect() {

}
// Setters

void Architect::setDomain(Domain city){
    City = city;
};
void Architect::setHomes(vector<Place*> homes){
    Homes = homes;
};
void Architect::setSchools(vector<Place*> schools){
    Schools = schools;
};
void Architect::setWorks(vector<Place*> works){
    Works = works;
};
void Architect::setCemetaries(vector<Place*> cemeteries){
    Cemeteries = cemeteries;
};
void Architect::setVisualization(Visualization *vis){
    _Visualization = vis;
}

// Getters
double Architect::getCurrentTime(){
	return CurrentTime;
}
double Architect::getTimeStep(){
	return TimeStep;
}
vector<Person*> Architect::getPeople(){
	return PeoplePtr;
}
Visualization* Architect::getVisualization(){
    return _Visualization;
}

double Architect::getDailyTime(){
	int hour    = floor(CurrentTime);
	double min  = CurrentTime - hour;

	return ((hour % 24) + min);
}
int Architect::getS(){
    return S;
}
int Architect::getI(){
    return I;
}
int Architect::getP(){
    return P;
}
int Architect::getR(){
    return R;
}
int Architect::getSc(){
    return Sc;
}
int Architect::getHo(){
    return Ho;
}
int Architect::getWo(){
    return Wo;
}

// Utilities
void Architect::IncrementTime(){
	CurrentTime += TimeStep;
	TimeIndex++;
}
void Architect::Simulate(){
	if (Store == "FileSystem"){
		dataPtr->citySave();
		dataPtr->homeSave();
		dataPtr->workSave();
		dataPtr->schoolSave();
        dataPtr->cemeterySave();
		for (double t = 0; t < EndTime; t += TimeStep){
			Update(t, dataPtr);
		}
        dataPtr->writeSIR();
	}
    else if (Store == "MYSQL"){
        PrepDB();
        _Visualization->Init();
        double Time = 0;
        while (!glfwWindowShouldClose(_Visualization->getWindow())){
            
            if (Time - floor(Time) < TimeStep){
                cout << "time " << Time << "!" << endl;
            }
            
            _Visualization->Render(PeoplePtr);
            
            sqlDataPtr-> InsertValue("HistoryData",
                                     "NULL, " +
                                     to_string(Time) + ", " +
                                     to_string(S) + ", " +
                                     to_string(I) + ", " +
                                     to_string(P) + ", " +
                                     to_string(R) + ", " +
                                     to_string(D) + ", " +
                                     to_string(Ho) + ", " +
                                     to_string(Wo) + ", " +
                                     to_string(Sc) + ", " +
                                     to_string(Econ.getGDP()) + ", " +
                                     to_string(Econ.getDemand())
                                     );
            Update(Time, sqlDataPtr);
            Time += TimeStep;
            usleep(static_cast<int>(TimeStep*1000000));
        }

//        for (double t = 0; t < EndTime; t += TimeStep){
//            
//            cout << "=========>>>>> Time " << t << " of " << EndTime << " <<<<<=================="<< endl;
//            sqlDataPtr-> InsertValue("HistoryData",
//                                  "NULL, " +
//                                  to_string(t) + ", " +
//                                  to_string(S) + ", " +
//                                  to_string(I) + ", " +
//                                  to_string(P) + ", " +
//                                  to_string(R) + ", " +
//                                  to_string(D) + ", " +
//                                  to_string(Ho) + ", " +
//                                  to_string(Wo) + ", " +
//                                  to_string(Sc) + ", " +
//                                  to_string(Econ.getGDP()) + ", " +
//                                  to_string(Econ.getDemand())
//                                  );
//            Update(t, sqlDataPtr);
//            
//        }
        
    }
	else{
		for (double t = 0; t < EndTime; t += TimeStep){
			Update(t);
		}
	}
    cout << "Simulation Complete. Thank you...!" << endl;
}
void Architect::Update(double t, Storage* data){
    
	data->saveSIR(TimeIndex, CurrentTime, S, I, P, R, D);
	data->startMovieSave(CurrentTime);
	
    //Econ.computeGDP(PeoplePtr, Econ.getGDP());
    Econ.getParameters(PeoplePtr);
    Econ.Update(TimeStep);
    IncrementTime();
	
    for (auto ip = PeoplePtr.cbegin(); ip != PeoplePtr.cend(); ++ip){
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
        
		(*ip)->setTime(CurrentTime);
		if ((*ip)->IsSingleLocation) {
			(*ip)->Move2((rand() % 360),5);
		}else{
			(*ip)->Move((rand() % 360),2, "Travel");
		}
		(*ip)->UpdateDiseaseWithInHost();
	}
	data->endMovieSave();
    PopulationData();
}
void Architect::Update(double t, SQLStorage* data){
    
    vector<Person*> econList;
    string SQLStatement;
    
    IncrementTime();
    for (auto ip = PeoplePtr.cbegin(); ip != PeoplePtr.cend(); ++ip){
        
        
        
       // if (((*ip)->getLocation())->getType()=="Work" || ((*ip)->getLocation())->getType()=="School") {
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
        
        (*ip)->setTime(CurrentTime);
        
        if ((*ip)->IsSingleLocation) {
            (*ip)->Move2((rand() % 360),5);
        }else{
            (*ip)->Move((rand() % 360),2, "DailyMovement",Econ.getDemand());
        }
        if ((*ip)->getState() != 'D'){
            (*ip)->UpdateDiseaseWithInHost();
        }

    }
    SQLStatement.pop_back();
    data -> InsertValue("PersonValues",SQLStatement, true);
    
    //Econ.computeGDP(econList, Econ.getGDP());
    Econ.getParameters(econList);
    Econ.Update(TimeStep);
    PopulationData();
    
}
void Architect::Update(double t){
	
    Econ.computeGDP(PeoplePtr, Econ.getGDP());
    IncrementTime();
	for (auto ip = PeoplePtr.cbegin(); ip != PeoplePtr.cend(); ++ip){
		(*ip)->setTime(CurrentTime);
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
	int day   = floor(CurrentTime/24);
	int mhour = floor(CurrentTime);
	mhour     = mhour % 24;
	int  hour = mhour % 12;

	int min   = floor((CurrentTime - floor(CurrentTime))*60);

	string AmPm = ((mhour < 12)? "AM":"PM");
	cout << "Day " << ((day < 10 )? " ":"") << day <<  " ";
	cout << ((hour <  10 && hour > 0)? " ":"");
	cout << ((hour ==  0)? 12:hour) << ":";
	cout << ((min  <  10)? "0":"");
	cout << min  << " " << AmPm << endl;

}
void Architect::PopulationData(){
    S = 0;
    I = 0;
    P = 0;
    R = 0;
    D = 0;
    Wo = 0;
    Sc = 0;
    Ho = 0;
    
    for(auto ip = PeoplePtr.cbegin(); ip != PeoplePtr.cend(); ++ip) {
        if (((*ip)->getState()) == 'I'){
            I += 1;
        }
        else if(((*ip)->getState()) == 'P'){
            P += 1;
        }
        else if(((*ip)->getState()) == 'S'){
            S += 1;
        }
        else if(((*ip)->getState()) == 'R'){
            R += 1;
        }
        else{
            D += 1;
        }
        if (((*ip)->getLocation())->getType() == "Work"){
            Wo += 1;
        }
        else if(((*ip)->getLocation())->getType() == "Home"){
            Ho += 1;
        }
        else if(((*ip)->getLocation())->getType() == "School"){
            Sc += 1;
        }
        
    }
}
void Architect::AddPerson(Person *p){
    PeoplePtr.push_back(p);
    PopulationData();
}
void Architect::PrepDB(){

    
    // ====================>>>>LocationData<<<========================== //
    // Domain
    sqlDataPtr->InsertValue("Location",
                   "NULL, '" +
                   City.getName() + "', " +
                   "'Domain'"     + ", " +
                   to_string((City.Boundary)[0][0]) + ", " +
                   to_string((City.Boundary)[0][1]) + ", " +
                   to_string((City.Boundary)[1][0]) + ", " +
                   to_string((City.Boundary)[1][1]));
    
    // Homes
    for(auto h = Homes.cbegin(); h != Homes.cend(); ++h) {
        sqlDataPtr->InsertValue("Location",
                       "NULL, '"        +
                       (*h)->getName() + "', '" +
                       (*h)->getType() + "', "  +
                       to_string(((*h)->Perimeter)[0][0]) + ", " +
                       to_string(((*h)->Perimeter)[0][1]) + ", " +
                       to_string(((*h)->Perimeter)[1][0]) + ", " +
                       to_string(((*h)->Perimeter)[1][1]));
    }
    // Works
    for(auto h = Works.cbegin(); h != Works.cend(); ++h) {
        sqlDataPtr->InsertValue("Location",
                       "NULL, '"        +
                       (*h)->getName() + "', '" +
                       (*h)->getType() + "', "  +
                       to_string(((*h)->Perimeter)[0][0]) + ", " +
                       to_string(((*h)->Perimeter)[0][1]) + ", " +
                       to_string(((*h)->Perimeter)[1][0]) + ", " +
                       to_string(((*h)->Perimeter)[1][1]));
    }
    // Schools
    for(auto h = Schools.cbegin(); h != Schools.cend(); ++h) {
        sqlDataPtr->InsertValue("Location",
                       "NULL, '"        +
                       (*h)->getName() + "', '" +
                       (*h)->getType() + "', "  +
                       to_string(((*h)->Perimeter)[0][0]) + ", " +
                       to_string(((*h)->Perimeter)[0][1]) + ", " +
                       to_string(((*h)->Perimeter)[1][0]) + ", " +
                       to_string(((*h)->Perimeter)[1][1]));
    }
    // Cemeteries
    for(auto h = Cemeteries.cbegin(); h != Cemeteries.cend(); ++h) {
        sqlDataPtr->InsertValue("Location",
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
    unsigned long ps = PeoplePtr.size();
    
    cout << "Prepping tables for " << ps << " people. Please wait..." << endl;
    for(auto p = PeoplePtr.cbegin(); p != PeoplePtr.cend(); ++p) {

        sqlDataPtr->InsertValue("People",
                                "NULL, '"        +
                                (*p)->getName() + "', " +
                                to_string((*p)->getAge()) + ", '"  +
                                (*p)->getGender() + "', " +
                                to_string(((*p)->getHome())->getID()) + ", " +
                                to_string(((*p)->getLocation())->getID()));
    }
    
    
    // =====================>>>End of People Data<<<========================== //
    
    
}