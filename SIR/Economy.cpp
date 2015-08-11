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


Economy::Economy(double a, double alpha, double beta){

    setA(a);
    setAlpha(alpha);
    setBeta(beta);
    
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

// Utilities

void Economy::computeGDP(vector<Person*> ppl, double oldGDP){
    
    unsigned long L = ppl.size();
    if (L == 0) {
        GDP = oldGDP;
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
    
//    if (I_avg < 1e-100){
//        H_avg = S_avg/0.001;
//        cout << "I'm here " << endl;
//    } else {
//        H_avg = S_avg/I_avg;
//        cout << "here is where I think I should be" << endl;
//    }
//
    H_avg = 1/(I_avg + 0.0001);
    
    
    GDP = A*pow(L,Alpha)*pow(H_avg,Beta);
    
//    if (GDP == 0){
//        Demand = 10;
//    }else {
//        Demand = 1/GDP;
//    }
    
};

