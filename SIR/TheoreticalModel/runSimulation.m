
clear all; close all; clc;
tend  = 200;
dt    = 0.1;
ic    = [0.9995,1-0.9995,0];

%% === SIRDE(tend, dt, ic)=======================
SIR1 = SIRDE(tend,0.2,ic, 2000);

% ===== V26 =======
SIR1.beta  = 3.875;
SIR1.gamma = .011;
SIR1.mu    = .05; 

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
SIR1.singlePlot('All',1)

%SIR1.makeMovie(3)

%open('../../Results/res_3-2-2015/sim_v26.fig')
open('../../Results/res_4-23-2015/sim_v2.fig')

set(gcf,'units','normalized','outerposition',[0 0 1 1])
hold on
%SIR1.plot('LegendOn')

