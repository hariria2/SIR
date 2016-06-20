/*
 * Domain.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */


#include "Domain.h"


using namespace std;



Domain::Domain(){
}
Domain::Domain(string name, int boundary[2][2]){
	
	setName(name);
	setBoundary(boundary);
}// end constructor


// setters
void Domain::setName(string name){
	_Name = name;
}
void Domain::setBoundary(int boundary[2][2]){
	for (int ii=0; ii<2; ii++){
		for (int jj=0; jj<2; jj++){
			Boundary[ii][jj] = boundary[ii][jj];
		}
	}
}


// getters
string Domain::getName(){
	return _Name;
}

Domain::~Domain(){
	
} // end destructor
