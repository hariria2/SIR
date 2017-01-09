//
//  Source.cpp
//  SIR
//
//  Created by Sahand Hariri on 6/29/16.
//  Copyright © 2016 Sahand Hariri. All rights reserved.
//

#include "Source.hpp"


Source::Source(string datadirectory){
	setDataDirectory(datadirectory);
}

void Source::setDataDirectory(string dd){
	_DataDirectory = dd;
}
void Source::setPlaces(vector<Place *> ps){
	_Places = ps;
}

string Source::getDataDirectory(){
	return _DataDirectory;
}
vector<Place*> Source::getPlaces(){
	return _Places;
}
void Source::readGeneralData(string FileName, Domain *domain){

	string filename;

	filename = _DataDirectory+FileName;

	ifstream File;
	File.open(filename, ios_base::in);

	if (File.is_open()){
		cout << "Data source file opened correctly." << endl;


		string sID;
		string name;
		string type;
		string sxmin;
		string sxmax;
		string symin;
		string symax;
		string spop;

		while (File.good()){
			getline(File, sID, ',');
			int ID = atoi(sID.c_str());

			getline(File, name, ',');

			getline(File, type, ',');

			getline(File, sxmin, ',');
			double xmin = atof(sxmin.c_str());

			getline(File, sxmax, ',');
			double xmax = atof(sxmax.c_str());

			getline(File, symin, ',');
			double ymin = atof(symin.c_str());

			getline(File, symax, ',');
			double ymax = atof(symax.c_str());

			getline(File, spop, '\n');
			int pop = atof(spop.c_str());


			//if (File.eof()) break;

			double boundary[2][2] = {{xmin, xmax},{ymin, ymax}};
			cout << "Population " << pop << endl;

			Place *h = new Place(ID, name, type, boundary, *domain, pop);
			_Places.push_back(h);

		}
		cout << "Finished reading the file. Will close now. " << endl;
		File.close();
	} else {
		cout << "File did not open correctly." << endl;
	}
};
void Source::getCoordinateDataForPlaces(){
	string placeName;
	string filename = _DataDirectory;
	vector<vector<double>> coordinates;

	for (auto p=_Places.begin(); p<_Places.end(); p++){
		coordinates.clear();
		placeName   = (*p)->getName();
		filename    = _DataDirectory+placeName+".csv";
		coordinates = readCoordinateFile(filename);
		(*p)->setCoordinates(coordinates);
		(*p)->setSides();
	}

}
void Source::getPolygonDataForPlaces(){
	string placeName;
	string filename = _DataDirectory;
	vector<vector<double>> triangles;

	for (auto p=_Places.begin(); p<_Places.end(); p++){
		triangles.clear();
		placeName   = (*p)->getName();
		filename    = _DataDirectory+placeName+"Polygon"+".csv";
		triangles   = readPolygonFile(filename);
		(*p)->setPolygonData(triangles);

	}};

vector<vector<double>> Source::readCoordinateFile(string filename){
	ifstream File;
	File.open(filename,ios_base::in);

	double x;
	double y;

	string sx;
	string sy;

	vector<double> co;
	vector<vector<double>> coordinates;

	if (File.is_open()){
		cout << "Coordinate file for " << filename << " opened up correctly." << endl;

		while (File.good()){
			co.clear();

			getline(File, sx, ',');
			x = atof(sx.c_str());

			getline(File, sy, '\n');
			y = atof(sy.c_str());

			co.push_back(x);
			co.push_back(y);

			coordinates.push_back(co);

		}

	}
	else{
		cout << "Coordinate file for " << filename << " FAILED to open." << endl;
	}

	return coordinates;
}
vector<vector<double>> Source::readPolygonFile(string filename){
	ifstream File;
	File.open(filename,ios_base::in);

	//Polygon data come in triangulated form. Each lines is x1,y1,x2,y2,x3,y3\n
	double x1,x2,x3;
	double y1,y2,y3;

	string sx1,sx2,sx3;
	string sy1,sy2,sy3;

	vector<double> tri;
	vector<vector<double>> tridata;

	if (File.is_open()){
		cout << "Coordinate file for " << filename << " opened up correctly." << endl;

		while (File.good()){
			tri.clear();

			getline(File, sx1, ',');
			getline(File, sy1, ',');

			getline(File, sx2, ',');
			getline(File, sy2, ',');

			getline(File, sx3, ',');
			getline(File, sy3, '\n');

			x1 = atof(sx1.c_str());
			x2 = atof(sx2.c_str());
			x3 = atof(sx3.c_str());

			y1 = atof(sy1.c_str());
			y2 = atof(sy2.c_str());
			y3 = atof(sy3.c_str());

			tri.push_back(x1);
			tri.push_back(y1);
			tri.push_back(x2);
			tri.push_back(y2);
			tri.push_back(x3);
			tri.push_back(y3);

			tridata.push_back(tri);

		}

	}
	return tridata;
}