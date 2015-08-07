close all; clc;
vis = SQLVisualization('sim_v1_8_6_2015','root','','MySQL','localhost');

vis.getHistoryData();
vis.PlotHistory(1);


vis.getPerson(1:200);
vis.getLocation('All');
vis.MakeMovie(2);
