from Utility import *

vis = SQLVisualization('root','','localhost','sim_v2_9_11_2016')

fignum = 1

vis._AllInfected    = False;    # Exposed + Infected. I usually want False
vis._AllPopulations = False;    # These both need fixing in PlotHistory.
vis._PeaksOnly      = False;    # These both need fixing in PlotHistory.
popOnly             = False;
drawGraph           = False;
drawPowerLaw        = True;
drawError           = True;

vis.getHistoryData()

'''
########## SIR ###############
'''
#sir = SIR(0,10,.1,.2,0.3,100);
#sir.Initialize(999,1,0);
#sir.Simulate();
#sir.PlotSIR(fignum);
#fignum += 1
'''
########## SIR ###############
'''

'''
########## GRAPH ###############
'''
if drawGraph:
    alivepeople=[x for x in range(1,10) if x not in [7,23]]
    timerange = [1,10]
    G = vis.getGraph(alivepeople,timerange);
    cn = vis.getConnectivity(G)
    cl = vis.getClustering(G)
    print "Connectivity: %3.3f"%cn
    print "Clustering: %3.3f"%cl
    print (cn*cl)
    print (cn/cl)
    vis.draw_degree_distribution(G,fignum)
    fignum += 1
    vis.draw_person_connections(G,fignum)
    fignum += 1
'''
########## GRAPH ###############
'''

'''
########## POWER LAW ###############
'''
if drawPowerLaw:
    if popOnly:
        vis.PlotHistory(fignum)
        fignum += 1
        #myPlot(fignum,x=vis.T,y=vis.N,xl='Time (days)',yl='Number of infection incidences',t='',c='k')
        #fignum += 1
        #vis.PlotIndividual(fignum, [1,3])
        #fignum += 1
    else:
        vis.PlotHistory(fignum)
        fignum += 1
        vis.getMonthlyData()
        vis.getMonthlyPeaks()
        vis.countPeakes();
        vis.ComputeProb()

        plt.figure(fignum)
        plt.subplot(2,2,1)
        vis.MPlotHistogram(fignum)
        plt.subplot(2,2,3)
        vis.MPlotLog(fignum)
        plt.subplot(2,2,2)
        myPlot(fignum,vis.MT,vis.MN,'','Number of infection incidences','',vis.re)
        plt.subplot(2,2,4)
        myStem(fignum,vis._MPeakTimes,vis._MPeaks,'Time (days)','Epidemic size','','k')
        fignum += 1
'''
########## POWER LAW ###############
'''

'''
########## ERROR ###############
'''
if drawError:
    errors =    getError(['sim_v1_8_1_2016','sim_v2_8_1_2016','sim_v4_8_1_2016','sim_v5_8_1_2016','sim_v3_8_1_2016','sim_v2_8_2_2016','sim_v1_8_2_2016','sim_v6_8_1_2016'],True,fignum)
    rr = [1,2,3,4,5,6,7,8]
    fignum += 1
    myPlot(fignum,x=rr,y=errors,xl='Spatial Variation',yl='r-squared values',t='',c='k')
    fignum += 1

    # (8.93,0.88,7.89,10.12), (5.41,0.737,3.98,7.34), (1.648,0.154,0.25,10.72),(0.592,0.215,0.126,2.75),(1.32,0.185,0.245,7.18),(1.05,0.682,0.722,1.55),(1.91,0.40,0.76,4.79),(2.11,0.488,1.03,4.32)
'''
########## ERROR ###############
'''

vis.Render()
