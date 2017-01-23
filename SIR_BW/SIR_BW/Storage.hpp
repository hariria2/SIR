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
	Storage(int len, vector<Place*> locations, string historydatafoldername, string persondatafoldername);
	virtual ~Storage();

	void saveSIR(int idx,double t, int S, int P,int I, int R, int D);

	void writeSIR();

	void startPersonDataSave(double t);
	void endPersonDataSave();
	void personDataSave(int ID, double t, double age, double coord[2], int locationID, char state, double suscells, double inflev, double virlev);

	int* getII();
	int* getSS();
	int* getPP();
	int* getRR();
	int* getDD();

	string listToString(list<int> l);

private:
	int    _Length;
	string _HistoryDataFolderName;
	string _PersonValueFolderName;

	string _timeStamp;

	ofstream _HistoryDataFile;
	ofstream _PersonValueFile;

	double* _TT;
	int*    _II;
	int*    _SS;
	int*    _PP;
	int*    _RR;
	int*    _DD;
	int*    _XX;
};

#endif /* STORAGE_H_ */
