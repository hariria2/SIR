from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v1_1_5_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_1_5_2016')
#vis = SQLVisualization('root','','localhost','sim_v3_1_5_2016')
#vis = SQLVisualization('root','','localhost','sim_v4_1_5_2016')

vis._AllInfected = True;
vis._AllPopulations = False;

vis.getHistoryData()
vis.countPeakes();
vis.PlotHistory(1)
#vis.PlotIndividual(2, [1,11,476])
vis.PlotHistogram(3)
vis.PlotLog(4)
vis.Render()
