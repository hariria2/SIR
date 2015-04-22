clear all; close all; clc;

%file = '../../../Explore/mydata3d_2.csv';
file = '../../Data/eigdata_single_v3_4-22-2015/eigHistoryData.dat';
ed = EigenData(3,file);
ed.EigenHistory();
ed.PlotTrajectory();
ed.PlotEigenValues();