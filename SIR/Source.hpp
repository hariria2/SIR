//
//  Source.hpp
//  SIR
//
//  Created by Sahand Hariri on 6/29/16.
//  Copyright © 2016 Sahand Hariri. All rights reserved.
//

#ifndef Source_hpp
#define Source_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Domain.h"
#include "Place.h"

using namespace std;

class Source {


public:
	Source(string datadirectory);

	void setDataDirectory(string dd);
	void setPlaces(vector<Place*> ps);

	string getDataDirectory();
	vector<Place*> getPlaces();


	void readGeneralData(string FileName, Domain *domain);
	void getCoordinateDataForPlaces();
	void getPolygonDataForPlaces();

private:
	string _DataDirectory;
	vector<Place*> _Places;

	vector<vector<double>> readCoordinateFile(string filename);
	vector<vector<double>> readPolygonFile(string filename);

};

#endif /* Source_hpp */
