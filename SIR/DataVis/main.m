clear all; close all; clc;
ver = '26';
[y,m,d] = datevec(date());

timestamp = [num2str(m),'-',num2str(d),'-',num2str(y)];

locnum = 1;

if locnum > 1
    dataFolder = ['../../Data/data_multi_v',ver,'_',timestamp];
    movieFolder = ['../../Data/movie_multi_v',ver,'_',timestamp];
else
    dataFolder = ['../../Data/data_single_v',ver,'_',timestamp];
    movieFolder = ['../../Data/movie_single_v',ver,'_',timestamp];
end
vis = Visualization(dataFolder, movieFolder,1,1);

%vis.MakeMovie();
ResFolder = ['../../Results/res_',timestamp];
mkdir(ResFolder);
movieFile = [ResFolder,'/mov_v',ver];
dataFile = [ResFolder,'/sim_v',ver];
%movie2avi(vis.Frames,movieFile,'fps',1.5);
h = vis.PlotHistory('SIRONLY');
saveas(h,dataFile,'pdf')
saveas(h,dataFile,'fig')