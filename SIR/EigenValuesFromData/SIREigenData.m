clear all; close all; clc;

ed = EigenData(3,'../../Data/eigdata_single_v2_4-20-2015/eigHistoryData.dat');
ed.EigenHistory();
ed.PlotEigenValues();
