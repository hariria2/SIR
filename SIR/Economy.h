//
//  Economy.h
//  SIR
//
//  Created by Sahand Hariri on 8/9/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#ifndef __SIR__Economy__
#define __SIR__Economy__

#include <iostream>
#include "Person.h"

using namespace std;

class Economy {
public:
    
    Economy(double a, double alpha, double beta, double Y0, double lc, double hc);
   
    
    // Setters
    void setA(double a);
    void setAlpha(double alpha);
    void setBeta(double beta);
    void setLc(double lc);
    void setLp(double lp);
    void setHc(double hc);
    void setHp(double hp);
    void setY(double Y);
    
    
    // Getters
    double getA();
    double getAlpha();
    double getBeta();
    double getGDP();
    double getDemand();
    void getParameters(vector<Person*> ppl);
    
    
    
    
    // Utilities
    void computeGDP(vector<Person*> ppl, double oldGDP);
    void Flow();
    void Update(double dt);
    void Simulate();
    
private:
    
    double _A;
    double _Alpha;
    double _Beta;
    double _GDP = 0;
    double _Demand = 0;
    
    double _Lc;
    double _Lp;
    double _Hc;
    double _Hp;
    double _Y;
    double _dY;
    
    
    
    
};


#endif /* defined(__SIR__Economy__) */
