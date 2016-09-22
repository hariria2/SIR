from SQLVisualization import SQLVisualization
from SIR import SIR
import matplotlib.pyplot as plt

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
            #plt.subplot(1,l,idx+1)
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
