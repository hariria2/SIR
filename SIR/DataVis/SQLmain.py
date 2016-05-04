from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v1_5_4_2016')
vis.getHistoryData()
vis.PlotHistory(1)
vis.PlotIndividual(2, [1,2,3])
vis.Render()
