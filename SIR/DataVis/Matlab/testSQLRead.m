tic
close all; clear all; clc;
conn = database('sim_v3_8_3_2015','root','sHa136384', 'Vendor','MySQL','server','localhost');


sqlquery = 'SELECT * FROM HistoryData';
curs = exec(conn,sqlquery);
curs = fetch(curs);
data = cell2mat(curs.Data);

time = data(:,2);
S    = data(:,3);
I    = data(:,4);
P    = data(:,5);
R    = data(:,6);
D    = data(:,7);

figure(1)
p = plot(time, S, 'b', time, I, 'Y', time, P, 'R', time, R, 'g', time, D, 'k');
set(p, 'LineWidth', 4);
grid on;

sqlquery = 'SELECT * FROM PersonValues';
curs = exec(conn,sqlquery);
curs = fetch(curs);
toc
% 
% sqlquery = 'SELECT time, SusCells, InfCells, VirLoads FROM PersonValues WHERE PersonID = 2';
% curs = exec(conn,sqlquery);
% curs = fetch(curs);
% data = cell2mat(curs.data);
% 
% time = data(:,1);
% S    = data(:,2);
% I    = data(:,3);
% V    = data(:,4);
% 
% 
% figure(2)
% p = plot(time, S, 'b', time, I, 'R', time, V, 'g');
% set(p, 'LineWidth', 4);
% grid on;