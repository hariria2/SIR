clear all; close all; clc;

%file = '../../../Explore/mydata3d_2.csv';
%file = '../../Data/eigdata_single_v4_5-18-2015/eigHistoryData.dat';
%file = '../../Data/eigdata_single_v2_5-19-2015/eigHistoryData.dat';
%file = '../../Data/eigdata_single_v3_5-19-2015/eigHistoryData.dat';
%file = '../../Data/eigdata_single_v4_5-19-2015/eigHistoryData.dat';
%file = '../../Data/eigdata_single_v5_5-19-2015/eigHistoryData.dat';
%file = '../../Data/eigdata_single_v3_5-21-2015/eigHistoryData.dat';
file = '../../Data/eigdata_single_v4_5-22-2015/eigHistoryData.dat';
ed = EigenData(3,file);
ed.EigenHistory();
ed.PlotTrajectory();
ed.PlotEigenValues();
ed.argonPlotEigen();