import mysql.connector
import matplotlib.pyplot as plt
from matplotlib import rc
from ProjectClasses import Location, Person
import numpy as np

class SQLVisualization:

    gr = [0.2, 0.7, 0.3];
    re = [0.7, 0.2, 0.3];
    bl = [0.2, 0.3, 0.7];
    yl = [0.7, 0.7, 0.3];

    def __init__(self,usrnm,psswd,hst,db):
        cnx = mysql.connector.connect(user=usrnm, password=psswd,host=hst,database=db)
        self._Connection = cnx
        self._Cursor = cnx.cursor()
        self._People = []
        self._PersonIDs = []
        self._Locations = []
        self._LocationIDs = []

        self.gr = [0.2, 0.7, 0.3];
        self.re = [0.7, 0.2, 0.3];
        self.bl = [0.2, 0.3, 0.7];
        self.yl = [0.7, 0.7, 0.3];

    def getHistoryData(self):
        sqlquery = 'SELECT * FROM HistoryData';
        self._Cursor.execute(sqlquery);
        data = self._Cursor.fetchall();

        self.T = [row[1] for row in data]
        self.S = [row[2] for row in data]
        self.I = [row[3] for row in data]
        self.P = [row[4] for row in data]
        self.R = [row[5] for row in data]
        self.D = [row[6] for row in data]
        self.Ho = [row[7] for row in data]
        self.Wo = [row[8] for row in data]
        self.Sc = [row[9] for row in data]
        self.GDP = [row[10] for row in data]
        self.Demand = [row[11] for row in data]
    def getPerson(self, ids, t):
        query1 = '';
        query2 = '';
        if ids == 'All':
            sqlquery1 = 'SELECT * FROM People';
            if t == 'All':
                sqlquery2 = 'SELECT * FROM PersonValues';
            else:
                self._T0 = t[0]
                sqlquery2 = 'SELECT * From PersonValues Where Time > %d and Time < %d' %(t[0],t[1])

            self._Cursor.execute(sqlquery1)
            data1 = self._Cursor.fetchall()

            self._Cursor.execute(sqlquery2)
            data2 = self._Cursor.fetchall()

            l = len(data1)

            ids = range(1,l+1)
            self.ShowProgress = 1
        else: # Make sure you add time picking to this case
            l = len(ids);

            for ii in range(0,l):
                if ii == l-1:
                    query1 = query1 + 'ID = ' + str(ids[ii]);
                else:
                    query1 = query1 + 'ID = ' + str(ids[ii]) + ' or ';

            for ii in range(0,l):
                if ii == l-1:
                    query2 = query2 + 'PersonID = ' + str(ids[ii]);
                else:
                    query2 = query2 + 'PersonID = ' + str(ids[ii]) + ' or ';

            sqlquery1 = 'SELECT * FROM People Where ' + query1;
            sqlquery2 = 'SELECT * FROM PersonValues Where ' + query2;


            self._Cursor.execute(sqlquery1);
            data1 = self._Cursor.fetchall();
            self._Cursor.execute(sqlquery2);
            data2 = self._Cursor.fetchall();

        # data1 - id, name, age, gender, homeID, locatioinID
        # data2 - index, PersID, time, x, y, LocID, State, HBS, S, I, V, MaxI
        for idx in range(l):
            id = ids[idx]
            alldata = [x for x in data2 if x[1] == id]
            name = data1[idx][1]
            age = data1[idx][2]
            gender = data1[idx][3]
            homeid = data1[idx][4]
            maxi = data2[idx][11]
            p = Person(id,name, age, gender, homeid, maxi)
            p.setTime([x[2] for x in alldata])
            p.setX([x[3] for x in alldata])
            p.setY([x[4] for x in alldata])
            p.setLocID([x[5] for x in alldata])
            p.setState([x[6] for x in alldata])
            p.setHBS([x[7] for x in alldata])
            p.setSC([x[8] for x in alldata])
            p.setIC([x[9] for x in alldata])
            p.setVL([x[10] for x in alldata])
            if not (id in self._PersonIDs):
                self._People.append(p)
                self._PersonIDs.append(id)
    def getLocation(self, ids):

            query1 = '';
            if ids == 'All':
                sqlquery1 = 'SELECT * FROM Location';

                self._Cursor.execute(sqlquery1);
                data1 = self._Cursor.fetchall();

                l = len(data1);

                ids = range(1,l+1);

            else:
                l = len(ids);

                for ii in range(0,l):
                    if ii == l-1:
                        query1 = query1 + 'ID = ' + str(ids[ii]);
                    else:
                        query1 = query1 + 'ID = ' + str(ids[ii]) + ' or ';

                sqlquery1 = 'SELECT * FROM Location Where ' + query1;


                self._Cursor.execute(sqlquery1);
                data1 = self._Cursor.fetchall();

            for idx in range(0,l):

                id = ids[idx]
                alldata = [x for x in data1 if x[0] == id]

                name = alldata[0][1]
                ltype = alldata[0][2]
                xmin = alldata[0][3]
                xmax = alldata[0][4]
                ymin = alldata[0][5]
                ymax = alldata[0][6]

                d = Location(id, name, ltype, xmin, xmax, ymin, ymax)

                if not (id in self._LocationIDs):
                    self._Locations.append(d)
                    self._LocationIDs.append(id)

    def Render(self):
        plt.show()

    def PlotHistory(self, fignum):
        h = plt.figure(fignum);
        print len(self.T)
        print len(self.S)
        ps = plt.plot(self.T, self.S);
        plt.setp(ps, 'Color', self.bl, 'LineWidth', 4)
        pi = plt.plot(self.T, [a+b for a,b in zip(self.P, self.I)])
        plt.setp(pi, 'Color', self.re, 'LineWidth', 4)
        pr = plt.plot(self.T, self.R);
        plt.setp(pr, 'Color', self.gr, 'LineWidth', 4)
        pd = plt.plot(self.T, self.D);
        plt.setp(pd, 'Color', 'k', 'LineWidth', 4)
        plt.grid(True)

        #plt.setp([ps,pi,pr,pd],'LineWidth', 3);

        #l = plt.legend([ps,pi,pr,pd],sprintf('Susceptible - %d',obj.S(end)),...
            #sprintf('Infected - %d',obj.P(end)+obj.I(end)), ...
            #sprintf('Recovered - %d',obj.R(end)),...
            #sprintf('Dead - %d',obj.D(end)));
        #set(l, 'FontSize', 16);
        ##plt.xlabel('Time','Interpreter', 'Latex', 'FontSize', 16)
        #plt.ylabel('Population','Interpreter', 'Latex', 'FontSize', 16)
    def PlotIndividual(self, fignum, ppl):
        h = plt.figure(fignum);

        ymax = 0;

        for ii = range(len(ppl)):

            #if (ppl[ii] in self._PersonIDs):
            #    p = []

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
