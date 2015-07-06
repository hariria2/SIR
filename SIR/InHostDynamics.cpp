//
//  InHostDynamics.cpp
//  SIR
//
//  Created by Sahand Hariri on 6/30/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <list>
#include <vector>
#include <random>
#include <ctime>
#include "InHostDynamics.h"

InHostDynamics::InHostDynamics(int id, double ti, double sc, double ic, double vi){
    setID(id);
    setdt(ti);
    setT(sc);
    setI(ic);
    setV(vi);
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
void InHostDynamics::setI(double ic){
    I = ic;
}
void InHostDynamics::setV(double vi){
    V = vi;
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
double InHostDynamics::getI(){
    return I;
}
double InHostDynamics::getV(){
    return V;
}

//utilities
void InHostDynamics::Simulate(){
    
    for (double tt = t0; tt <= t0+1; tt = tt+dt){
        Update();
    }
    
}

void InHostDynamics::Update(){
    
    Flow();
    
    T = T + dt*dT;
    I = I + dt*dI;
    V = V + dt*dV;
   
    
}

void InHostDynamics::Flow(){
    
    dT = -Beta*T*V;
    dI =  Beta*T*V - Delta*I;
    dV =  P*I - C*V;
    
    
    
}

void InHostDynamics::Jacobian(){
    
}

InHostDynamics::~InHostDynamics(){
    
}



