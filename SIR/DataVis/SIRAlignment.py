from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt
from SIR import SIR

#vis = SQLVisualization('root','','localhost','sim_v1_4_27_2016')
vis = SQLVisualization('root','','localhost','sim_v2_5_2_2016')

sir = SIR(0,8,.01,.8,1.2,100);
sir.Initialize(999,1,0);
sir.Simulate();



vis._AllInfected = False;  # Exposed + Infected. I usually want False
vis._AllPopulations = True;
vis._PeaksOnly = False;
vis.getHistoryData()

popOnly = False

plt.figure(1)
plt.subplot2grid((2,2), (0, 0), colspan=2)
sir.PlotSIR(1)
vis.PlotHistory(1)
plt.subplot(2,2,3)
sir.PlotSIR(1)
plt.subplot(2,2,4)
vis.PlotHistory(1)

#vis.PlotIndividual(2, [1,2,34,53,435])
vis.Render()
