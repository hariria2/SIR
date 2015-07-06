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
#include <string>
#include <list>

using namespace std;

class InHostDynamics {
public:
    
    InHostDynamics(int id, double ti);
    
    ~InHostDynamics();
    
    // setters
    void setID(int id);
    void setdt(int ti);
    void setBeta(double b);
    void setDelta(double d);
    void setP(double p);
    void setC(double c);
    
    // getters
    int getID();
    double getdt();
    double getBeta();
    double getDelta();
    double getP();
    double getC();
    
    // utilities
    void Simulate();
    void Update();
    void Flow();
    void Jacobian();
    
    
private:
    
    int ID;
    double dt;
    int t0;
    
    // state variables and derivatives
    double V;
    double I;
    double T;
    double dV;
    double dI;
    double dT;
    
    // parameters
    double Beta;
    double Delta;
    double P;
    double C;
    
    
};



#endif
