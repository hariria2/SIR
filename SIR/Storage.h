/*
 * Storage.h
 *
 *  Created on: Jan 12, 2015
 *      Author: Sahand
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <sys/stat.h>
#include "Place.h"


using namespace std;

class Domain;
class Place;

class Storage {
public:
	Storage(int len, Domain* city, vector<Place*> homes, vector<Place*> works, vector<Place*> schools,vector<Place*> cemeteries,
			  string datafoldername, string moviefoldername);
	virtual ~Storage();
	void saveSIR(int idx,double t, int S, int P,int I, int R, int D);
	void writeSIR();
	void startMovieSave(double t);
	void endMovieSave();
	void movieSave(int ID, string name, double t, double coord[2], string location, char state, bool hbs, double inflev, double suscells, double virlev, double maxInfLev, list<int> SIconnections, list<int> SIconnectionsHist, list<int> Allconnections, list<int> AllconnectionsHist);
	void citySave();
	void homeSave();
	void workSave();
	void schoolSave();
    void cemeterySave();

	int* getII();
	int* getSS();
    int* getPP();
	int* getRR();
    int* getDD();
    
    string listToString(list<int> l);

private:
	int _Length;
	string _dataFolderName;
	string _movieFolderName;

	string _cityFileName;
	string _homeFileName;
	string _workFileName;
	string _schoolFileName;
    string _cemeteryFileName;
    string _timeStamp;
    
	ofstream _historyFile;
	ofstream _movieFile;
	ofstream _cityFile;
	ofstream _homeFile;
	ofstream _schoolFile;
	ofstream _workFile;
    ofstream _cemeteryFile;

	Domain* _CityPtr;
	vector<Place*> _Homes;
	vector<Place*> _Schools;
	vector<Place*> _Works;
    vector<Place*> _Cemeteries;
	double* _TT;
	int* _II;
	int* _SS;
    int* _PP;
	int* _RR;
    int* _DD;
    int* _XX;
};

#endif /* STORAGE_H_ */
