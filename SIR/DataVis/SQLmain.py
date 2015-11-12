from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt

vis = SQLVisualization('root','','localhost','sim_v1_11_10_2015')

vis.getHistoryData()
vis.PlotHistory(1)
vis.PlotIndividual(2, [1,10,32,306])
vis.PlotHistogram(3)
vis.Render()
