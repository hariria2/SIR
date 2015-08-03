clear all; close all; clc;
vis = SQLVisualization('sim_v3_8_3_2015','root','sHa136384','MySQL','localhost');

vis.readHistoryData();

plot(vis.T, vis.S);