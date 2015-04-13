clear all; close all; clc;

ed = EigenData(3,'/Users/sahand/Research/Explore/mydata3d.csv');
ed.EigenHistory();
ed.PlotEigenValues();

pv = 200000;
ir = 0.08;

for ii = 1:10
    iv = pv*ir;
    pv = pv + iv;
end
