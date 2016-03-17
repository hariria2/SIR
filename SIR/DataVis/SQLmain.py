from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

#vis = SQLVisualization('root','','localhost','sim_v2_2_22_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_2_26_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_4_2016')
#vis = SQLVisualization('root','','localhost','sim_v3_3_6_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_3_11_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_13_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_16_2016')
vis = SQLVisualization('root','','localhost','sim_v1_3_17_2016')


vis._AllInfected = False;  # Exposed + Infected. I usually want False
vis._AllPopulations = True;
vis._PeaksOnly = True;
vis.getHistoryData()

popOnly = False;


if popOnly:
    vis.PlotHistory(1)
    vis.getMonthlyData()
    plt.figure(2)
    pi = plt.plot(vis.MT,vis.MN)
    plt.ylabel('Number of infection incidences')
else:
    vis.getMonthlyData()
    vis.getMonthlyPeaks()
    vis._PeakRes = 1;

    vis.countPeakes();

    plt.figure(1)
    plt.suptitle(r'Increment: %d day intervals.' %vis._PeakRes, fontsize=18)
    plt.subplot(2,2,1)
    vis.PlotHistogram(1)
    plt.subplot(2,2,2)
    vis.PlotLog(1)
    plt.subplot2grid((2,2), (1,0), colspan=2)
    vis.PlotHistory(1)

    plt.figure(2)
    plt.suptitle(r'Increment: %d day intervals.' %vis._PeakRes, fontsize=18)
    plt.subplot(2,2,1)
    vis.MPlotHistogram(2)
    plt.subplot(2,2,3)
    vis.MPlotLog(2)
    #plt.subplot2grid((2,2), (1,0), colspan=2)
    plt.subplot(2,2,2)
    pi = plt.plot(vis.MT,vis.MN)
    plt.ylabel('Number of infection incidences')
    plt.subplot(2,2,4)
    pn = plt.stem(vis.MPeakTimes,vis.MPeaks,'k')
    plt.ylabel('Epidemic size')
    plt.setp(pi, 'Color', vis.re, 'LineWidth', 2)

    #vis.PlotIndividual(2, [1,11,476])

vis.Render()
