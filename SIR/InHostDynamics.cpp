//
//  InHostDynamics.cpp
//  SIR
//
//  Created by Sahand Hariri on 6/30/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#include "InHostDynamics.h"

InHostDynamics::InHostDynamics(int id, double ti, double sc, double ic, double vi, double tol,int incubationPeriod, int infectionPeriod, int recoveryPeriod){
	setID(id);
	setdt(ti);
	setT(sc);
	setI(ic);
	setV(vi);
	setTi(sc);
	setTol(tol);
	setAverageIncubationPeriod(incubationPeriod);
	setAverageInfectionPeriod(infectionPeriod);
	setAverageRecoveryPeriod(recoveryPeriod);
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
void InHostDynamics::setK(double k){
	_K = k;
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
void InHostDynamics::setE(double E){
	_E = E;
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
void InHostDynamics::setAverageInfectionPeriod(int ip){
	_AverageInfectionPeriod = ip;
}
void InHostDynamics::setAverageIncubationPeriod(int ip){
	_AverageIncubationPeriod = ip;
}
void InHostDynamics::setAverageRecoveryPeriod(int rp){
	_AverageRecoveryPeriod = rp;
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
int InHostDynamics::getAverageInfectionPeriod(){
	return _AverageInfectionPeriod;
}
int InHostDynamics::getAverageIncubationPeriod(){
	return _AverageIncubationPeriod;
}
int InHostDynamics::getAverageRecoveryPeriod(){
	return _AverageRecoveryPeriod;
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
	/**
	 *\callergraph
	 *
	 * This function simply uses Euler's method (below) to update the disease status.
	 * The model is described by Flow().
	 *
	 * #### Notes:
	 *	+ If _V reaches below 1e-2, it is set to zero.
	 *	+ If the person has been sick before, and the infection level has dropped 
	 *		to belor 0.05, then _V and _I are set to zero.
	 *
	 * ### Euler's method:
	 * \f[
	 *	y_t = y_{t-1} + dt \frac{df}{dt}
	 * \f]
	 */
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
	_E = _E + _dt*_dE;
	_I = _I + _dt*_dI;
	_V = _V + _dt*_dV;
	
	/*if (_I < 1e-3){
	 _I=0;
	 }*/
	if (_V < 1e-2){
		_V=0;
	}
	if (HasBeenSick==1 & _I < 0.05){
		_V=0;
		_I=0;
	}
}
void InHostDynamics::Flow(){
	/**
	 *\callergraph
	 *
	 * This function describes the flow field for development of the disease within each individual.
	 *
	 * #### Notes:
	 *	+ A term NE is added to the last equation which represents the effect of other people on _this_ individual.
	 *
	 * ### The model:
	 * \f{eqnarray*}{
	 *	\frac{dT}{dt} &=& -\beta T V \\
	 *	\frac{dI}{dt} &=&  \beta T V - \delta I \\
	 *	\frac{dV}{dt} &=& P I - C V
	 * \f}
	 */
	_NE = (_NE > 1)? 1:_NE;
	_dT = -_Beta*_T*_V;
	_dE = _Beta*_T*_V - _K*_E;
	_dI = _K*_E - _Delta*_I;
	_dV = _P*_I - _C*_V + _NE;
	
}
void InHostDynamics::Jacobian(){
	
}

InHostDynamics::~InHostDynamics(){
	
}



