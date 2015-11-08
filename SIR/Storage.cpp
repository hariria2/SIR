/*
 * Storage.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: Sahand
 */

#include "Storage.h"


Storage::Storage(int len, Domain* city, vector<Place*> homes, vector<Place*> works, vector<Place*> schools, vector<Place*> cemeteries,
		  string datafoldername, string moviefoldername):
    _Length(len),
    _CityPtr(city),
    _Homes(homes),
    _Works(works),
    _Schools(schools),
    _Cemeteries(cemeteries),
    _dataFolderName(datafoldername),
    _movieFolderName(moviefoldername)
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
    
	string historyFileName = "../Data/"+_dataFolderName+_timeStamp+"/historyData.dat";
	_historyFile.open(historyFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (_historyFile.is_open()){
		cout << "File "+historyFileName+" opened correctly." << endl;

		_historyFile << setw(6)  << "Index,";
		_historyFile << setw(15) << "Time,";
		_historyFile << setw(15) << "Suseptible,";
		_historyFile << setw(15) << "Infected,";
        _historyFile << setw(15) << "Symptomatic,";
		_historyFile << setw(15) << "Recovered,";
        _historyFile << setw(15) << "Dead,";
		_historyFile << endl;
		for (int i = 0; i<_Length; i++){
			_historyFile << setw(5)  <<     i  << ",";
			_historyFile << setw(14) << _TT[i] << ",";
			_historyFile << setw(14) << _SS[i] << ",";
            _historyFile << setw(14) << _II[i] << ",";
			_historyFile << setw(14) << _PP[i] << ",";
			_historyFile << setw(14) << _RR[i] << ",";
            _historyFile << setw(14) << _DD[i] << ",";
			_historyFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		_historyFile.close();
		cout << "File "+historyFileName+" closed." << endl;
	}

}
void Storage::startMovieSave(double t){
	string folder = "../Data/"+_movieFolderName+_timeStamp;

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

	_movieFile.open(movieFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (_movieFile.is_open()){
		cout << "File "+movieFileName+" opened correctly." << endl;
		_movieFile << setw(8) << "ID,";
		_movieFile << setw(15) << "Name,";
		_movieFile << setw(15) << "Time,";
		_movieFile << setw(15) << "x,";
		_movieFile << setw(15) << "y,";
		_movieFile << setw(15) << "Location,";
        _movieFile << setw(15) << "State,";
        _movieFile << setw(15) << "HasBeenSick,";
        _movieFile << setw(17) << "SusceptibleCells,";
        _movieFile << setw(17) << "InfectionLevel,";
        _movieFile << setw(17) << "VirionLevel,";
        _movieFile << setw(17) << "MaxInfLev,";
        _movieFile << setw(30) << "SIConnections,";
        _movieFile << setw(30) << "SIConnectionsHist,";
        _movieFile << setw(30) << "AllConnections,";
        _movieFile << setw(30) << "AllConnectionsHist,";
		_movieFile << endl;
	}

}
void Storage::endMovieSave(){
	cout << "Done writing to file. Closing..." << endl;
	_movieFile.close();
	cout << "File "+_movieFolderName+" closed." << endl;
}
void Storage::movieSave(int ID, string name, double t, double coord[2], string location, char state, bool hbs, double inflev, double suscells, double virlev, double maxInfLev, list<int> SIconnections, list<int> SIconnectionsHist, list<int> Allconnections, list<int> AllconnectionsHist){
	double x;
	double y;
	x = coord[0];
	y = coord[1];
   
    
	if (_movieFile.is_open()){
		_movieFile << setw(7) << ID << ",";
		_movieFile << setw(14) << name << ",";
		_movieFile << setw(14) << t << ",";
		_movieFile << setw(14) << x << ",";
		_movieFile << setw(14) << y << ",";
		_movieFile << setw(14) << location << ",";
		_movieFile << setw(14) << state  << ",";
        _movieFile << setw(14) << hbs << ",";
        _movieFile << setw(16) << inflev << ",";
        _movieFile << setw(16) << suscells << ",";
        _movieFile << setw(16) << virlev << ",";
        _movieFile << setw(16) << maxInfLev << ",";
        _movieFile << setw(30) << "[" << listToString(SIconnections) << "]" << ",";
        _movieFile << setw(30) << "[" << listToString(SIconnectionsHist) << "]" << ",";
        _movieFile << setw(30) << "[" << listToString(Allconnections) << "]" << ",";
        _movieFile << setw(30) << "[" << listToString(AllconnectionsHist) << "]" << ",";
		_movieFile << endl;
	}
}
void Storage::citySave(){
	string folder = "../Data/"+_dataFolderName+_timeStamp;
    
	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	_cityFileName = folder+"/cityData.dat";
    
	_cityFile.open(_cityFileName, ios_base::out|ios_base::app);
    
    cout << "Thank you!" << endl;
	if (_cityFile.is_open()){
		cout << "File "+_cityFileName+" opened correctly." << endl;
		_cityFile << setw(15) << "Name,";
		_cityFile << setw(15) << "xmin,";
		_cityFile << setw(15) << "xmax,";
		_cityFile << setw(15) << "ymin,";
		_cityFile << setw(15) << "ymax,";
		_cityFile << endl;

		_cityFile << setw(14) << _CityPtr->getName() << ",";
		_cityFile << setw(14) << (_CityPtr->Boundary)[0][0] << ",";
		_cityFile << setw(14) << (_CityPtr->Boundary)[0][1] << ",";
		_cityFile << setw(14) << (_CityPtr->Boundary)[1][0] << ",";
		_cityFile << setw(14) << (_CityPtr->Boundary)[1][1] << ",";
		_cityFile << endl;

		cout << "Done writing to file. Closing..." << endl;
		_cityFile.close();
		cout << "File "+_cityFileName+" closed." << endl;
	}

}
void Storage::homeSave(){
	string folder = "../Data/"+_dataFolderName+_timeStamp;

	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	_homeFileName = folder+"/homeData.dat";
	_homeFile.open(_homeFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;

	if (_homeFile.is_open()){
		cout << "File "+_homeFileName+" opened correctly." << endl;
		_homeFile << setw(15) << "Name,";
		_homeFile << setw(15) << "Type,";
		_homeFile << setw(15) << "xmin,";
		_homeFile << setw(15) << "xmax,";
		_homeFile << setw(15) << "ymin,";
		_homeFile << setw(15) << "ymax,";
		_homeFile << endl;

		for(auto h = _Homes.cbegin(); h != _Homes.cend(); ++h) {
			_homeFile << setw(14) <<  (*h)->getName() << ",";
			_homeFile << setw(14) <<  (*h)->getType() << ",";
			_homeFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
			_homeFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
			_homeFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
			_homeFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
			_homeFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		_homeFile.close();
		cout << "File "+_homeFileName+" closed." << endl;
	}
}
void Storage::workSave(){
	string folder = "../Data/"+_dataFolderName+_timeStamp;

	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	_workFileName = folder+"/workData.dat";
	_workFile.open(_workFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (_workFile.is_open()){
		cout << "File "+_workFileName+" opened correctly." << endl;
		_workFile << setw(15) << "Name,";
		_workFile << setw(15) << "Type,";
		_workFile << setw(15) << "xmin,";
		_workFile << setw(15) << "xmax,";
		_workFile << setw(15) << "ymin,";
		_workFile << setw(15) << "ymax,";
		_workFile << endl;

		for(auto h = _Works.cbegin(); h != _Works.cend(); ++h) {
			_workFile << setw(14) << (*h)->getName() << ",";
			_workFile << setw(14) << (*h)->getType() << ",";
			_workFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
			_workFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
			_workFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
			_workFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
			_workFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		_workFile.close();
		cout << "File "+_workFileName+" closed." << endl;
	}
}
void Storage::schoolSave(){
	string folder = "../Data/"+_dataFolderName+_timeStamp;

	mode_t process_mask = umask(0);
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(process_mask);

	_schoolFileName = folder+"/schoolData.dat";
	_schoolFile.open(_schoolFileName, ios_base::out|ios_base::app);
	cout << "Thank you!" << endl;
	if (_schoolFile.is_open()){
		cout << "File "+_schoolFileName+" opened correctly." << endl;
		_schoolFile << setw(15) << "Name,";
		_schoolFile << setw(15) << "Type,";
		_schoolFile << setw(15) << "xmin,";
		_schoolFile << setw(15) << "xmax,";
		_schoolFile << setw(15) << "ymin,";
		_schoolFile << setw(15) << "ymax,";
		_schoolFile << endl;

		for(auto h = _Schools.cbegin(); h != _Schools.cend(); ++h) {
			_schoolFile << setw(14) << (*h)->getName() << ",";
			_schoolFile << setw(14) << (*h)->getType() << ",";
			_schoolFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
			_schoolFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
			_schoolFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
			_schoolFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
			_schoolFile << endl;
		}
		cout << "Done writing to file. Closing..." << endl;
		_schoolFile.close();
		cout << "File "+_schoolFileName+" closed." << endl;
	}
}
void Storage::cemeterySave(){
    string folder = "../Data/"+_dataFolderName+_timeStamp;
    
    mode_t process_mask = umask(0);
    mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    umask(process_mask);
    
    _cemeteryFileName = folder+"/cemeteryData.dat";
    _cemeteryFile.open(_cemeteryFileName, ios_base::out|ios_base::app);
    cout << "Thank you!" << endl;
    if (_cemeteryFile.is_open()){
        cout << "File "+_schoolFileName+" opened correctly." << endl;
        _cemeteryFile << setw(15) << "Name,";
        _cemeteryFile << setw(15) << "Type,";
        _cemeteryFile << setw(15) << "xmin,";
        _cemeteryFile << setw(15) << "xmax,";
        _cemeteryFile << setw(15) << "ymin,";
        _cemeteryFile << setw(15) << "ymax,";
        _cemeteryFile << endl;
        
        for(auto h = _Cemeteries.cbegin(); h != _Cemeteries.cend(); ++h) {
            _cemeteryFile << setw(14) << (*h)->getName() << ",";
            _cemeteryFile << setw(14) << (*h)->getType() << ",";
            _cemeteryFile << setw(14) << ((*h)->Perimeter)[0][0] << ",";
            _cemeteryFile << setw(14) << ((*h)->Perimeter)[0][1] << ",";
            _cemeteryFile << setw(14) << ((*h)->Perimeter)[1][0] << ",";
            _cemeteryFile << setw(14) << ((*h)->Perimeter)[1][1] << ",";
            _cemeteryFile << endl;
        }
        cout << "Done writing to file. Closing..." << endl;
        _cemeteryFile.close();
        cout << "File "+_cemeteryFileName+" closed." << endl;
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