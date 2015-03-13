/*
 * Disease.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */

#include "Disease.h"
#include <iostream>
#include <string>

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
	Name = name;
}
void Disease::setAverageInfectionPeriod(int ip){
	AverageInfectionPeriod = ip;
}
void Disease::setAverageIncubationPeriod(int ip){
	AverageIncubationPeriod = ip;
}
void Disease::setAverageRecoveryPeriod(int rp){
    AverageRecoveryPeriod = rp;
}

// Getters
int Disease::getAverageInfectionPeriod(){
	return AverageInfectionPeriod;
}
int Disease::getAverageIncubationPeriod(){
	return AverageIncubationPeriod;
}
int Disease::getAverageRecoveryPeriod(){
    return AverageRecoveryPeriod;
}
string Disease::getName(){
	return Name;
}



