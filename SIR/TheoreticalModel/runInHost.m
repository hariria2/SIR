
clear all; close all; clc;
tend  = 50;
dt    = 0.1;
ic    = [1,0,0.1];

%% === SIRDE(tend, dt, ic)=======================
ihf = InHostFlu(tend,0.2,ic, 2000);

% ===== V26 =======
ihf.beta  = @(t) 1;
ihf.delta = @(t) 0.5;
ihf.p     = @(t) 5;

ihf.Simulate(0);
ihf.plot(1,'LegendOn')
