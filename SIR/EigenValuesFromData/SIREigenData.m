clear all; close all; clc;

%file = '../../../Explore/mydata3d.csv';
file = '../../Data/eigdata_single_v11_4-21-2015/eigHistoryData.dat'
ed = EigenData(3,file);
ed.EigenHistory();
ed.PlotEigenValues();

SS = ed.x(:,1);
II = ed.x(:,2);
RR = ed.x(:,3);

figure
plot(ed.t,SS,ed.t,II,ed.t,RR, 'linewidth',3)
legend('Susceptible', 'Infected', 'Recovered')