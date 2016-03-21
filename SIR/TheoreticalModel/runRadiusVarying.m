
clear all; close all; clc;
tend  = 200;
dt    = 0.1;
ic    = [0.7,1-0.7,0];

%% === SIRDE(tend, dt, ic)=======================
SIR1 = SIRDE(tend,0.2,ic, 2000);

bbeta = 0.8; %linspace(1,1000,5);
eigs  = zeros(length(ic),length(bbeta));
for ii = 1:length(bbeta)

    SIR1.beta  = bbeta(ii);
    SIR1.gamma = 0.1;
    SIR1.mu    = .05;
    SIR1.f     = 0.0;
    
    ic(1)*SIR1.beta/SIR1.gamma
    
    
    SIR1.Simulate(0);
    SIR1.getEigen();
    eigs(:,ii) = SIR1.lam(:,1);
    SIR1.plot(1,'LegendOn'); hold on
    grid on

    SIR1.getEigen();
    SIR1.timePlotEigen(2);
    hold on
end
hold off
figure(3)
for ii = 1:length(ic)
    subplot(length(ic), 2, 2*ii-1)
    plot(bbeta,real(eigs(ii,:)),'LineWidth',3)
    
    subplot(length(ic),2, 2*ii)
    plot(bbeta,imag(eigs(ii,:)),'LineWidth',3)
end