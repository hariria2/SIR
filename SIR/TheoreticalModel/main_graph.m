clear all; close all; clc;

ver = '2';
[y,m,d] = datevec(date());

timestamp = [num2str(m),'-',num2str(d),'-',num2str(y)];

locnum = 2;

if locnum > 1
    dataFolder = ['../../Data/data_multi_v',ver,'_',timestamp];
    movieFolder = ['../../Data/movie_multi_v',ver,'_',timestamp];
else
    dataFolder = ['../../Data/data_single_v',ver,'_',timestamp];
    movieFolder = ['../../Data/movie_single_v',ver,'_',timestamp];
end

t = 5;

gs = GraphStuff(movieFolder, 1);
gs.Directed = 0;
gs.FullScreen = 1;
gs.ShowNodes = 1;
gs.GraphType = 'SIHist';
gs.ProduceGraphData(t, gs.GraphType); 
gs.DrawGraph(1, 0.1)




t = 2:49;
yl = zeros(1,length(t));
yc = zeros(1,length(t));
yd = zeros(1,length(t));
ln = zeros(1,length(t));

for ii = 1:length(t)
    gs.ProduceGraphData(t(ii), gs.GraphType); 
    gs.getAverageDistance();
    gs.getAverageClustering();
    yl(ii) = gs.MeanDistance;
    yc(ii) = gs.MeanClustering;
    yd(ii) = gs.MeanDegree;
    ln(ii) = log(length(gs.ActiveIDs));
end

figure(2)
plot(t, yl,'o', t, yc,'x', t, yd, '+');


l=legend('Average Length','Average Clustering Coefficient','Average Degree','Location','NorthWest');
set(l,'fontsize',18)

xlabel('t','fontsize',18)

hold on 
plot(t,ln,'k--')

% hold on
% y1 = (yl(end)-yl(1))/(t(end)-t(1)) * t + yl(1);
% y2 = (yc(end)-yc(2))/(t(end)-t(2)) * t + yc(2);
% plot(t, y1,'k',t, y2,'k','linewidth',.2)