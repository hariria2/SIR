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
    plt.rc('font', family='serif')

    def __init__(self,usrnm,psswd,hst,db):
        cnx = mysql.connector.connect(user=usrnm, password=psswd,host=hst,database=db)
        self._Connection = cnx
        self._Cursor = cnx.cursor()
        self._People = []
        self._PersonIDs = []
        self._Locations = []
        self._LocationIDs = []

        self.gr = '#32AF4B'
        self.re = '#AF324B'
        self.bl = '#323BAF'
        self.yl = '#AFAF4B'


    def isMember(self,list, elt):
        for m in list:
            if elt == m:
                return True
        return False

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
    def getPerson(self, ids):
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
            print sqlquery2
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
        ps = plt.plot(self.T, self.S, label="Susceptible")
        plt.setp(ps, 'Color', self.bl, 'LineWidth', 4)
        pi = plt.plot(self.T, [a+b for a,b in zip(self.P, self.I)], label="Infected")
        plt.setp(pi, 'Color', self.re, 'LineWidth', 4)
        pr = plt.plot(self.T, self.R, label="Recovered")
        plt.setp(pr, 'Color', self.gr, 'LineWidth', 4)
        #pd = plt.plot(self.T, self.D, label="Dead")
        #plt.setp(pd, 'Color', 'k', 'LineWidth', 4)
        plt.grid(True)
        plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,ncol=4, mode="expand", borderaxespad=0.)
        #set(l, 'FontSize', 16)
        plt.xlabel(r'Time', fontsize=18)
        plt.ylabel(r'Population', fontsize=18)

    def PlotIndividual(self,fignum, ppl):

        h = plt.figure(fignum)
        ymax = 0
        ymin = 0


        for ii, pe in enumerate(ppl):

            if self.isMember(self._PersonIDs, pe):
                p = self._People(ii);
            else:
                self.getPerson([pe]);
                p = self._People[ii];

            SC = p._SC;
            IC = p._IC;
            VL = p._VL;


        for ii, pe in enumerate(ppl):
            p = self._People[ii]
            SC = p._SC;
            IC = p._IC;
            VL = p._VL;
            t = p._Time;

            ymax = max(ymax, max(SC))
            ymin = min(ymin, min(SC))
            ymin = min(ymin, min(VL))

            ax1 = h.add_subplot(len(ppl),1,ii+1)
            p1 = ax1.plot(t, SC,label="Scuseptible Cells")
            plt.setp(p1, 'Color', self.bl, 'linewidth', 3)
            p2 = ax1.plot(t, IC,label="Infected Cells")
            plt.setp(p2, 'Color',self.re, 'linewidth', 3)
            plt.ylim(ymax=ymax)
            plt.ylim(ymin=ymin)
            plt.ylabel(r'ID $%d$' % self._PersonIDs[ii],fontsize=18)
            if ii+1 == len(ppl):
                plt.xlabel(r"Time", fontsize=18)
            ax2 = ax1.twinx()
            p3 = ax2.plot(t, VL, '--', label="Viral Load")
            plt.setp(p3, 'Color', self.gr, 'linewidth', 3)
            for tl in ax2.get_yticklabels():
                tl.set_color(self.gr)

            ax1.grid(True)

            ax1.axhspan(ymin, 0.01, color=(0,0.9,0), alpha=0.1, lw=0)
            ax1.axhspan(0.01, 0.3, color=(0.9,0.9,0), alpha=0.1, lw=0)
            ax1.axhspan(0.3, 2.8, color=(0.9,0,0), alpha=0.1, lw=0)
            ax1.axhspan(2.8, ymax, color=(0.0,0.0,0.0), alpha=0.1, lw=0)

            if ii == 0:
                lines1, labels1 = ax1.get_legend_handles_labels()
                lines2, labels2 = ax2.get_legend_handles_labels()
                ax2.legend(lines1 + lines2, labels1 + labels2, bbox_to_anchor=(0., 1.02, 1., .102), loc=3,ncol=3, mode="expand", borderaxespad=0.)
