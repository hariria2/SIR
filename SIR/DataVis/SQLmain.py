from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v1_10_22_2015')

vis.getHistoryData()
vis.PlotHistory(1)
vis.PlotHistory(2)
vis.Render()
