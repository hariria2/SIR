
clear all; close all; clc;
tend  = 100;
dt    = 0.1;
ic    = [0.6,1-0.6,0];

%% === SIRDE(tend, dt, ic)=======================
SIR1 = SIRDE(tend,0.2,ic, 2000);

% ===== V26 =======
SIR1.beta  = @(t) 0.3;
SIR1.gamma = @(t) 0.1;
SIR1.mu    = 0.5; 
SIR1.f     = 0.15;
% ===== V23 =======
%SIR1.beta = .175;
%SIR1.gamma = .0075;` 

% ===== V20 =======
%SIR1.beta = .175;
%SIR1.gamma = .0085;

SIR1.Simulate(0);
%SIR1.plot('LegendOn')
%grid on

SIR1.getEigen();
SIR1.plot(1,'LegendOn')
SIR1.phasePlot(2)
SIR1.timePlotEigen(3)
%SIR1.getRoots();
%SIR1.singlePlot('All',1)
%SIR1.timePlotEigen()
%SIR1.makeMovie(3)

%open('../../Results/res_3-2-2015/sim_v26.fig')
%open('../../Results/res_4-23-2015/sim_v2.fig')

%set(gcf,'units','normalized','outerposition',[0 0 1 1])
%hold on
%SIR1.plot('LegendOn')

