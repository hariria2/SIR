//
//  Economy.cpp
//  SIR
//
//  Created by Sahand Hariri on 8/9/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#include "Economy.h"


Economy::Economy(double a, double alpha, double beta, double Y0,double lc, double hc){

    setA(a);
    setAlpha(alpha);
    setBeta(beta);
    setLp(0);
    setLc(lc);
    setHc(hc);
    _dY = 0;
    _Y = Y0;
    _GDP = _Y;
    _Demand = 100/_GDP;
    
}

// Setters
void Economy::setA(double a){
    _A = a;
}
void Economy::setAlpha(double alpha){
    _Alpha = alpha;
}
void Economy::setBeta(double beta){
    _Beta = beta;
}
void Economy::setLp(double lp){
    _Lp = lp;
}
void Economy::setLc(double lc){
    _Lc = lc;
}
void Economy::setHp(double hp){
    _Hp = hp;
}
void Economy::setHc(double hc){
    _Hc = hc;
}


// Getters
double Economy::getA(){
    return _A;
}
double Economy::getAlpha(){
    return _Alpha;
}
double Economy::getBeta(){
    return _Beta;
}
double Economy::getGDP(){
    return _GDP;
}
double Economy::getDemand(){
    return _Demand;
}
void Economy::getParameters(vector<Person*> ppl){
    setLp(_Lc);
    setHp(_Hc);
    
    _Lc = ppl.size();
    
    double S_avg;
    double I_avg;
    double H_avg;
    
    for (auto ip = ppl.cbegin(); ip != ppl.cend(); ++ip){
        S_avg += ((*ip)->getInHostDynamics()).getT();
        I_avg += ((*ip)->getInHostDynamics()).getI();
    }
    
    I_avg = I_avg/_Lc;
    S_avg = S_avg/_Lc;
    
    H_avg = 1/(I_avg + 0.0001);
    
    _Hc = H_avg;
    
}

// Utilities

void Economy::computeGDP(vector<Person*> ppl, double oldGDP){
    
    unsigned long L = ppl.size();
    if (L == 0) {
        _GDP = oldGDP;
        if (_GDP == 0){
            _Demand = 1;
        }else {
            _Demand = .3/_GDP;
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
    
    
    _GDP = _A*pow(L,_Alpha)*pow(H_avg,_Beta);
      
    if (_GDP == 0){
        _Demand = 0.001;
    }else {
        _Demand = 0.001/_GDP;
    }
    
};

void Economy::Flow(){
    

    if (_Lp == 0 || _Hp == 0){
        _dY = 0;
    }else {
        _dY = _Alpha * _Y * (_Lc-_Lp)/_Lp + _Beta*_Y*(_Hc-_Hp)/_Hp;
    }
    
}

void Economy::Update(double dt){

    Flow();
    _Y = _Y + dt*_dY;
    _GDP = _Y;
    if (_GDP == 0){
        _Demand = 2.8;
    }else {
        _Demand = fmin(10/_GDP,2.8);
    }
    
}

