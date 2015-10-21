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
    
    InHostDynamics(int id, double ti, double sc, double ic, double vi, double tol);
    
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
    
    // getters
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
    
    int ID;
    double dt;
    int t0;
    
    // state variables and derivatives
    double V;
    double I;
    double T;
    double Ti;
    double dV;
    double dI;
    double dT;
    double MaxInfLev;
    double NE;     // Neighbor Effects
    
    // parameters
    double Beta;
    double Delta;
    double P;
    double C;
    double Tol;
    double ILRate;
    
    
    
};



#endif
