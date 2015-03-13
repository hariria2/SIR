/*
 * Disease.h
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */

#ifndef DISEASE_H_
#define DISEASE_H_

#include <iostream>
#include <string>
#include <vector>


using namespace std;

class Disease {
private:
	string Name;
	int AverageInfectionPeriod;
	int AverageIncubationPeriod;
    int AverageRecoveryPeriod;

public:
	Disease(string name, int incubationPeriod, int infectionPeriod, int recoveryPeriod);

	// Setters
	void setName(string x);
	void setAverageInfectionPeriod(int x);
	void setAverageIncubationPeriod(int x);
    void setAverageRecoveryPeriod(int x);
    
	// Getters
	string getName();
	int getAverageInfectionPeriod();
	int getAverageIncubationPeriod();
    int getAverageRecoveryPeriod();
};

#endif /* DISEASE_H_ */
