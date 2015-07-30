clear all; close all; clc;

%for pii = 10:18
    
% wbh = waitbar(0,'Please wait...');
% ver = num2str(pii);

ver = '7';
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

tend = 19;

gs = GraphStuff(movieFolder, 1);
gs.Directed = 0;
gs.FullScreen = 1;
gs.ShowNodes = 1;
gs.GraphType = 'SIHist';
gs.ProduceGraphData(tend, gs.GraphType); 
%gs.DrawGraph(1, 0.1)

fill_between_lines = @(X,Y1,Y2,C) fill( [X fliplr(X)],  [Y1 fliplr(Y2)], C );


t = 2:tend;
yl = zeros(1,length(t));
yc = zeros(1,length(t));
cv = zeros(2,length(t));
dv = zeros(2,length(t));
yd = zeros(1,length(t));
ln = zeros(1,length(t));

for ii = 1:length(t)
    gs.ProduceGraphData(t(ii), gs.GraphType); 
    gs.getAverageDistance();
    gs.getAverageClustering();
    mb = gs.MeanDistance;
    mc = gs.MeanClustering;
    md = gs.MeanDegree;
    if isnan(mb)
        yl(ii) = 0;
    else
        yl(ii) = mb;
    end
    if isnan(mc)
        yc(ii) = 0;
    else
        yc(ii) = mc;
    end
    if isnan(md)
        yd(ii) = 0;
    else
        yd(ii) = md;
    end
    ln(ii) = log(length(gs.ActiveIDs));
    if isnan(gs.ClusteringVar)
        cv(:,ii) = [0,0];
    else
        cv(:,ii) = [gs.MeanClustering+gs.ClusteringVar;gs.MeanClustering-gs.ClusteringVar];
    end
    
    if isnan(gs.DegreeVar)
        dv(:,ii) = [0,0];
    else
        dv(:,ii) = [gs.MeanDegree+gs.DegreeVar;gs.MeanDegree-gs.DegreeVar];
    end
end

f1 = figure(1);
% subplot(3,3,pii-9)
% ccolor = [0.1,0.95,0.9];
% fh = fill_between_lines(t, cv(1,:),cv(2,:),ccolor);
% set(fh,'edgecolor', ccolor)
% hold on
% 
% dcolor= [0.9,0.95,0.1];
% fh = fill_between_lines(t, dv(1,:),dv(2,:),dcolor);
% set(fh,'edgecolor', dcolor)
% hold on


p = plot(t, yl,'o', t, yc,'x', t, yd, '+');

l=legend(p,'Average Length','Average Clustering Coefficient','Average Degree','Location','NorthWest');
set(l,'fontsize',18)

xlabel('t','fontsize',18)

hold on 
plot(t,ln,'k--','linewidth', 3)

f2 = figure(2);
%subplot(3,3,pii-9)
hist(gs.DegreeDistribution,10)

%waitbar(pii/9,wbh,sprintf('%2.1f %% of this step complete...',100 * pii/9))


% close(wbh)

% ResFolder = ['../../Results/res_',timestamp];
% if ~(isdir(ResFolder))
%     mkdir(ResFolder);
% end
% 
% dataFile_mean = [ResFolder,'/mean_v',ver];
% saveas(f1,dataFile_mean,'fig')
% 
% 
% dataFile_Dist= [ResFolder,'/dist_v',ver];
% saveas(f2,dataFile_Dist,'fig')
