classdef SQLVisualization < handle
    
    properties
        Connection;
        People = {};
        Locations = {};
        LocationIDs = [];
        PeopleIDs = [];
        HistoryData;
        MaxInfLev = 0;
        T;
        S;
        I;
        P;
        R;
        D;
        Ho;
        Sc;
        Wo;
        GDP;
        Demand;
        FullScreen = 0;
        TracePerson = 0;
        Frames;
        polyxs;
        polyys;
        colors;
        TaggedPeople = [];
        ShowProgress = 0;
        ShowDemand = 0;
        % Colors
        gr = [0.2, 0.7, 0.3];
        re = [0.7, 0.2, 0.3];
        bl = [0.2, 0.3, 0.7];
        yl = [0.7, 0.7, 0.3];
        
    end
    
    
    methods
        function sv = SQLVisualization(db, usrnm, psswd, vndr, hst)
            sv.Connection = database(db, usrnm, psswd, 'vendor', vndr, 'server', hst);
        end
        
        function getHistoryData(obj)
            sqlquery = 'SELECT * FROM HistoryData';
            curs = exec(obj.Connection,sqlquery);
            curs = fetch(curs);
            data = cell2mat(curs.data);
            
            obj.T = data(:,2);
            obj.S = data(:,3);
            obj.I = data(:,4);
            obj.P = data(:,5);
            obj.R = data(:,6);
            obj.D = data(:,7);
            obj.Ho = data(:,8);
            obj.Wo = data(:,9);
            obj.Sc = data(:,10);
            obj.GDP = data(:,11);
            obj.Demand = data(:,12);
            
        end             
        function getPerson(obj, ids)
            
            query1 = '';
            query2 = '';
            if strcmp(ids, 'All')    
                sqlquery1 = 'SELECT * FROM People';
                sqlquery2 = 'Select * FROM PersonValues';
                
                curs1 = exec(obj.Connection,sqlquery1);
                curs1 = fetch(curs1);
                
                curs2 = exec(obj.Connection,sqlquery2);
                curs2 = fetch(curs2);
            
                s = size(curs1.data);
                l = s(1);
                ids = 1:l;
                obj.ShowProgress = 1;
            else 
                l = length(ids);
                
                for ii = 1:l
                    if ii == l
                        query1 = [query1, ['ID = ', int2str(ids(ii))]];
                    else
                        query1 = [query1, ['ID = ', int2str(ids(ii))], ' or '];
                    end
                end
                
                for ii = 1:l
                    if ii == l
                        query2 = [query2, ['PersonID = ', int2str(ids(ii))]];
                    else
                        query2 = [query2, ['PersonID = ', int2str(ids(ii))], ' or '];
                    end
                end
                
                sqlquery1 = ['SELECT * FROM People Where ',query1];
                sqlquery2 = ['SELECT * FROM PersonValues Where ',query2];
                
                curs1 = exec(obj.Connection,sqlquery1);
                curs1 = fetch(curs1);
                
                curs2 = exec(obj.Connection,sqlquery2);
                curs2 = fetch(curs2);
                
            end
            
            
            % ID, time, x, y, locationid, hasbeensick, suscells, infcells,
            % virloads, maxinflev
                
            myData = curs2.data;
    
            Alltxy = cell2mat(myData(:,[2, 3, 4, 5, 6, 8, 9, 10, 11, 12]));
            
            if obj.ShowProgress
                wbh = waitbar(0,'Loading People. Please wait...');
            end
            for ii = 1:length(ids)
                iah = cell2mat(curs1.data(ii, [1, 3, 5]));
                ng  = cell2mat(curs1.data(ii, [2,4]));
                % ===============>>>> FIX THIS HACK <<<<=================
                p = Person(iah(1), ng(1:end-1), iah(2), ng(end), iah(3));
          
                id = ids(ii);

                txy = Alltxy(Alltxy(:,1)==id,:);
                
                states = cell2mat(myData(:,7));
                state = states(Alltxy(:,1)==id);
            
                p.setTime(txy(:,2));
                p.setx(txy(:,3));
                p.sety(txy(:,4));
                p.setLocation(txy(:,5));
                p.setHasBeenSick(txy(:,6));
                p.setSusCells(txy(:,7));
                p.setInfCells(txy(:,8));
                p.setVirLoads(txy(:,9));
                p.setMaxInfLev(txy(:,10));
                p.setState(state);
            
                if ~obj.isMember(obj.PeopleIDs, id)
                    obj.People = [obj.People, p];
                    obj.PeopleIDs = [obj.PeopleIDs, id];
                end
                
                if obj.MaxInfLev < max(txy(:,10))
                    obj.MaxInfLev = max(txy(:,10));
                end
                
                if obj.ShowProgress
                    waitbar(ii/l,wbh,sprintf('%2.1f %% of loading people data is done...',100 * ii/l))
                end
                clear p;
            end
            if obj.ShowProgress
                close(wbh)
            end
        end
        function getLocation(obj, ids)
            
            query1 = '';
            if strcmp(ids, 'All')    
                sqlquery1 = 'SELECT * FROM Location';

                
                curs1 = exec(obj.Connection,sqlquery1);
                curs1 = fetch(curs1);
               
                s = size(curs1.data);
                l = s(1);
                ids = 1:l;
            else 
                l = length(ids);
                
                for ii = 1:l
                    if ii == l
                        query1 = [query1, ['ID = ', int2str(ids(ii))]];
                    else
                        query1 = [query1, ['ID = ', int2str(ids(ii))], ' or '];
                    end
                end
               
                
                sqlquery1 = ['SELECT * FROM Location Where ',query1];
                
                curs1 = exec(obj.Connection,sqlquery1);
                curs1 = fetch(curs1);
                
            end
            
            
            for ii = 1:length(ids)
                ng  = curs1.data(ii, [2, 3]);
                iah  = curs1.data(ii, [1,4,5,6,7]);
                
                L = Location(iah(1), ng(1), ng(2), iah(2), iah(3), iah(4),iah(5));
                id = ids(ii);   
                
                if ~obj.isMember(obj.LocationIDs, id)
                    
                    obj.Locations = [obj.Locations, L];
                    obj.LocationIDs = [obj.LocationIDs, id];
                end
                clear L;
            end
            
        end

        function h = PlotHistory(obj, fignum)  
            h = figure(fignum);
            hold on;
            ps = plot(obj.T, obj.S);
            set(ps,'Color', obj.bl);
            %pe = plot(obj.T, obj.I);
            %set(pe, 'Color', obj.yl);
            pi = plot(obj.T, obj.P+obj.I);
            set(pi, 'Color', obj.re);
            pr = plot(obj.T, obj.R);
            set(pr, 'Color', obj.gr);
            pd = plot(obj.T, obj.D);
            set(pd, 'Color', 'k');
     
            set([ps,pi,pr,pd],'LineWidth', 3);
            grid on;
            %sprintf('Exposed - %d',obj.I(end)),...
            l = legend([ps,pi,pr,pd],sprintf('Susceptible - %d',obj.S(end)),... 
                sprintf('Infected - %d',obj.P(end)+obj.I(end)), ...
                sprintf('Recovered - %d',obj.R(end)),...
                sprintf('Dead - %d',obj.D(end)));
            set(l, 'FontSize', 16);
            xlabel('Time','Interpreter', 'Latex', 'FontSize', 16)
            ylabel('Population','Interpreter', 'Latex', 'FontSize', 16)
            hold off;
        end
        function h = PlotGDP(obj, fignum)
            h = figure(fignum);
            c = [0.9,0,0; 0.9,0.9,0.0; 0,0.9,0];
            obj.ShadeMyFig(obj.T, [100, 400, max(obj.GDP)], c)
            hold on
            p = plot(obj.T, obj.GDP,'K');
            set(p,'LineWidth', 3);
            grid on;
            xlabel('Time','Interpreter', 'Latex', 'FontSize', 16)
            ylabel('GDP','Interpreter', 'Latex', 'FontSize', 16)
        end
        function DrawPeople(obj,person)
            if strcmpi(person.State,'S')
                plot(person.x,person.y,'.b', 'MarkerSize',20)
            elseif strcmpi(person.State,'I')
                plot(person.x,person.y,'.y', 'MarkerSize',20)
            elseif strcmpi(person.State,'P')
                plot(person.x,person.y,'.r', 'MarkerSize',20)
            elseif strcmpi(person.State,'R')
                plot(person.x,person.y,'.g', 'MarkerSize',20)
            else % Dead people
                plot(person.x,person.y,'.k', 'MarkerSize',20)
            end
            if any(person.ID == obj.TaggedPeople)
                text(person.x,person.y,['\bf',int2str(person.ID)], 'fontsize', 18, 'Color', 'm')
            end
        end   
        function DrawPeople_InHost(obj,person, t)
            b = abs(person.InfCells(t));
            hbs = person.HasBeenSick(t);
            cval = b; % /obj.MaxInfLev;
            state = person.State(t);
            
            
            
            if isnan(cval)
                cval = 0;
            end
            if cval > 1
                cval = 1;
            end
            if cval < 0
                cval = 0;
            end
            
            if hbs
                
                if strcmp(state, 'D')
                    color = [0,0,0];
                else
                    color = [cval,1-cval,0];
                end
            else
                color = [cval,0,1-cval];
            end
            
            
            
            p = plot(person.x(t),person.y(t), '.', 'MarkerSize',20);
            set(p,'Color', color);
            if any(person.ID == obj.TaggedPeople)
                text(person.x(t),person.y(t),['\bf',int2str(person.ID)], 'fontsize', 18, 'Color', 'm')
            end
        end
        function DrawPlace(obj,place)
            
            if strcmpi(place.Type, 'Domain')
                c = [0.95,1,.95];
            elseif strcmpi(place.Type,'Home')
                c = [.7,.8,.8];
            elseif strcmpi(place.Type,'School')
                c = [.6,.7,.7];
            elseif strcmpi(place.Type,'Work')
                c = [.6,.6,.1];
            else % Cemetery
                c= [0.9,0.9,0.9];
            end

            A = place.Perimeter;
            obj.polyxs = [obj.polyxs,[A(1,1);A(1,2);A(1,2);A(1,1)]];
            obj.polyys = [obj.polyys,[A(2,1);A(2,1);A(2,2);A(2,2)]];
            obj.colors = [obj.colors;c];
                
        end        
        function h = Render(obj,fignum, t)
            h = figure(fignum);
            if obj.FullScreen
                set(gcf,'units','normalized','outerposition',[0 0 1 1])
            end
            p = patch(obj.polyxs,obj.polyys,'b'); 
            set(p,'FaceColor','flat','FaceVertexCData',obj.colors)
            hold on
            
            
            for ii = 1:length(obj.People)
                obj.DrawPeople_InHost(obj.People(ii), t)
                hold on
            end
            hold off
            title(sprintf('Time = %d \n Total Time = %d', mod(t,24), t),'fontsize',22)
            
            day = floor(t/24);
            mhour = mod(t,24);
            hour = mod(t,12);
             if hour == 0
                 hour = 12;
             end
             
             if mhour < 12
                 AmPm = 'am';
             else
                 AmPm = 'pm';
             end
            text(50,1870,sprintf('Day: %d',day),'FontSize',16)
            text(50,1750,[sprintf('Time: %d:00 ',hour),AmPm],'FontSize',16)
            
        end
        function h = PlotIndividual(obj,fignum, ppl)
           
            h = figure(fignum);
            if obj.FullScreen
                set(gcf,'units','normalized','outerposition',[0 0 1 1])
            end
           
            ymax = 0;
            
            for ii = 1:length(ppl)
                if obj.isMember(obj.PeopleIDs, ppl(ii))
                    p = obj.People(obj.PeopleIDs==ppl(ii));
                else
                    obj.getPerson(ppl(ii));
                    p = obj.People(obj.PeopleIDs==ppl(ii));
                end
                SC = p.SusCells;
                IC = p.InfCells;
                VL = p.VirLoads;
                ymax = max(ymax,max(SC));
            end
            
            
            
            for ii = 1:length(ppl)
                p = obj.People(obj.PeopleIDs==ppl(ii));
                SC = p.SusCells;
                IC = p.InfCells;
                VL = p.VirLoads;
                t = p.Time;
                
                subplot(length(ppl),1,ii)
                c = [0,0.9,0; 0.9,0.9,0; 0.9,0,0; 0.0,0.0,0.0];
                obj.ShadeMyFig(t, [0.01,0.3,2.5,ymax], c)
                hold on
                [ax,sph,vph] = plotyy(t, SC, t, VL);
                set(ax(1),'ycolor','k','FontSize',14) 
                set(ax(2),'ycolor',obj.gr,'FontSize',14)
                set(sph,'Color',obj.bl,'LineWidth',3);
                set(vph,'Color',obj.gr,'LineWidth',3);
                
                iph = plot( t, IC);
                set(iph, 'Color',obj.re, 'linewidth', 3);
                ylabel(sprintf('ID: %d',ppl(ii)),'FontSize', 16);
                grid on
                if ymax > 0
                    ylim([0, ymax])
                end
                    
                if obj.ShowDemand
                    plot(t, obj.Demand, 'k-');
                end
                hold off
                if ii == 1
                    l = legend ([sph,iph,vph], 'Susceptible cells', 'Infected Cells','Viral Load');
                    newPosition = [0.31 0.6 0.4 0.73];
                    newUnits = 'normalized';
                    set(l,'Position', newPosition,'Units', newUnits, 'Orientation','horizontal', 'FontSize', 18);
                end
            end
            
            %subplot(length(ppl),1,length(ppl))
            xlabel('Time', 'FontSize', 16)
        end
        function h = PlotLocalPopulation(obj, fignum)
            h = figure(fignum);
            hold on;
            ph = plot(obj.T, obj.Ho);
            set(ph,'Color', obj.bl);
            pw = plot(obj.T, obj.Wo);
            set(pw, 'Color', obj.yl);
            ps = plot(obj.T, obj.Sc);
            set(ps, 'Color', obj.re);
                
            set([ph,pw,ps],'LineWidth', 3);
            grid on;
            l = legend([ph,pw,ps],...
                sprintf('At Home - %d',max(obj.Ho)), ...
                sprintf('At Work - %d',max(obj.Wo)), ...
                sprintf('At School - %d',max(obj.Sc)));
            set(l, 'FontSize', 16);
            xlabel('Time','Interpreter', 'Latex', 'FontSize', 16)
            ylabel('Population','Interpreter', 'Latex', 'FontSize', 16)
            hold off;
        end
        function MakeMovie(obj, fignum)
            
            for ii = 1:length(obj.Locations)
                obj.DrawPlace(obj.Locations(ii));
            end
            
            if isempty(obj.People)
                obj.getPerson(1);
            end
            
            t = obj.People(1).Time;
            loops = length(t);
            
            frames(loops) = struct('cdata',[],'colormap',[]);
            wbhm = waitbar(0,'Making a movie. Please wait...');
            for ii = 1:loops
                h = obj.Render(fignum, ii);
                frames(ii) = getframe(h);
                waitbar(ii/loops,wbhm,sprintf('%2.1f %% of this step complete...',100 * ii/loops))
            end
            close(wbhm)
            obj.Frames = frames;
        end
        
        function r = isMember(~, list,  id)
            r = sum(list == id);
        end
        function ShadeMyFig(~, t, levels, c)
            s = size(t);
            if s(1) > 1
                x = t';
            end
           
            X=[x,fliplr(x)];                %#create continuous x value array for plotting
            for ii = 1:length(levels)
                if ii == 1
                    y1 = zeros(size(x));
                else
                    y1 = levels(ii-1)*ones(size(x));
                end
                y2 = levels(ii)*ones(size(x));
                Y=[y1,fliplr(y2)];              %#create y values for out and then back
                fill(X,Y,c(ii,:), 'FaceAlpha', 0.05,...
                    'FaceLighting', 'gouraud', ...
                    'EdgeColor', c(ii,:)); 
                hold on
            end
            hold off
        end
    end
    
end
