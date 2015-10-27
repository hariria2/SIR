from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v1_10_26_2015')

vis.getHistoryData()
vis.PlotHistory(1)

vis.PlotIndividual(2, [1,12, 56, 78])

vis.Render()
