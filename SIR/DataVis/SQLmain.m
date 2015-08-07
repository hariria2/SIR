close all; clc;
vis = SQLVisualization('sim_v1_8_6_2015','root','','MySQL','localhost');

vis.getHistoryData();
vis.PlotHistory(1);

vis.getLocation('All');
vis.getPerson('All');
vis.PlotIndividual(2, [1,2,3])


vis.MakeMovie(3);
