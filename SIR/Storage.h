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
#include <string>
#include <vector>

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
    void writeEigSIR();
	void startMovieSave(double t);
	void endMovieSave();
	void movieSave(int ID, string name, double t, double coord[2], string location, char state);
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

private:
	int Length;
	string dataFolderName;
	string movieFolderName;

	string cityFileName;
	string homeFileName;
	string workFileName;
	string schoolFileName;
    string cemeteryFileName;
    string timeStamp;
    
	ofstream historyFile;
    ofstream historyEigFile;
	ofstream movieFile;
	ofstream cityFile;
	ofstream homeFile;
	ofstream schoolFile;
	ofstream workFile;
    ofstream cemeteryFile;

	Domain* CityPtr;
	vector<Place*> Homes;
	vector<Place*> Schools;
	vector<Place*> Works;
    vector<Place*> Cemeteries;
	double* TT;
	int* II;
	int* SS;
    int* PP;
	int* RR;
    int* DD;
    int* XX;
};

#endif /* STORAGE_H_ */
