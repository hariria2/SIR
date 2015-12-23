#include "Domain.h"
#include "Place.h"
#include "Disease.h"
#include "InHostDynamics.h"
#include "Person.h"
#include "Architect.h"
#include "Storage.h"
#include "SQLStorage.h"
#include "Economy.h"

using namespace std;

// Function prototyping
template <typename T>
vector<T> operator+(const vector<T> &A, const vector<T> &B);
template <typename T>
vector<T> &operator+=(vector<T> &A, const vector<T> &B);

void GenerateHomes(vector<Place*> &homes, int perimeter[2][2], int homesize[2], Domain domain);
void getDefaultCoordinates(Place* location, double co[2]);
void readIslandData(string FileName, Domain *city, vector<Place*> &islands);
void readPeopleData();
void generateSourceData();
void updatePop(vector<Person*> p1, char s);

void FaroeIslands(bool SaveData=false);


//=================


// ========================= Main ======================
int main(){
    
    FaroeIslands();
    return 0;
 }
// ========================= End main =================


// ========================= Example Simulations ==============================
void FaroeIslands(bool SaveData){
    
    int maxdim = 115;
    int Boundary[2][2]   = {{0, 90},{0, 100}};
    Domain Island("Faroe", Boundary);
    
    vector<Place*> islands;
    
    readIslandData("../Source/Faroe2.csv", &Island, islands);
    
    
    int Pop_Str = 224;
    int Pop_Eys = 107;
    int Pop_Vag = 30;
    int Pop_Suo = 46;
    int Pop_San = 12;
    int Pop_Bor = 49;
    int Pop_Vio = 5;
    int Pop_Kun = 2;
    
    int population = Pop_Str+Pop_Eys+Pop_Vag+Pop_Suo+Pop_San+Pop_Bor+Pop_Vio+Pop_Kun;
    Disease flu("Flu", 44, 40, 100);
    char state = 'S';
    double VirLev = 0.0;
    
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    
    normal_distribution<double> ageDist(25,20);
    
    
    normal_distribution<double> icDist(2,0.01);
    normal_distribution<double> betaDist(0.5,0.01);
    normal_distribution<double> deltaDist(0.5,0);
    normal_distribution<double> PDist(.4,0);
    normal_distribution<double> CDist(0.8,0);
    normal_distribution<double> ILDist(0.0001,0.00001);
    
    vector<Person*> people;
    list<Person*> vpeople;
    
    for (int ii=1; ii <= population; ii++){
        string name = "randomName"+to_string(ii);
        double randage  = ageDist(generator);
        int age = (randage < 1)? 1:floor(randage);
        if (ii == 1){
            VirLev = 0.1;
        } else {
            VirLev = 0;
            state = 'S';
        }
        double randic  = icDist(generator);
        double ict = (randic < 0.5)? 0.5:randic;
        InHostDynamics ihd = InHostDynamics(ii,0.01,3,0.0,VirLev,ict);
        double randil = ILDist(generator);
        double il = (randil < 0.0005)? 0.0005:randil;
        ihd.setILRate(il);
        double randbeta  = betaDist(generator);
        double beta = (randbeta < 0)? 0:randbeta;
        ihd.setBeta(beta);
        double randdelta  = deltaDist(generator);
        double delta = (randdelta < 0)? 0:randdelta;
        ihd.setDelta(delta);
        double randP  = PDist(generator);
        double P = (randP < 0)? 0:randP;
        ihd.setP(P);
        double randC  = CDist(generator);
        double C = (randC < 0)? 0:randC;
        ihd.setC(C);
        
        for (auto p = islands.cbegin(); p != islands.cend(); ++p){
            if (ii <= Pop_Str){
                if ((*p)->getName()=="Streymoy"){
                    
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
            }
            else if (ii <= Pop_Str+Pop_Eys){
                if ((*p)->getName()=="Eysturoy"){
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
            }
            else if(ii <= Pop_Str+Pop_Eys+Pop_Vag){
                if ((*p)->getName()=="Vagar"){
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
            }
            else if(ii <= Pop_Str+Pop_Eys+Pop_Vag+Pop_Suo){
                if ((*p)->getName()=="Suouroy"){
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
            }
            else if(ii <= Pop_Str+Pop_Eys+Pop_Vag+Pop_Suo+Pop_San){
                if ((*p)->getName()=="Sandoy"){
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
            }
            else if(ii <= Pop_Str+Pop_Eys+Pop_Vag+Pop_Suo+Pop_San+Pop_Bor){
                if ((*p)->getName()=="Borooy"){
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
            }
            else if(ii <= Pop_Str+Pop_Eys+Pop_Vag+Pop_Suo+Pop_San+Pop_Bor+Pop_Vio){
                if ((*p)->getName()=="Viooy"){
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
            }
            else if(ii <= Pop_Str+Pop_Eys+Pop_Vag+Pop_Suo+Pop_San+Pop_Bor+Pop_Vio+Pop_Kun){
                if ((*p)->getName()=="Kunoy"){
                    Person *ip = new Person(ii, name, age, state, flu, ihd,
                                            &Island, (*p),islands,10,10,10);
                    people.push_back(ip);
                    vpeople.push_back(ip);
                }
    
            }
            
        }
        
    };
    
    double InitialTime = 0;
    double EndTime = 5000;
    double TimeStep = 1;
    //int l = floor((EndTime-InitialTime)/TimeStep);
    string ver = "3";
    //cout << "Enter version number for multi location simulation: ";
    //cin >> ver;
    
    int xdim = maxdim;
    int ydim = maxdim;
    Visualization* vis = getVisualization(xdim, ydim, true);
    vis->setPlaces(islands);
    vis->setPeople(vpeople);
    
    SQLStorage sqldata("localhost", "root", "", "anchorDB", ver);
    //Architect archie(InitialTime,EndTime,TimeStep, people,  "MYSQL", &sqldata);
    
    Architect archie(InitialTime,EndTime,TimeStep, vpeople, vis, "MYSQL", &sqldata);
    
    //Architect archie(InitialTime,EndTime,TimeStep, people, vis);
    vis->Init();
    vis->setArchitect(&archie);
    //vis->RenderSplash();
    archie.setDomain(&Island);
    archie.setPlaces(islands);
    archie.Simulate();
}

// ========================= End Examples ======================================

// ========================= Utilities ===========================================

template <typename T>
vector<T> operator+(const vector<T> &A, const vector<T> &B)
{
    std::vector<T> AB;
    AB.reserve( A.size() + B.size() );                // preallocate memory
    AB.insert( AB.end(), A.begin(), A.end() );        // add A;
    AB.insert( AB.end(), B.begin(), B.end() );        // add B;
    return AB;
}

template <typename T>
vector<T> &operator+=(vector<T> &A, const vector<T> &B)
{
    A.reserve( A.size() + B.size() );                // preallocate memory without erasing original data
    A.insert( A.end(), B.begin(), B.end() );         // add B;
    return A;                                        // here A could be named AB
}

void GenerateHomes(vector<Place*> &homes, int perimeter[2][2], int homesize[2], Domain domain){
 
	int xdist  = homesize[1];
	int ydist  = homesize[2];
 
	int xrange = perimeter[0][1] - perimeter[0][0];
	int yrange = perimeter[1][1] - perimeter[1][0];
 
	int xnum = floor((xrange/xdist)/2);
	int ynum = floor((yrange/ydist)/2);
 
	int total = xnum*ynum;
	int homedimx[total][2];
	int homedimy[total][2];
 
	int x[2] = {perimeter[0][0],perimeter[0][0]+xdist};
	int y[2] = {static_cast<int>(perimeter[1][0]+floor(ydist/2)),static_cast<int>(perimeter[1][0]+ydist+floor(ydist/2))};
	int n = 0;
 
	for (int i = 0; i < xnum; i++){
        for ( int j = 0; j < ynum; j++){
            homedimx[n][0] = x[0];
            homedimx[n][1] = x[1];
            homedimy[n][0] = y[0];
            homedimy[n][1] = y[1];
            for (int i = 0; i < 2; i++){
                y[i] = y[i] + 2*ydist;
            }
            n += 1;
        }
        y[0] = perimeter[1][0]+floor(ydist/2);
        y[1] = perimeter[1][0]+ydist+floor(ydist/2);
        for (int i = 0; i < 2; i++){
            x[i] = x[i] + 2*xdist;
        }
 
    }
 
	double perim[2][2];
	for (int i=0; i<total; i++){
        for (int n=0;n<2;n++){
            perim[0][n] = homedimx[i][n];
            perim[1][n] = homedimy[i][n];
        }
 Place* p = new Place(i, "home", "Home", perim, domain);
 homes.push_back(p);
	}
 }
void getDefaultCoordinates(Place* location, double co[2]){
    
    double x = fmod(rand(),(location->Perimeter[0][1] - location->Perimeter[0][0] + 1)) + location->Perimeter[0][0];
    double y = fmod(rand(),(location->Perimeter[1][1] - location->Perimeter[1][0] + 1)) + location->Perimeter[1][0];
    co[0] = x;
    co[1] = y;
}
void readIslandData(string FileName, Domain *city, vector<Place*> &islands){
    ifstream File;
    File.open(FileName, ios_base::in);
    
    if (File.is_open()){
        cout << "Island File opened correctly." << endl;
        
        string sID;
        string name;
        string type;
        string sxmin;
        string sxmax;
        string symin;
        string symax;
    
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
            
            getline(File, symax, '\r');
            double ymax = atof(symax.c_str());
            
            
            double boundary[2][2] = {{xmin, xmax},{ymin, ymax}};
            
            Place *h = new Place(ID, name, type, boundary, *city);
            islands.push_back(h);
            
        }
        cout << "Finished reading the file. Will close now. " << endl;
        File.close();
    } else {
        cout << "File did not open correctly." << endl;
    }
};

void readPeopleData(){
    
}
void generateSourceData(){
    
}
void updatePop(vector<Person*> p1, char s){
    for (auto ip = p1.cbegin(); ip != p1.cend(); ++ip){
        if ((*ip)->getState() != s){
            (*ip)->setState(s);
            break;
        }
    }
}
// ========================= End utilities ======================================