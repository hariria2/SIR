/*
 * Storage.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: Sahand
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include "Domain.h"
#include "Place.h"
#include "Storage.hpp"
using namespace std;


Storage::Storage(int len, vector<Place*> locations, string historydatafoldername, string persondatafoldername): _Length(len), _HistoryDataFolderName(historydatafoldername), _PersonValueFolderName(persondatafoldername)
{
	_TT = new double[_Length];
	_II = new int[_Length];
	_PP = new int[_Length];
	_SS = new int[_Length];
	_RR = new int[_Length];
	_DD = new int[_Length];
	_XX = new int[_Length];

	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	_timeStamp = to_string((now->tm_mon + 1)) + "-" +
	to_string(now->tm_mday) + "-" +
	to_string((now->tm_year + 1900));

}

void Storage::saveSIR(int idx, double t, int S, int I, int P, int R, int D){
	_TT[idx] = t;
	_II[idx] = I;
	_PP[idx] = P;
	_SS[idx] = S;
	_RR[idx] = R;
	_DD[idx] = D;

}

void Storage::writeSIR(){

	string historyFileName = "../Data/"+_HistoryDataFolderName+_timeStamp+"/HistoryData.dat";
	_HistoryDataFile.open(historyFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (_HistoryDataFile.is_open()){
		cout << "File "+historyFileName+" opened correctly." << endl;

		_HistoryDataFile << setw(6)  << "Index,";
		_HistoryDataFile << setw(15) << "Time,";
		_HistoryDataFile << setw(15) << "Suseptible,";
		_HistoryDataFile << setw(15) << "Infected,";
		_HistoryDataFile << setw(15) << "Symptomatic,";
		_HistoryDataFile << setw(15) << "Recovered,";
		_HistoryDataFile << setw(15) << "Dead,";
		_HistoryDataFile << endl;
		for (int i = 0; i<_Length; i++){
			_HistoryDataFile << setw(5)  <<    i  << ",";
			_HistoryDataFile << setw(14) << _TT[i] << ",";
			_HistoryDataFile << setw(14) << _SS[i] << ",";
			_HistoryDataFile << setw(14) << _II[i] << ",";
			_HistoryDataFile << setw(14) << _PP[i] << ",";
			_HistoryDataFile << setw(14) << _RR[i] << ",";
			_HistoryDataFile << setw(14) << _DD[i] << ",";
			_HistoryDataFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		_HistoryDataFile.close();
		cout << "File "+historyFileName+" closed." << endl;
	}

}
void Storage::startPersonDataSave(double t){
	string folder = "../Data/"+_PersonValueFolderName+_timeStamp;

	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	string movieFileName;
	stringstream ss(stringstream::in | stringstream::out);
	ss << setprecision(4) << t;
	string filename = ss.str();
	// TODO fix this for fractional times.
	if (t<10){
		filename = "00"+ filename;
	}else if(t<100){
		filename = "0"+ filename;
	}
	// movieFileName = "../Data/"+movieFolderName+"/Time"+filename+".dat";
	movieFileName = folder+"/Time"+filename+".dat";

	_PersonValueFile.open(movieFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (_PersonValueFile.is_open()){
		cout << "File "+movieFileName+" opened correctly." << endl;
		_PersonValueFile << setw(8) << "ID,";
		_PersonValueFile << setw(15) << "Time,";
		_PersonValueFile << setw(15) << "Age,";
		_PersonValueFile << setw(15) << "x,";
		_PersonValueFile << setw(15) << "y,";
		_PersonValueFile << setw(15) << "Location,";
		_PersonValueFile << setw(15) << "State,";
		_PersonValueFile << setw(17) << "SusceptibleCells,";
		_PersonValueFile << setw(17) << "InfectionLevel,";
		_PersonValueFile << setw(17) << "VirionLevel,";
		_PersonValueFile << endl;
	}

}
void Storage::endPersonDataSave(){
	cout << "Done writing to file. Closing..." << endl;
	_PersonValueFile.close();
	cout << "File "+_PersonValueFolderName+" closed." << endl;
}
void Storage::personDataSave(int ID, double t, double age, double coord[2], int locationID, char state, double suscells, double inflev, double virlev){
	double x;
	double y;
	x = coord[0];
	y = coord[1];

	if (_PersonValueFile.is_open()){
		_PersonValueFile << setw(7) << ID << ",";
		_PersonValueFile << setw(14) << t << ",";
		_PersonValueFile << setw(14) << age << ",";
		_PersonValueFile << setw(14) << x << ",";
		_PersonValueFile << setw(14) << y << ",";
		_PersonValueFile << setw(14) << locationID << ",";
		_PersonValueFile << setw(14) << state  << ",";
		_PersonValueFile << setw(16) << suscells << ",";
		_PersonValueFile << setw(16) << inflev << ",";
		_PersonValueFile << setw(16) << virlev << ",";
		_PersonValueFile << endl;
	}
}
int* Storage::getII(){
	return _II;
}
int* Storage::getRR(){
	return _RR;
}
int* Storage::getPP(){
	return _PP;
}
int* Storage::getSS(){
	return _SS;
}
int* Storage::getDD(){
	return _DD;
}

string Storage::listToString(list<int> l){

	ostringstream ss;


	for (auto v:l){
		ss << v << " ";
	}
	return ss.str();;
}

Storage::~Storage(){
	delete [] _II;
	delete [] _SS;
	delete [] _PP;
	delete [] _RR;
	delete [] _DD;
}
