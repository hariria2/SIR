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
#include "Storage.h"
using namespace std;


Storage::Storage(int len, Domain* city, vector<Place*> homes, vector<Place*> works, vector<Place*> schools, vector<Place*> cemeteries,
		  string datafoldername, string moviefoldername)
		: Length(len), CityPtr(city), Homes(homes), Works(works), Schools(schools), Cemeteries(cemeteries),
		  dataFolderName(datafoldername), movieFolderName(moviefoldername)
{
	TT = new double[Length];
	II = new int[Length];
    PP = new int[Length];
	SS = new int[Length];
	RR = new int[Length];
    DD = new int[Length];
    XX = new int[Length];

    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    timeStamp = to_string((now->tm_mon + 1)) + "-" +
                to_string(now->tm_mday) + "-" +
                to_string((now->tm_year + 1900));
    
}

void Storage::saveSIR(int idx, double t, int S, int I, int P, int R, int D){
	TT[idx] = t;
	II[idx] = I;
    PP[idx] = P;
	SS[idx] = S;
	RR[idx] = R;
    DD[idx] = D;
    
}

void Storage::writeSIR(){
    
	string historyFileName = "../Data/"+dataFolderName+timeStamp+"/historyData.dat";
	historyFile.open(historyFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (historyFile.is_open()){
		cout << "File "+historyFileName+" opened correctly." << endl;

		historyFile << setw(6)  << "Index,";
		historyFile << setw(15) << "Time,";
		historyFile << setw(15) << "Suseptible,";
		historyFile << setw(15) << "Infected,";
        historyFile << setw(15) << "Symptomatic,";
		historyFile << setw(15) << "Recovered,";
        historyFile << setw(15) << "Dead,";
		historyFile << endl;
		for (int i = 0; i<Length; i++){
			historyFile << setw(5)  <<    i  << ",";
			historyFile << setw(14) << TT[i] << ",";
			historyFile << setw(14) << SS[i] << ",";
            historyFile << setw(14) << II[i] << ",";
			historyFile << setw(14) << PP[i] << ",";
			historyFile << setw(14) << RR[i] << ",";
            historyFile << setw(14) << DD[i] << ",";
			historyFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		historyFile.close();
		cout << "File "+historyFileName+" closed." << endl;
	}

}
void Storage::startMovieSave(double t){
	string folder = "../Data/"+movieFolderName+timeStamp;

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

	movieFile.open(movieFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (movieFile.is_open()){
		cout << "File "+movieFileName+" opened correctly." << endl;
		movieFile << setw(8) << "ID,";
		movieFile << setw(15) << "Name,";
		movieFile << setw(15) << "Time,";
		movieFile << setw(15) << "x,";
		movieFile << setw(15) << "y,";
		movieFile << setw(15) << "Location,";
        movieFile << setw(15) << "State,";
        movieFile << setw(15) << "HasBeenSick,";
        movieFile << setw(17) << "SusceptibleCells,";
        movieFile << setw(17) << "InfectionLevel,";
        movieFile << setw(17) << "VirionLevel,";
        movieFile << setw(17) << "MaxInfLev,";
        movieFile << setw(30) << "SIConnections,";
        movieFile << setw(30) << "SIConnectionsHist,";
        movieFile << setw(30) << "AllConnections,";
        movieFile << setw(30) << "AllConnectionsHist,";
		movieFile << endl;
	}

}
void Storage::endMovieSave(){
	cout << "Done writing to file. Closing..." << endl;
	movieFile.close();
	cout << "File "+movieFolderName+" closed." << endl;
}
void Storage::movieSave(int ID, string name, double t, double coord[2], string location, char state, bool hbs, double inflev, double suscells, double virlev, double maxInfLev, list<int> SIconnections, list<int> SIconnectionsHist, list<int> Allconnections, list<int> AllconnectionsHist){
	double x;
	double y;
	x = coord[0];
	y = coord[1];
   
    
	if (movieFile.is_open()){
		movieFile << setw(7) << ID << ",";
		movieFile << setw(14) << name << ",";
		movieFile << setw(14) << t << ",";
		movieFile << setw(14) << x << ",";
		movieFile << setw(14) << y << ",";
		movieFile << setw(14) << location << ",";
		movieFile << setw(14) << state  << ",";
        movieFile << setw(14) << hbs << ",";
        movieFile << setw(16) << inflev << ",";
        movieFile << setw(16) << suscells << ",";
        movieFile << setw(16) << virlev << ",";
        movieFile << setw(16) << maxInfLev << ",";
        movieFile << setw(30) << "[" << listToString(SIconnections) << "]" << ",";
        movieFile << setw(30) << "[" << listToString(SIconnectionsHist) << "]" << ",";
        movieFile << setw(30) << "[" << listToString(Allconnections) << "]" << ",";
        movieFile << setw(30) << "[" << listToString(AllconnectionsHist) << "]" << ",";
		movieFile << endl;
	}
}
void Storage::citySave(){
	string folder = "../Data/"+dataFolderName+timeStamp;
    
	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	cityFileName = folder+"/cityData.dat";
    
	cityFile.open(cityFileName, ios_base::out|ios_base::app);
    
    cout << "Thank you!" << endl;
	if (cityFile.is_open()){
		cout << "File "+cityFileName+" opened correctly." << endl;
		cityFile << setw(15) << "Name,";
		cityFile << setw(15) << "xmin,";
		cityFile << setw(15) << "xmax,";
		cityFile << setw(15) << "ymin,";
		cityFile << setw(15) << "ymax,";
		cityFile << endl;

		cityFile << setw(14) << CityPtr->getName() << ",";
		cityFile << setw(14) << (CityPtr->Boundary)[0][0] << ",";
		cityFile << setw(14) << (CityPtr->Boundary)[0][1] << ",";
		cityFile << setw(14) << (CityPtr->Boundary)[1][0] << ",";
		cityFile << setw(14) << (CityPtr->Boundary)[1][1] << ",";
		cityFile << endl;

		cout << "Done writing to file. Closing..." << endl;
		cityFile.close();
		cout << "File "+cityFileName+" closed." << endl;
	}

}
void Storage::homeSave(){
	string folder = "../Data/"+dataFolderName+timeStamp;

	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	homeFileName = folder+"/homeData.dat";
	homeFile.open(homeFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;

	if (homeFile.is_open()){
		cout << "File "+homeFileName+" opened correctly." << endl;
		homeFile << setw(15) << "Name,";
		homeFile << setw(15) << "Type,";
		homeFile << setw(15) << "xmin,";
		homeFile << setw(15) << "xmax,";
		homeFile << setw(15) << "ymin,";
		homeFile << setw(15) << "ymax,";
		homeFile << endl;

		for(auto h = Homes.cbegin(); h != Homes.cend(); ++h) {
			homeFile << setw(14) <<  (*h)->getName() << ",";
			homeFile << setw(14) <<  (*h)->getType() << ",";
			homeFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
			homeFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
			homeFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
			homeFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
			homeFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		homeFile.close();
		cout << "File "+homeFileName+" closed." << endl;
	}
}
void Storage::workSave(){
	string folder = "../Data/"+dataFolderName+timeStamp;

	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	workFileName = folder+"/workData.dat";
	workFile.open(workFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (workFile.is_open()){
		cout << "File "+workFileName+" opened correctly." << endl;
		workFile << setw(15) << "Name,";
		workFile << setw(15) << "Type,";
		workFile << setw(15) << "xmin,";
		workFile << setw(15) << "xmax,";
		workFile << setw(15) << "ymin,";
		workFile << setw(15) << "ymax,";
		workFile << endl;

		for(auto h = Works.cbegin(); h != Works.cend(); ++h) {
			workFile << setw(14) << (*h)->getName() << ",";
			workFile << setw(14) << (*h)->getType() << ",";
			workFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
			workFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
			workFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
			workFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
			workFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		workFile.close();
		cout << "File "+workFileName+" closed." << endl;
	}
}
void Storage::schoolSave(){
	string folder = "../Data/"+dataFolderName+timeStamp;

	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	schoolFileName = folder+"/schoolData.dat";
	schoolFile.open(schoolFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (schoolFile.is_open()){
		cout << "File "+schoolFileName+" opened correctly." << endl;
		schoolFile << setw(15) << "Name,";
		schoolFile << setw(15) << "Type,";
		schoolFile << setw(15) << "xmin,";
		schoolFile << setw(15) << "xmax,";
		schoolFile << setw(15) << "ymin,";
		schoolFile << setw(15) << "ymax,";
		schoolFile << endl;

		for(auto h = Schools.cbegin(); h != Schools.cend(); ++h) {
			schoolFile << setw(14) << (*h)->getName() << ",";
			schoolFile << setw(14) << (*h)->getType() << ",";
			schoolFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
			schoolFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
			schoolFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
			schoolFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
			schoolFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		schoolFile.close();
		cout << "File "+schoolFileName+" closed." << endl;
	}
}
void Storage::cemeterySave(){
    string folder = "../Data/"+dataFolderName+timeStamp;
    
    mode_t process_mask = umask(0);
    mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    umask(process_mask);
    
    cemeteryFileName = folder+"/cemeteryData.dat";
    cemeteryFile.open(cemeteryFileName, ios_base::out|ios_base::app);
    cout << "Thank you!" << endl;
    if (cemeteryFile.is_open()){
        cout << "File "+schoolFileName+" opened correctly." << endl;
        cemeteryFile << setw(15) << "Name,";
        cemeteryFile << setw(15) << "Type,";
        cemeteryFile << setw(15) << "xmin,";
        cemeteryFile << setw(15) << "xmax,";
        cemeteryFile << setw(15) << "ymin,";
        cemeteryFile << setw(15) << "ymax,";
        cemeteryFile << endl;
        
        for(auto h = Cemeteries.cbegin(); h != Cemeteries.cend(); ++h) {
            cemeteryFile << setw(14) << (*h)->getName() << ",";
            cemeteryFile << setw(14) << (*h)->getType() << ",";
            cemeteryFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
            cemeteryFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
            cemeteryFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
            cemeteryFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
            cemeteryFile << endl;
        }
        cout << "Done writing to file. Closing..." << endl;
        cemeteryFile.close();
        cout << "File "+cemeteryFileName+" closed." << endl;
    }
}

int* Storage::getII(){
	return II;
}
int* Storage::getRR(){
	return RR;
}
int* Storage::getPP(){
    return PP;
}
int* Storage::getSS(){
	return SS;
}
int* Storage::getDD(){
    return DD;
}

string Storage::listToString(list<int> l){
    
    ostringstream ss;
    
    
    for (auto v:l){
        ss << v << " ";
    }
    return ss.str();;
}

Storage::~Storage(){
	delete [] II;
	delete [] SS;
    delete [] PP;
	delete [] RR;
    delete [] DD;
}