clear all; close all; clc;

ver = '31';
[y,m,d] = datevec(date());

timestamp = [num2str(m),'-',num2str(d),'-',num2str(y)];

locnum = 2;

if locnum > 1
    dataFolder = ['../../Data/data_multi_v',ver,'_',timestamp];
    movieFolder = ['../../Data/movie_multi_v',ver,'_',timestamp];
else
    dataFolder = ['../../Data/data_single_v',ver,'_',timestamp];
    movieFolder = ['../../Data5/movie_single_v',ver,'_',timestamp];
end



vis = Visualization(dataFolder, movieFolder,1,1,1);
vis.TaggedPeople = [1, 9];
vis.FullScreen = 1;
vis.ShowNodes  = 0;
vis.GraphType = 'SIHist';

vis.MakeMovie = 1;

vis.ReadData();


ResFolder = ['../../Results/res_',timestamp];
if ~(isdir(ResFolder))
    mkdir(ResFolder);
end
movieFile = [ResFolder,'/mov_v',ver];
dataFile = [ResFolder,'/sim_v',ver];
SaveVid(vis.Frames, movieFile, 3)

h = vis.PlotHistory('SIR');

h2 = vis.PlotIndividual(3,1);

saveas(h,dataFile,'pdf')
saveas(h,dataFile,'fig')

saveas(h2,[dataFile,'p1'],'pdf')
saveas(h2,[dataFile,'p1'],'fig')

% h = vis.PlotPhase();
