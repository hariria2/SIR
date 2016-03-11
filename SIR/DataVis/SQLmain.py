from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

#vis = SQLVisualization('root','','localhost','sim_v2_2_22_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_2_26_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_4_2016')
vis = SQLVisualization('root','','localhost','sim_v3_3_6_2016')


vis._AllInfected = False;  # Exposed + Infected. I usually want False
vis._AllPopulations = False;
vis._PeaksOnly = True;

vis.getHistoryData()


vis.countPeakes();

vis.PlotHistory(1)
vis.PlotHistogram(3)
vis.PlotLog(4)


#vis.PlotIndividual(2, [1,11,476])
vis.Render()

"""
 + Increase the number of population until we get a power law
 + Then reduce the number of population to see how it affects the power law
 + Change other variabilities to see if they affect the powe law
 + In the limit if everyone was the same, everyone gets sick as soon as a sick is introduced.

"""
