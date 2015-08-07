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
        FullScreen = 0;
        TracePerson = 0;
        Frames;
        polyxs;
        polyys;
        colors;
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
            
            
            wbh = waitbar(0,'Loading People. Please wait...');
            for ii = 1:length(ids)
                iah = cell2mat(curs1.data(ii, [1, 3, 5]));
                ng  = cell2mat(curs1.data(ii, [2,4]));
                % ===============>>>> FIX THIS HACK <<<<=================
                p = Person(iah(1), ng(1:end-1), iah(2), ng(end), iah(3));
          
                id = ids(ii);

                txy = Alltxy(Alltxy(:,1)==id,:);
                
                state = cell2mat(myData(:,7));
            
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
                
                waitbar(ii/l,wbh,sprintf('%2.1f %% of loading people data is done...',100 * ii/l))
                clear p;
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

        function PlotHistory(obj, fignum)
           figure(fignum)
           p = plot(obj.T, obj.S,'b',...
                    obj.T, obj.I,'y',...
                    obj.T, obj.P,'r',...
                    obj.T, obj.R,'g');
     
            set(p,'LineWidth', 3);
            grid on;
            l = legend('Susceptible', 'Exposed', 'Infected', 'Recovered');
            set(l, 'FontSize', 16);
            xlabel('Time','Interpreter', 'Latex', 'FontSize', 16)
            ylabel('Population','Interpreter', 'Latex', 'FontSize', 16)
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
            cval = b/obj.MaxInfLev;
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
                color = [cval,1-cval,0];
            else
                color = [cval,0,1-cval];
            end
            
            p = plot(person.x(t),person.y(t), '.', 'MarkerSize',20);
            set(p,'Color', color);
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
            title(sprintf('Time = %d',t),'fontsize',22)
            
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
                ymax = max(ymax,max([max(SC),max(IC), max(VL)]));
            end
            
            for ii = 1:length(ppl)
                p = obj.People(obj.PeopleIDs==ppl(ii));
                SC = p.SusCells;
                IC = p.InfCells;
                VL = p.VirLoads;
                t = p.Time;
                subplot(length(ppl),1,ii)
                plot(t, SC, 'b', t, IC, 'r', t, VL, 'g', 'linewidth', 3)
                ylabel(sprintf('ID: %d',ppl(ii)),'FontSize', 16);
                grid on
                ylim([0, ymax])
            end
            %subplot(pltnum,1,1)
            l = legend ('Susceptible cells','Infected Cells', 'Free Virion');
            newPosition = [0.31 0.6 0.4 0.73];
            newUnits = 'normalized';
            set(l,'Position', newPosition,'Units', newUnits, 'Orientation','horizontal', 'FontSize', 18);
            %set(l, 'FontSize', 18, 'Location', 'NorthOutside','Orientation','horizontal')
            
            subplot(length(ppl),1,length(ppl))
            xlabel('Time', 'FontSize', 16)
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
    end
    
end
