from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt
from SIR import SIR

#vis = SQLVisualization('root','','localhost','sim_v1_3_4_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_3_20_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_3_21_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_22_2016')
#vis = SQLVisualization('root','','localhost','sim_v4_3_23_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_26_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_5_17_2016')
#vis = SQLVisualization('root','','localhost','sim_v6_5_23_2016')
vis = SQLVisualization('root','','localhost','sim_v1_5_27_2016')

#sir = SIR(0,10,.1,.2,0.3,100);
#sir.Initialize(999,1,0);
#sir.Simulate();
#sir.PlotSIR(1);


vis._AllInfected = False;  # Exposed + Infected. I usually want False
vis._AllPopulations = True;
vis._PeaksOnly = False;
vis.getHistoryData()

popOnly = False;


if popOnly:
    vis.PlotHistory(1)
    #vis.getMonthlyData()
    #plt.figure(2)
    #pi = plt.plot(vis.MT,vis.MN)
    #plt.ylabel('Number of infection incidences')

    #vis.PlotIndividual(3, [1])

else:
    vis.PlotHistory(3)
    vis.getMonthlyData()
    vis.getMonthlyPeaks()


    vis.countPeakes();
    vis.ComputeProb()

    plt.figure(1)
    plt.subplot(2,2,1)
    vis.MPlotHistogram(1)
    plt.subplot(2,2,3)
    vis.MPlotLog(1)
    #plt.subplot2grid((2,2), (1,0), colspan=2)
    plt.subplot(2,2,2)
    pi = plt.plot(vis.MT,vis.MN)
    plt.ylabel('Number of infection incidences',fontsize=18)
    plt.subplot(2,2,4)
    pn = plt.stem(vis._MPeakTimes,vis._MPeaks,'k')
    plt.ylabel('Epidemic size',fontsize=18)
    plt.xlabel('Time (days)',fontsize=18)
    plt.setp(pi, 'Color', vis.re, 'LineWidth', 2)

#vis.MPlotLog(2)

#plt.figure(3)
#pi = plt.plot(vis.MT,vis.MN,'k')
#plt.xlabel("Time (Days)",fontsize=18)
#plt.ylabel('Number of infection incidences',fontsize=18)

vis.Render()
