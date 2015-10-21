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
    ID = id;
}
void InHostDynamics::setdt(double ti){
    dt = ti;
}
void InHostDynamics::setT0(int it){
    t0 = it;
}
void InHostDynamics::setBeta(double b){
    Beta = b;
}
void InHostDynamics::setDelta(double d){
    Delta = d;
}
void InHostDynamics::setP(double p){
    P = p;
}
void InHostDynamics::setC(double c){
    C = c;
}
void InHostDynamics::setT(double sc){
    T  = sc;
}
void InHostDynamics::setTi(double sc){
    Ti = sc;
}
void InHostDynamics::setI(double ic){
    I = ic;
}
void InHostDynamics::setV(double vi){
    V = vi;
}
void InHostDynamics::setMaxInfLev(double mil){
    MaxInfLev = mil;
}
void InHostDynamics::setNE(double ne){
    NE = ne;
}
void InHostDynamics::setTol(double tol){
    Tol = tol;
}
void InHostDynamics::setILRate(double ilrate){
    ILRate = ilrate;
}

// getters
int InHostDynamics::getID(){
    return ID;
}
double InHostDynamics::getdt(){
    return dt;
}
double InHostDynamics::getT0(){
    return t0;
}
double InHostDynamics::getBeta(){
    return Beta;
}
double InHostDynamics::getDelta(){
    return Delta;
}
double InHostDynamics::getP(){
    return P;
}
double InHostDynamics::getC(){
    return C;
}
double InHostDynamics::getT(){
    return T;
}
double InHostDynamics::getTi(){
    return Ti;
}
double InHostDynamics::getI(){
    return I;
}
double InHostDynamics::getV(){
    return V;
}
double InHostDynamics::getMaxInfLev(){
    return MaxInfLev;
}
double InHostDynamics::getNE(){
    return NE;
}
double InHostDynamics::getTol(){
    return Tol;
}
double InHostDynamics::getILRate(){
    return ILRate;
}

//utilities
void InHostDynamics::Simulate(){
    for (double tt = t0; tt <= t0+1; tt = tt+dt){
        Update();
        if (getI() > getMaxInfLev()){
            setMaxInfLev(getI());
        }
    }
    
}

void InHostDynamics::Update(){
    
    Flow();
    
    if (isnan(V)){
        cout << "V: " << V << endl;
        cout << "I: " << I << endl;
        cout << "T: " << T << endl;
        cout << "dV: " << dV << endl;
        cout << "dI: " << dI << endl;
        cout << "dT: " << dT << endl;
        exit(1);
    }
    
    
    T = T + dt*dT;
    I = I + dt*dI;
    V = V + dt*dV;
    
    
}

void InHostDynamics::Flow(){
    
    
    if (T<Tol){
        if (!HasBeenSick){
            dT = ILRate;
            dI = 0.0;
            dV = 0.0;
        } else{
            dT = -Beta*T*V;
            dI = Beta*T*V - Delta*I;
            dV = P*I - C*V + NE;
        }
    }else {
        dT = -Beta*T*V;
        dI = Beta*T*V - Delta*I;
        dV = P*I - C*V + NE;
    }
    
}

void InHostDynamics::Jacobian(){
    
}

InHostDynamics::~InHostDynamics(){
    
}



