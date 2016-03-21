//
//  InHostDynamics.h
//  SIR
//
//  Created by Sahand Hariri on 6/30/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#ifndef SIR_InHostDynamics_h
#define SIR_InHostDynamics_h

#include <iostream>
#include <cmath>

using namespace std;

class InHostDynamics {
public:
    
    InHostDynamics(int id, double ti, double sc, double ic, double vi, double tol,int incubationPeriod, int infectionPeriod, int recoveryPeriod);
    
    ~InHostDynamics();
    
    // setters
    void setID(int id);
    void setdt(double ti);
    void setT0(int it);
    void setBeta(double b);
    void setDelta(double d);
    void setP(double p);
    void setC(double c);
    void setT(double sc);
    void setTi(double sc);
    void setI(double ic);
    void setV(double vi);
    void setMaxInfLev(double mil);
    void setNE(double ne);
    void setTol(double tol);
    void setILRate(double ilrate);
    
    void setAverageInfectionPeriod(int x);
    void setAverageIncubationPeriod(int x);
    void setAverageRecoveryPeriod(int x);
    
    // getters
    int getAverageInfectionPeriod();
    int getAverageIncubationPeriod();
    int getAverageRecoveryPeriod();
    
   
    int getID();
    double getdt();
    double getT0();
    double getBeta();
    double getDelta();
    double getP();
    double getC();
    double getT();
    double getTi();
    double getI();
    double getV();
    double getMaxInfLev();
    double getNE();
    double getTol();
    double getILRate();
    
    
    // utilities
    void Simulate();
    void Update();
    void Flow();
    void Jacobian();
    
    int HasBeenSick = 0;
    
private:
    
    int _ID;
    double _dt;
    int _t0;
    
    int _AverageInfectionPeriod;
    int _AverageIncubationPeriod;
    int _AverageRecoveryPeriod;
    
    // state variables and derivatives
    double _V;
    double _I;
    double _T;
    double _Ti;
    double _dV;
    double _dI;
    double _dT;
    double _MaxInfLev;
    double _NE;     // Neighbor Effects
    
    // parameters
    double _Beta;
    double _Delta;
    double _P;
    double _C;
    double _Tol;
    double _ILRate;
    
    
    
};



#endif
