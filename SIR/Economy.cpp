//
//  Economy.cpp
//  SIR
//
//  Created by Sahand Hariri on 8/9/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#include <iostream>
#include <string>
#include <cmath>
#include <list>
#include <vector>
#include <random>
#include <ctime>
#include "Economy.h"
#include "Person.h"


Economy::Economy(double a, double alpha, double beta, double Y0,unsigned long lc, double hc){

    setA(a);
    setAlpha(alpha);
    setBeta(beta);
    setLc(lc);
    setHc(hc);
    Y = Y0;
    GDP = Y;
    Demand = 100/GDP;
    
}

// Setters
void Economy::setA(double a){
    A = a;
}
void Economy::setAlpha(double alpha){
    Alpha = alpha;
}
void Economy::setBeta(double beta){
    Beta = beta;
}
void Economy::setLp(unsigned long lp){
    Lp = lp;
}
void Economy::setLc(unsigned long lc){
    Lc = lc;
}
void Economy::setHp(double hp){
    Hp = hp;
}
void Economy::setHc(double hc){
    Hc = hc;
}


// Getters
double Economy::getA(){
    return A;
}
double Economy::getAlpha(){
    return Alpha;
}
double Economy::getBeta(){
    return Beta;
}
double Economy::getGDP(){
    return GDP;
}
double Economy::getDemand(){
    return Demand;
}
void Economy::getParameters(vector<Person*> ppl){
    setLp(Lc);
    setHp(Hc);
    
    Lc = ppl.size();
    
    double S_avg;
    double I_avg;
    double H_avg;
    
    for (auto ip = ppl.cbegin(); ip != ppl.cend(); ++ip){
        S_avg += ((*ip)->getInHostDynamics()).getT();
        I_avg += ((*ip)->getInHostDynamics()).getI();
    }
    
    I_avg = I_avg/Lc;
    S_avg = S_avg/Lc;
    
    H_avg = 1/(I_avg + 0.0001);
    
    Hc = H_avg;
    
}

// Utilities

void Economy::computeGDP(vector<Person*> ppl, double oldGDP){
    
    unsigned long L = ppl.size();
    if (L == 0) {
        GDP = oldGDP;
        if (GDP == 0){
            Demand = 1;
        }else {
            Demand = .3/GDP;
        }
        return;
    }
    double S_avg;
    double I_avg;
    double H_avg;
    
    for (auto ip = ppl.cbegin(); ip != ppl.cend(); ++ip){
        S_avg += ((*ip)->getInHostDynamics()).getT();
        I_avg += ((*ip)->getInHostDynamics()).getI();
    }
    
    I_avg = I_avg/L;
    S_avg = S_avg/L;
    
    H_avg = 1/(I_avg + 0.0001);
    
    
    GDP = A*pow(L,Alpha)*pow(H_avg,Beta);
      
    if (GDP == 0){
        Demand = 0.001;
    }else {
        Demand = 0.001/GDP;
    }
    
};

void Economy::Flow(){
    if (Lp == 0 || Hp == 0){
        dY = 0;
    }else {
        dY = Alpha * Y * (Lc-Lp)/Lp + Beta*Y*(Hc-Hp)/Hp;
    }
}

void Economy::Update(double dt){
    Flow();
    Y = Y + dt*dY;
    GDP = Y;
    if (GDP == 0){
        Demand = 0.001;
    }else {
        Demand = 100/GDP;
    }
    
}

