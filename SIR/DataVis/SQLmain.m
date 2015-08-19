close all; clc;

ver = '1';

[y,m,d] = datevec(date());

timestamp = [num2str(m),'_',num2str(d),'_',num2str(y)];

saveResults = 1;
makeMovie   = 1;

vis = SQLVisualization(['sim_v',ver,'_',timestamp],'root','','MySQL','localhost');

vis.getHistoryData();
subplot(2,1,1)
h = vis.PlotHistory(1);
subplot(2,1,2)
h = vis.PlotGDP(1);
vis.FullScreen = 0;
vis.ShowProgress = 0;
vis.ShowDemand = 1;
h2 = vis.PlotIndividual(3, [1, 89, 575, 561]);

if makeMovie
    vis.FullScreen = 1;
    vis.getLocation('All');
    vis.TaggedPeople = [];
    vis.getPerson('All');
    vis.MakeMovie(4);
end

if saveResults 
    ResFolder = ['../../Results/res_',timestamp];
    if ~(isdir(ResFolder))
        mkdir(ResFolder);
    end

    movieFile = [ResFolder,'/mov_v',ver];
    dataFile = [ResFolder,'/sim_v',ver];

    SaveVid(vis.Frames, movieFile, 3)
    saveas(h,dataFile,'pdf')
    saveas(h,dataFile,'fig')
    
    saveas(h2,[dataFile,'p1'],'pdf')
    saveas(h2,[dataFile,'p1'],'fig')  
end

