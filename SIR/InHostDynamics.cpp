//
//  InHostDynamics.cpp
//  SIR
//
//  Created by Sahand Hariri on 6/30/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#include "InHostDynamics.h"

InHostDynamics::InHostDynamics(int id, double ti, double sc, double ic, double vi, double tol){
    setID(id);
    setdt(ti);
    setT(sc);
    setI(ic);
    setV(vi);
    setTi(sc);
    setTol(tol);
}

// setters
void InHostDynamics::setID(int id){
    _ID = id;
}
void InHostDynamics::setdt(double ti){
    _dt = ti;
}
void InHostDynamics::setT0(int it){
    _t0 = it;
}
void InHostDynamics::setBeta(double b){
    _Beta = b;
}
void InHostDynamics::setDelta(double d){
    _Delta = d;
}
void InHostDynamics::setP(double p){
    _P = p;
}
void InHostDynamics::setC(double c){
    _C = c;
}
void InHostDynamics::setT(double sc){
    _T  = sc;
}
void InHostDynamics::setTi(double sc){
    _Ti = sc;
}
void InHostDynamics::setI(double ic){
    _I = ic;
}
void InHostDynamics::setV(double vi){
    _V = vi;
}
void InHostDynamics::setMaxInfLev(double mil){
    _MaxInfLev = mil;
}
void InHostDynamics::setNE(double ne){
    _NE = ne;
}
void InHostDynamics::setTol(double tol){
    _Tol = tol;
}
void InHostDynamics::setILRate(double ilrate){
    _ILRate = ilrate;
}

// getters
int InHostDynamics::getID(){
    return _ID;
}
double InHostDynamics::getdt(){
    return _dt;
}
double InHostDynamics::getT0(){
    return _t0;
}
double InHostDynamics::getBeta(){
    return _Beta;
}
double InHostDynamics::getDelta(){
    return _Delta;
}
double InHostDynamics::getP(){
    return _P;
}
double InHostDynamics::getC(){
    return _C;
}
double InHostDynamics::getT(){
    return _T;
}
double InHostDynamics::getTi(){
    return _Ti;
}
double InHostDynamics::getI(){
    return _I;
}
double InHostDynamics::getV(){
    return _V;
}
double InHostDynamics::getMaxInfLev(){
    return _MaxInfLev;
}
double InHostDynamics::getNE(){
    return _NE;
}
double InHostDynamics::getTol(){
    return _Tol;
}
double InHostDynamics::getILRate(){
    return _ILRate;
}

//utilities
void InHostDynamics::Simulate(){
    for (double tt = _t0; tt <= _t0+1; tt = tt+_dt){
        Update();
        if (getI() > getMaxInfLev()){
            setMaxInfLev(getI());
        }
    }
    
}

void InHostDynamics::Update(){
    
    Flow();
    
    if (isnan(_V)){
        cout << "V: " << _V << endl;
        cout << "I: " << _I << endl;
        cout << "T: " << _T << endl;
        cout << "dV: " << _dV << endl;
        cout << "dI: " << _dI << endl;
        cout << "dT: " << _dT << endl;
        exit(1);
    }
    
    
    _T = _T + _dt*_dT;
    _I = _I + _dt*_dI;
    _V = _V + _dt*_dV;
    
    
}

void InHostDynamics::Flow(){
    
    _NE = (_NE > 5)? 5:_NE;
    if (_T<_Tol){
        if (!HasBeenSick){
            _dT = _ILRate;
            _dI = 0.0;
            _dV = 0.0;
        } else{
            _dT = -_Beta*_T*_V;
            _dI = _Beta*_T*_V - _Delta*_I;
            _dV = _P*_I - _C*_V + _NE;
        }
    }else {
        _dT = -_Beta*_T*_V;
        _dI = _Beta*_T*_V - _Delta*_I;
        _dV = _P*_I - _C*_V + _NE;
    }
    
}

void InHostDynamics::Jacobian(){
    
}

InHostDynamics::~InHostDynamics(){
    
}



