clear all; close all; clc;

ed = EigenData(3,'/Users/sahand/Research/Explore/mydata3d.csv');
ed.EigenHistory();
ed.PlotEigenValues();
