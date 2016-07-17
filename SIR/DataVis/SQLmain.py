from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt
from SIR import SIR


def getError(databases=[],plotQ=False, fignum=1):
    errors = []
    l = 0
    for idx, db in enumerate(databases):
        l += 1
    if plotQ:
        plt.figure(fignum)
    for idx, db in enumerate(databases):
        vis = SQLVisualization('root','','localhost',db)
        vis.getHistoryData()
        vis.getMonthlyData()
        vis.getMonthlyPeaks()
        vis.countPeakes()
        vis.ComputeProb()
        vis.doLinearRegression()
        errors.append(vis._R2)
        if plotQ:
            plt.subplot(1,l,idx+1)
            vis.MPlotLog(fignum)
    return errors
def myPlot(fignum=1,x=[],y=[],xl='',yl='',t='',c='k'):
    plt.figure(fignum)
    p = plt.plot(x,y,'o-')
    plt.ylabel(yl,fontsize=18)
    plt.xlabel(xl,fontsize=18)
    plt.setp(p, 'Color', c, 'LineWidth', 2)
    plt.title(t)
    plt.grid(True)
def myStem(fignum=1,x=[],y=[],xl='',yl='',t='',c='k'):
    plt.figure(fignum)
    p = plt.stem(x,y)
    plt.ylabel(yl,fontsize=18)
    plt.xlabel(xl,fontsize=18)
    plt.setp(p, 'Color', c, 'LineWidth', 2)
    plt.title(t)
    plt.grid(True)

#vis = SQLVisualization('root','','localhost','sim_v1_3_4_2016')
vis = SQLVisualization('root','','localhost','sim_v3_7_17_2016')

fignum = 1
#sir = SIR(0,10,.1,.2,0.3,100);
#sir.Initialize(999,1,0);
#sir.Simulate();
#sir.PlotSIR(fignum);
#fignum += 1

vis._AllInfected = False;    # Exposed + Infected. I usually want False
vis._AllPopulations = False; # These both need fixing in PlotHistory.
vis._PeaksOnly = False;      # These both need fixing in PlotHistory.
vis.getHistoryData()
popOnly = False;

if popOnly:
    vis.PlotHistory(fignum)
    fignum += 1
    myPlot(fignum,x=vis.T,y=vis.N,xl='Time (days)',yl='Number of infection incidences',t='',c='k')
    fignum += 1
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
v1 -> 0.1
v2 -> 0.05
v3 -> 0.025
'''
errors = getError(['sim_v3_7_5_2016','sim_v2_7_5_2016','sim_v1_7_5_2016','sim_v4_7_5_2016'],True,fignum)
rr = [0.025,0.05,0.1,0.2]
fignum += 1
myPlot(fignum,x=rr,y=errors,xl='Motion step size',yl='r-squared values',t='',c='k')
fignum += 1

#vis.MPlotLog(2)

#plt.figure(3)
#pi = plt.plot(vis.MT,vis.MN,'k')
#plt.xlabel("Time (Days)",fontsize=18)
#plt.ylabel('Number of infection incidences',fontsize=18)

vis.Render()
