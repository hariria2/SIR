from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v1_11_15_2015')

vis.getHistoryData()
vis.PlotHistory(1)
vis.PlotIndividual(2, [1,2,3])
vis.Render()
