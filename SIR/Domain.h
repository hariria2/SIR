/*
 * Domain.h
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */
#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <iostream>

using namespace std;


class Domain
{
public:
	int Boundary[2][2];
	// Default Constructor
	Domain();
	
	// Overloaded Constructor
	Domain(string name, int boundary[2][2]);
	
	// Destructor
	~Domain();
	
	// Accessors
	string getName();
	
	// Mutators
	void setName(string);
	void setBoundary(int boundary[2][2]);
	
	
private:
	string _Name;
	
};

#endif /* DOMAIN_H_ */
