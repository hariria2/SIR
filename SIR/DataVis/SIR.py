import numpy as np
import math
import matplotlib.pylab as plt

class SIR:
    def __init__(self, tstart, tend, dt, beta, gamma, N):

        self._tstart = tstart;
        self._dt     = dt;
        self._tend   = tend;
        self._beta   = beta;
        self._gamma  = gamma;
        self._N      = N;
        self._dS     = 0;
        self._dI     = 0;
        self._dR     = 0;

    def Flow(self):
        self._dS = -self._beta*self._I*self._S/self._N;
        self._dI =  self._beta*self._I*self._S/self._N - self._gamma*self._I;
        self._dR =  self._gamma*self._I;

    def Update(self):
        self.Flow();
        self._S = self._S + self._dt*self._dS;
        self._I = self._I + self._dt*self._dI;
        self._R = self._R + self._dt*self._dR;

    def Initialize(self,S0,I0,R0):
        self._S    = S0;
        self._I    = I0;
        self._R    = R0;
        self._Time = np.arange(self._tstart,self._tend,self._dt);
        self._SS   = np.zeros(len(self._Time));
        self._II   = np.zeros(len(self._Time));
        self._RR   = np.zeros(len(self._Time));

    def Simulate(self):
        for ii in range(len(self._Time)):
            self._SS[ii] = self._S;
            self._II[ii] = self._I;
            self._RR[ii] = self._R;
            self.Update();

    def PlotSIR(self, num):
        plt.figure(num)
        ps = plt.plot(self._Time,self._SS,label="Susceptible");
        plt.setp(ps, 'Color', 'b', 'linewidth', 3)
        pi = plt.plot(self._Time,self._II,label="Infected");
        plt.setp(pi, 'Color', 'r', 'linewidth', 3)
        pr = plt.plot(self._Time,self._RR,label="Recovered");
        plt.setp(pr, 'Color', 'g', 'linewidth', 3)
        plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,ncol=3, mode="expand", borderaxespad=0.)
        plt.show()
