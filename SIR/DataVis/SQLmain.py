from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v1_11_6_2015')

vis.getHistoryData()
vis.PlotHistory(1)

vis.PlotIndividual(2, [1,40, 34, 401])

vis.Render()
