/*
 * Disease.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */

#include "Disease.h"


using namespace std;


Disease::Disease(string name, int incubationPeriod, int infectionPeriod, int recoveryPeriod)
{
	setName(name);
	setAverageIncubationPeriod(incubationPeriod);
    setAverageInfectionPeriod(infectionPeriod);
    setAverageRecoveryPeriod(recoveryPeriod);
}// end of constructor

// Setters
void Disease::setName(string name){
	_Name = name;
}
void Disease::setAverageInfectionPeriod(int ip){
	_AverageInfectionPeriod = ip;
}
void Disease::setAverageIncubationPeriod(int ip){
	_AverageIncubationPeriod = ip;
}
void Disease::setAverageRecoveryPeriod(int rp){
    _AverageRecoveryPeriod = rp;
}

// Getters
int Disease::getAverageInfectionPeriod(){
	return _AverageInfectionPeriod;
}
int Disease::getAverageIncubationPeriod(){
	return _AverageIncubationPeriod;
}
int Disease::getAverageRecoveryPeriod(){
    return _AverageRecoveryPeriod;
}
string Disease::getName(){
	return _Name;
}



