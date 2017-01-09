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

	InHostDynamics(int id, double ti, double sc, double ic, double vi, double tol,int incubationPeriod, int infectionPeriod, int recoveryPeriod);

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

	void setAverageInfectionPeriod(int x);
	void setAverageIncubationPeriod(int x);
	void setAverageRecoveryPeriod(int x);

	// getters
	int getAverageInfectionPeriod();
	int getAverageIncubationPeriod();
	int getAverageRecoveryPeriod();


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
	/// Update computes the next step in the model using Euler's method.
	void Update();
	/// This is the flowfield, or the derivative function for the model.
	void Flow();
	void Jacobian();

	int HasBeenSick = 0;

private:

	int _ID;
	double _dt;
	int _t0;

	int _AverageInfectionPeriod;
	int _AverageIncubationPeriod;
	int _AverageRecoveryPeriod;

	// state variables and derivatives
	/// Total number of virions. This is a state variable.
	double _V;
	/// Total number of infcted cells. This is a state variable.
	double _I;
	/// Total number of susceptible cells. This is a state variable.
	double _T;
	/// Initial susceptibility level of the individual.
	double _Ti;
	/// Change of virion level per time.
	double _dV;
	/// Change in number of infected cells per time.
	double _dI;
	/// Change in number of susceptible cells per time.
	double _dT;
	/// Maximum infection level a person achieves for all time.
	double _MaxInfLev;
	/// Total effect of neighbors on the person.
	double _NE;     // Neighbor Effects

	// parameters
	/// The rate at which number of suscebtile/infected cells decrease/increase.
	double _Beta;
	/// The rate at which number of infected cells decrease.
	double _Delta;
	/// The rate at which number of virion increase because of infected cells.
	double _P;
	/// The rate at which virions die.
	double _C;
	/// Deprecated
	double _Tol;
	/// Deprecated
	double _ILRate;



};



#endif
