from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v3_12_3_2015')
#vis = SQLVisualization('root','','localhost','sim_v3_12_1_2015')

vis.getHistoryData()
vis.PlotHistory(1)
#vis.PlotIndividual(2, [1,2,3])
vis.PlotHistogram(3)
vis.PlotLog(4)
vis.Render()
