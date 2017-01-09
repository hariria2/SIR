import mysql.connector
import matplotlib.pyplot as plt
import networkx as nx
from math import floor, log10
from matplotlib import rc
from ProjectClasses import Location, Person
import numpy as np
import collections
from scipy import stats
from itertools import chain

class SQLVisualization:

    plt.rc('font', family='serif')

    def __init__(self,usrnm,psswd,hst,db):
        cnx = mysql.connector.connect(user=usrnm, password=psswd,host=hst,database=db)
        self._Connection = cnx
        self._Cursor = cnx.cursor()
        self._People = []
        self._PersonIDs = []
        self._Locations = []
        self._LocationIDs = []
        self._AllInfected = False
        self._AllPopulations = True;
        self._PeaksOnly = True;
        self._Peaks = [];
        self._MPeaks = [];
        self._PeakTimes = [];
        self._MPeakTimes = [];
        self._PeakRes = 1.
        self._Prob = [];
        self._R2 = 0;
        self._stderr = 0;
        self._network = [];
        self.gr = '#32AF4B'
        self.re = '#AF324B'
        self.bl = '#323BAF'
        self.yl = '#AFAF4B'
        self.bk = '#000000'

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
        self.B = [row[7] for row in data]
        self.N = [row[8] for row in data]
    def getMonthlyData(self):
        sqlquery = 'SELECT * FROM HistoryData';
        self._Cursor.execute(sqlquery);
        data = self._Cursor.fetchall();
        self.MT = []
        self.MN = []
        t = 0;
        while t < self.T[-1]-30:
            self.MT.append(self.T[t])
            self.MN.append(sum(self.N[t:t+30]))
            t += 30
    def getMonthlyPeaks(self):
        peaks = []
        peaktimes = []
        peak = self.MN[0];
        for idx,pq in enumerate(self.MN):
            if pq > 0:
                peak += pq
            else:
                peaks.append(peak)
                peaktimes.append(self.MT[idx])
                peak = 0

        self._MPeaks = [x for x in peaks if x !=0]
        self._MPeakTimes = [x for x,y in zip(peaktimes, peaks) if y !=0]
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

        # data1 - id, name, age, gender, homeID, locatioinID
        # data2 - index, PersID, time, x, y, LocID, State, HBS, S, I, V, MaxI
        peeps = []
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
            p.setAge([x[3] for x in alldata])
            p.setX([x[4] for x in alldata])
            p.setY([x[5] for x in alldata])
            p.setLocID([x[6] for x in alldata])
            p.setState([x[7] for x in alldata])
            p.setConnections([x[8] for x in alldata])
            p.setHBS([x[9] for x in alldata])
            p.setSC([x[10] for x in alldata])
            p.setIC([x[11] for x in alldata])
            p.setVL([x[12] for x in alldata])
            if not (id in self._PersonIDs):
                self._People.append(p)
                self._PersonIDs.append(id)
            peeps.append(p)
        return peeps
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
    def countPeakes(self):
        te = self.T[-1]
        dt = self.T[1]-self.T[0]
        n  = int(floor(te/(self._PeakRes*dt)));
        ii = 0

        if self._AllInfected:
            l = [a+b for a,b in zip(self.P, self.I)]
        else:
            l = [x for x in self.N]
            #l = [a+b for a,b in zip(self.P, self.I)]

        while ii < n:
            ii = self.peakQ(l,ii,dt,0)
            ii = ii + 1
    def peakQ(self,data,ii,dt,ps):

        s = max(data[int(floor(ii*self._PeakRes/dt)):int(floor((1+ii)*self._PeakRes/dt))])
        ps = sum(data[int(floor(ii*self._PeakRes/dt)):int(floor((1+ii)*self._PeakRes/dt))]);
        n = 0;
        # The end bit of the while loop logic is to account for the case where simulation ends in the middle of an epidemic.
        while s > 1 and int(floor((1+ii)*self._PeakRes/dt))<len(self.T):


            ii = ii + 1
            """
            if int(floor((1+ii)*self._PeakRes/dt)) > len(self.T):
                s  = max(data[int(floor(ii*self._PeakRes/dt)):len(self.T)])
                nn = sum(self.N[int(floor(ii*self._PeakRes/dt)):len(self.T)])
            else:
                s   = max(data[int(floor(ii*self._PeakRes/dt)):int(floor((1+ii)*self._PeakRes/dt))])
                nn  = sum(self.N[int(floor(ii*self._PeakRes/dt)):int(floor((1+ii)*self._PeakRes/dt))])
            """
            s   = max(data[int(floor(ii*self._PeakRes/dt)):int(floor((1+ii)*self._PeakRes/dt))])
            nn  = sum(data[int(floor(ii*self._PeakRes/dt)):int(floor((1+ii)*self._PeakRes/dt))])
            ps = ps + nn
            n  = n + 1;

        if s > 0:
            self._Peaks.append(ps)
            self._PeakTimes.append(self.T[int((ii-floor(n/2.))*self._PeakRes/dt)])
        return ii
    def Render(self):
        plt.show()
    def PlotHistory(self, fignum):
        h = plt.figure(fignum);
        if self._AllPopulations:
            ps = plt.plot(self.T, self.S, label="Susceptible")
            plt.setp(ps, 'Color', self.bl, 'LineWidth', 4)
            pr = plt.plot(self.T, self.R, label="Recovered")
            plt.setp(pr, 'Color', self.gr, 'LineWidth', 4)
        if self._AllInfected:
            pi = plt.plot(self.T, [a+b for a,b in zip(self.P, self.I)],label="Infected")
            pn = plt.plot(self.T, self.N,label="Newly Infected")
            if self._PeaksOnly:
                pe = plt.plot(self._PeakTimes, self._Peaks,'ko',label="Peaks")
        else:
            pi = plt.plot(self.T, self.P, label="Infected")
            plt.setp(pi, 'Color', self.re,'LineWidth', 4)
            pn = plt.plot(self.T, self.D, label="Dead People")
            plt.setp(pn, 'Color', self.bk,'LineWidth', 4)
            ps = plt.plot(self.T, self.S, label="Susceptible People")
            plt.setp(ps, 'Color', self.bl,'LineWidth', 4)
            pr = plt.plot(self.T, self.R, label="Recovered People")
            plt.setp(pr, 'Color', self.gr,'LineWidth', 4)
            pb = plt.plot(self.T, self.B, label="Newborn People")
            plt.setp(pb, 'Color', 'm','LineWidth', 4)
            pa = plt.plot(self.T, [d+i+s+r+p+b for d,i,s,r,p,b in zip(self.D,self.I,self.S,self.R,self.P,self.B)], label="Whole population")
            plt.setp(pa, 'Color', 'c','LineWidth', 4)
            if self._PeaksOnly:
                pe = plt.plot(self._PeakTimes, self._Peaks,'ko',label="Peaks")

        #plt.xticks(np.linspace(0,36000,5),np.linspace(0,100,5))

        plt.grid(True)
        plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,ncol=4, mode="expand", borderaxespad=0.)

        plt.xlabel(r'Time', fontsize=18)
        plt.ylabel(r'Population', fontsize=18)
    def PlotHistogram(self, fignum):
        h = plt.figure(fignum);

        if self._PeaksOnly:
            l = self._Peaks
        else:
            if self._AllInfected:
                l = [a+b for a,b in zip(self.P, self.I) if a+b != 0]
                #l = [x for x in self.N if x != 0]
            else:
                l = [x for x in self.N if x != 0]

        plt.hist(l,20,color=self.bk)
        plt.xlabel(r'Epidemic Size', fontsize=18)
        plt.ylabel(r'Number of Occurences', fontsize=18)
        plt.grid(True)
    def PlotLog(self, fignum):
        h = plt.figure(fignum);

        if self._PeaksOnly:
            l = self._Peaks
        else:
            if self._AllInfected:
                l = sorted([a+b for a,b in zip(self.P, self.I) if a+b != 0])
                #l = sorted([x for x in self.N if x != 0]);
            else:
                l = sorted([x for x in self.N if x != 0]);

        counter=collections.Counter(l)
        vals = [log10(x) for x in counter.keys()]
        freq = [log10(x) for x in counter.values()]
        ml = int(floor(len(freq)*0.8))
        slope, intercept, r_value, p_value, std_err = stats.linregress(vals[0:ml],freq[0:ml])
        vals = counter.keys()
        freq = counter.values()
        pi = plt.plot(vals, freq,'k.') #this is a hack. Get rid of it.
        pi = plt.plot(vals, freq)

        pl = plt.plot(vals, [10**(slope*log10(x)+intercept) for x in vals])

        plt.xscale('log')
        plt.yscale('log')
        plt.setp(pi, 'Color', self.bk, 'LineWidth', 2)
        plt.setp(pl, 'Color', self.re, 'LineWidth', 4, 'linestyle','--')
        plt.xlabel(r'Epidemic Size', fontsize=18)
        plt.ylabel(r'Number of Occurances', fontsize=18)
        plt.title(r'Slope: %2.2f' %slope, fontsize=18)
        plt.grid(True)
    def MPlotHistogram(self, fignum):
        h = plt.figure(fignum);

        l = self._MPeaks

        plt.hist(l,20,color=self.bk)
        plt.ylabel(r'Number of Occurences', fontsize=18)
        plt.grid(True)
    def doLinearRegression(self):
        l = self._MPeaks

        counter=collections.Counter(l)
        cc = collections.OrderedDict(sorted(counter.items()))
        vals = cc.keys()

        ml = int(floor(len(vals)*0.95))
        xl = [log10(x) for x in vals[0:ml]]
        yl = [log10(x) for x in self._Prob[0:ml]]
        slope, intercept, r_value, p_value, std_err = stats.linregress(xl,yl)
        self._R2 = r_value**2;
        self._stderr = std_err;
    def MPlotLog(self, fignum):
        h = plt.figure(fignum);

        l = self._MPeaks

        counter=collections.Counter(l)
        cc = collections.OrderedDict(sorted(counter.items()))
        vals = cc.keys()

        ml = int(floor(len(vals)*0.95))
        xl = [log10(x) for x in vals[0:ml]]
        yl = [log10(x) for x in self._Prob[0:ml]]
        slope, intercept, r_value, p_value, std_err = stats.linregress(xl,yl)
        self._R2 = r_value**2;
        self._stderr = std_err;
        pi = plt.plot(vals, self._Prob,'k.',markersize=10) #this is a hack. Get rid of it.
        pi = plt.plot(vals, self._Prob)
        #pl = plt.plot(vals, [10**(slope*log10(x)+intercept) for x in vals])
        plt.ylim((0.01,1.1))
        plt.xscale('log')
        plt.yscale('log')
        #plt.setp(pi, 'Color', self.bk, 'LineWidth', 2)
        #plt.setp(pl, 'Color', self.re, 'LineWidth', 4, 'linestyle','--')
        plt.xlabel(r'Epidemic Size', fontsize=18)
        plt.ylabel(r'P(>s)', fontsize=18)
        plt.title('Slope: %2.2f, R^2: %2.4f' %(-slope+2,r_value**2), fontsize=18)
        plt.grid(True)
    def ComputeProb(self):
        l = self._MPeaks
        counter=collections.Counter(l)
        cc = collections.OrderedDict(sorted(counter.items()))
        sp = cc.keys()
        ns = cc.values()
        ssp = sum([n for s, n in zip(sp,ns)]);
        for ii in range(len(sp)):
            sspi = sum([n for s, n in zip(sp[ii:len(sp)],ns[ii:len(sp)])])
            self._Prob.append((float(sspi)/float(ssp)))
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
    def getGraph(self,ids, time):
        peeps = self.getPerson(ids)
        graph = []
        for t in time:
            for p in peeps:
                for con in p._Connections[t]:
                    graph.append(con)
        # extract nodes from graph
        nodes = set([n1 for n1, n2 in graph] + [n2 for n1, n2 in graph])
        # create networkx graph
        G=nx.Graph()
        # add nodes
        for node in nodes:
            G.add_node(node)
        # add edges
        for edge in graph:
            G.add_edge(edge[0], edge[1])
        return G
    def getConnectivity(self,G):
        return nx.average_node_connectivity(G);
    def getClustering(self,G):
        return nx.average_clustering(G);
    def draw_graph(self,G,fignum):
        h = plt.figure(fignum);
        # draw graph
        pos = nx.shell_layout(G)
        nx.draw(G, pos)
    def draw_graph_detailed(self, G, fignum, labels=None, graph_layout='shell',
                   node_size=1600, node_color='blue', node_alpha=0.3,
                   node_text_size=12,
                   edge_color='blue', edge_alpha=0.3, edge_tickness=1,
                   edge_text_pos=0.3,
                   text_font='sans-serif'):
        h = plt.figure(fignum);

        # these are different layouts for the network you may try
        # shell seems to work best
        if graph_layout == 'spring':
            graph_pos=nx.spring_layout(G)
        elif graph_layout == 'spectral':
            graph_pos=nx.spectral_layout(G)
        elif graph_layout == 'random':
            graph_pos=nx.random_layout(G)
        else:
            graph_pos=nx.shell_layout(G)

        # draw graph
        nx.draw_networkx_nodes(G,graph_pos,node_size=node_size,
                               alpha=node_alpha, node_color=node_color)
        nx.draw_networkx_edges(G,graph_pos,width=edge_tickness,
                               alpha=edge_alpha,edge_color=edge_color)
        nx.draw_networkx_labels(G, graph_pos,font_size=node_text_size,
                                font_family=text_font)

        '''
        if labels is None:
            labels = range(len(graph))

        edge_labels = dict(zip(graph, labels))
        nx.draw_networkx_edge_labels(G, graph_pos, edge_labels=edge_labels,
                                     label_pos=edge_text_pos)
        '''
        # show graph
        # plt.show()
    def degree_dist(self,G,fignum):
        h = plt.figure(fignum);
        degree_sequence=sorted(nx.degree(G).values(),reverse=True) # degree sequence
        #print "Degree sequence", degree_sequence
        dmax=max(degree_sequence)

        counter=collections.Counter(degree_sequence)
        cc = collections.OrderedDict(sorted(counter.items()))
        x = cc.keys()
        y = cc.values()

        ml = int(floor(len(x)*0.8))
        #xl = [log10(xx) for xx in x[0:ml]]
        #yl = [log10(xx) for xx in self._Prob[0:ml]]

        pi = plt.loglog(x,y,'o-')
        #plt.ylim((0.01,1.1))
        #plt.xscale('log')
        #plt.yscale('log')

        #plt.loglog(degree_sequence,'b-',marker='o')
        plt.title("Degree rank plot")
        plt.ylabel("degree")
        plt.xlabel("rank")

        # draw graph in inset
        plt.axes([0.45,0.45,0.45,0.45])
        Gcc=sorted(nx.connected_component_subgraphs(G), key = len, reverse=True)[0]
        pos=nx.spring_layout(Gcc)
        plt.axis('off')
        nx.draw_networkx_nodes(Gcc,pos,node_size=20)
        nx.draw_networkx_edges(Gcc,pos,alpha=0.4)
    def draw_person_connections(self, G, fignum):
        self.draw_graph_detailed(G,fignum,graph_layout='spring',node_size=500)
        #self.degree_dist(graph,fignum)
    def draw_degree_distribution(self, G, fignum):
        self.degree_dist(G,fignum)
