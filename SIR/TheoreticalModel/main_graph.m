clear all; close all; clc;

ver = '1';
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


gs = GraphStuff(movieFolder, 1);
gs.Directed = 0;
gs.FullScreen = 1;
gs.ShowNodes = 1;
gs.GraphType = 'SIHist';
gs.DrawGraph(8, 1, 0.1)
gs.DrawGraphWithNodes();
gs.getAverageDistance();
gs.MeanDistance;
gs.getDistanceFromTo(5,1);
gs.getAverageClustering();
