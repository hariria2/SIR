
clear all; close all; clc;
tend  = 50;
dt    = 0.1;
ic    = [3,0,0.1];

%% === SIRDE(tend, dt, ic)=======================
ihf = InHostFlu(tend,0.2,ic, 1);

% ===== V26 =======
ihf.beta  = @(t) 2;
ihf.delta = @(t) 1.5;
ihf.p     = @(t) 5;
ihf.c     = @(t) .5;

ihf.Simulate(0);
ihf.plot(1,'LegendOn')
