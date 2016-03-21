classdef GraphStuff < handle
    
    properties
        
        
        Showgraph;
        Frames;

        colors  = [];
        People  = {};
        Peopx   = [];
        Peopy   = [];
        MovieFolder;
        FullScreen = 0;
        ShowNodes  = 0;
        graphFiles;
        Adj;
        IDs;
        ActiveIDs;
        Nodes;
        ActiveNodes;
        GraphType = 'SI';
        DistanceMatrix;
        WeinerIndex;
        MeanDistance = 0;
        MeanClustering = 0;
        ClusteringVar = 0;
        MeanDegree = 0;
        DegreeVar = 0;
        DegreeDistribution = [];
        LocalClustering = [];
        Directed = 0;
    end
    
    methods
        function gs = GraphStuff(movieFolder, showGraph)
            gs.MovieFolder = movieFolder;
           
            if showGraph
                directoryfiles = dir(['../Data/', movieFolder]);
                
                files = cell(1, length(directoryfiles)-2);
                for ii = 1:length(files)
                    files{ii} = directoryfiles(ii + 2).name;
                end
               gs.graphFiles = files;
            end
        end
       
        function ProduceGraphData(obj, t, type)
            f = strcat(obj.MovieFolder,'/',obj.graphFiles(t));
            rawData = readtable(f{1});
            ids  = rawData.ID+1;
            obj.IDs = ids;
            xs   = rawData.x;
            ys   = rawData.y;
            switch type
                case 'SI'
                    cons = rawData.SIConnections;
                case 'SIHist'
                    cons = rawData.SIConnectionsHist;
                case 'All'
                    cons = rawData.AllConnections;
                case 'AllHist' 
                    cons = rawData.AllConnectionsHist;
                otherwise
                    disp('error: choose the right case of network type.')
            end
            activeids = [];
        
            A    = zeros(length(ids));
            C    = zeros(length(ids),2);
            n = 1;
            md = 0;
            dd = []; % degree distribution
            for ii = 1:length(ids)
                c = eval(char(cons(ii)))+1;
                for jj = c
                    if ii == jj
                        A(ii,jj) = 0;
                    else
                        A(ii,jj) = 1;
                    end
                end
                C(ii,:) =[xs(ii), ys(ii)];
                if ~isempty(c)
                    obj.ActiveNodes(n,:) = [xs(ii), ys(ii)];
                    n = n + 1;
                    activeids = [activeids, ii];
                    activeids = [activeids, c];
                    md = md + length(unique(c));
                    dd = [dd, length(unique(c))];
                    
                    %obj.MeanDegree = obj.MeanDegree + length(unique(c));
                end
            end
            obj.ActiveIDs = sort(unique(activeids));
            obj.ActiveNodes = zeros(length(obj.ActiveIDs),2);
            obj.MeanDegree = md/length(obj.ActiveIDs);
            obj.DegreeDistribution = dd;
            obj.DegreeVar = var(dd);
            
            % TODO: Should I really do this? 
            if obj.Directed == 1
                obj.Adj = sparse(A);  
            else
                A = (A + A');
                obj.Adj = sparse(A);  
            end
                    
            obj.Nodes = C;
            
        end     
        function DrawPeople(obj,person) 
            if strcmpi(person.State,'S')
                plot(person.Coordinates(1),person.Coordinates(2),'.b', 'MarkerSize',20)
            elseif strcmpi(person.State,'I')
                plot(person.Coordinates(1),person.Coordinates(2),'.y', 'MarkerSize',20)
            elseif strcmpi(person.State,'P')
                plot(person.Coordinates(1),person.Coordinates(2),'.r', 'MarkerSize',20)
            elseif strcmpi(person.State,'R')
                plot(person.Coordinates(1),person.Coordinates(2),'.g', 'MarkerSize',20)
            else % Dead people
                plot(person.Coordinates(1),person.Coordinates(2),'.k', 'MarkerSize',20)
            end
            if any(person.ID == obj.TaggedPeople)
                text(person.Coordinates(1),person.Coordinates(2),['\bf',int2str(person.ID)], 'fontsize', 18, 'Color', 'm')
            end
        end
        function h = Render(obj,t)
            h = figure(1);
            if obj.FullScreen
                set(gcf,'units','normalized','outerposition',[0 0 1 1])
            end
            hold on
           
            obj.DrawGraph(t,1,0.01);
            for ii = 1:length(obj.People)
                obj.DrawPeople(obj.People{ii})
                hold on
            end
            
            title(sprintf('Time = %d',obj.People{1}.Time),'fontsize',22)
           
        end
        function DrawGraph(obj, fignum, pad) 
            figure(fignum)
            if obj.FullScreen
                set(gcf,'units','normalized','outerposition',[0 0 1 1])
            end
            gplot(obj.Adj,obj.Nodes,'k-*');
            
            if obj.ShowNodes 
               [fn,sn,~] = find(obj.Adj);
                 nodes = [fn;sn];
                 for n = nodes
                     text(obj.Nodes(n,1),obj.Nodes(n,2)+pad,num2str(n,'%d'),'fontsize', 18)
                 end
            end
        end
        function DrawGraphWithNodes(obj)
            view(biograph(obj.Adj))
        end
        function [S, C] = GraphStats(obj)
        bgo = biograph(obj.Adj);
        [S, C] = conncomp(bgo);
        end
        function getAverageDistance(obj)
            if obj.Directed
                obj.DistanceMatrix = graphallshortestpaths(obj.Adj,'Directed', true);
            else
                obj.DistanceMatrix = graphallshortestpaths(obj.Adj,'Directed', false);
            end
            n = length(obj.ActiveIDs);
            s = 1;
            for ii = 1:n
                for jj = 1:n
                    if ii ~= jj
                        s = s + obj.DistanceMatrix(obj.ActiveIDs(ii),obj.ActiveIDs(jj));
                    end
                end
            end
            
            obj.WeinerIndex = s/2;
            obj.MeanDistance = obj.WeinerIndex/(n*(n-1));
        end
        function getAverageClustering(obj)
            N = length(obj.ActiveIDs);
            C = 0;
            obj.LocalClustering = zeros(1,N);
      
            for ii = 1:N
                c = obj.getLocalClusteringCoeff(obj.ActiveIDs(ii));
                obj.LocalClustering(ii) = c;
                C = C + c; 
            end
            obj.MeanClustering = C/N;
            obj.ClusteringVar = var(obj.LocalClustering);
        end
        function r = getDistanceFromTo(obj,a,b)
            r = obj.DistanceMatrix(a,b);
        end
        function c = getLocalClusteringCoeff(obj, v)
            [~, neighbors, vals] = find(obj.Adj(v,:));
            N = length(vals);
            tc = 0;
            AA = [];
            for ii = 1:length(neighbors)
                [~, Nn, ~] = find(obj.Adj(neighbors(ii),:));

                for jj = 1:length(Nn)     
                    if sum(neighbors==Nn(jj))
                        AA = [AA; sort([Nn(jj), neighbors(ii)])];
                    end
                end
            end
            nc = size(unique(AA, 'rows'));
            
            tc = tc + nc(1);
            
            if N == 1
                c = 0;
            else
                if obj.Directed
                	c = tc/(N*(N-1));
                else
                    c = 2*tc/(N*(N-1));
                end
            end
            
            
            
        end
        
    end
end
