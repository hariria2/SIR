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

using namespace std;

Architect::Architect(double t0, double te, double ts,
		vector<Person *> pp, bool store, Storage* d):
	dataPtr(d)
{

	InitialTime  = t0;
	EndTime      = te;
	TimeStep     = ts;
	CurrentTime  = t0;
	TimeIndex    = 0;
	PeoplePtr    = pp;
	Store        = store;
	S = 0;
	I = 0;
    P = 0;
	R = 0;
    D = 0;
}

Architect::Architect(double t0, double te, double ts, vector<Person *> pp, bool store)
{
    
    InitialTime  = t0;
    EndTime      = te;
    TimeStep     = ts;
    CurrentTime  = t0;
    TimeIndex    = 0;
    PeoplePtr    = pp;
    Store        = store;
    S = 0;
    I = 0;
    R = 0;
    D = 0;
}

Architect::~Architect() {

}
// Setters

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
double Architect::getDailyTime(){
	int hour    = floor(CurrentTime);
	double min  = CurrentTime - hour;

	return ((hour % 24) + min);
}

// Utilities
void Architect::IncrementTime(){
	CurrentTime += TimeStep;
	TimeIndex++;
}
void Architect::Simulate(){
	if (Store){
		dataPtr->citySave();
		dataPtr->homeSave();
		dataPtr->workSave();
		dataPtr->schoolSave();
        dataPtr->cemeterySave();
		for (double t = 0; t < EndTime; t += TimeStep){
			Update(t, dataPtr);
		}
	}
	else{
		for (double t = 0; t < EndTime; t += TimeStep){
			Update(t);
		}
	}
}
void Architect::Update(double t, Storage* data){
    
    //Disease flu = (PeoplePtr.front())->getDisease();
    //Domain* myCity = (PeoplePtr.front())->getDomain();
    //Place* home = (PeoplePtr.front())->getHome();
    //double hco[2];
    //hco[0] = ((PeoplePtr.front())->getHomeCoordinates())[0];
    //hco[1] = ((PeoplePtr.front())->getHomeCoordinates())[1];
    
    //int s = PeoplePtr.size();
    //Person *p = new Person(s+1, "newBaby", 0, 'S', flu, myCity, home, hco, 10,10,10, true);
    //PeoplePtr.push_back(p);
    
    //s = PeoplePtr.size();
    //Person *p2 = new Person(s+1, "newBaby", 0, 'S', flu, myCity, home, hco, 10,10,10, true);
    //PeoplePtr.push_back(p2);
    
	PopulationData();
	data->saveSIR(TimeIndex, CurrentTime, S, I, P, R, D);
	data->startMovieSave(CurrentTime);
	IncrementTime();
	for (auto ip = PeoplePtr.cbegin(); ip != PeoplePtr.cend(); ++ip){
		data->movieSave((*ip)->getID(),
                        (*ip)->getName(),
                        (*ip)->getTime(),
                        (*ip)->getCoordinates(),
                       ((*ip)->getLocation())->getName(),
                        (*ip)->getState());
        
		(*ip)->setTime(CurrentTime);
		if ((*ip)->IsSingleLocation) {
			(*ip)->Move2((rand() % 360),200);
		}else{
			(*ip)->Move((rand() % 360),5);
		}
		(*ip)->UpdateDisease();
	}
	data->endMovieSave();
}
void Architect::Update(double t){
	IncrementTime();
	for (auto ip = PeoplePtr.cbegin(); ip != PeoplePtr.cend(); ++ip){
		(*ip)->setTime(CurrentTime);
		if ((*ip)->IsSingleLocation) {
			(*ip)->Move2((rand() % 360),5);
		}else{
			(*ip)->Move((rand() % 360),5);
		}
		(*ip)->UpdateDisease();
	}
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
    }
}
