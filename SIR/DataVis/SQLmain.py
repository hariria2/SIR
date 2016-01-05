from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt


#vis = SQLVisualization('root','','localhost','sim_v1_12_29_2015')
#vis = SQLVisualization('root','','localhost','sim_v3_1_4_2016')
vis = SQLVisualization('root','','localhost','sim_v1_1_5_2016')

vis._AllInfected = True;
vis._AllPopulations = False;

vis.getHistoryData()
vis.PlotHistory(1)
#vis.PlotIndividual(2, [1,11,476])
vis.PlotHistogram(3)
vis.PlotLog(4)
vis.Render()
