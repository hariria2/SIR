from SQLVisualization import SQLVisualization
import matplotlib.pyplot as plt
from SIR import SIR

#vis = SQLVisualization('root','','localhost','sim_v1_3_4_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_3_20_2016')
#vis = SQLVisualization('root','','localhost','sim_v2_3_21_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_22_2016')
#vis = SQLVisualization('root','','localhost','sim_v4_3_23_2016')
#vis = SQLVisualization('root','','localhost','sim_v1_3_26_2016')
vis = SQLVisualization('root','','localhost','sim_v1_4_19_2016')
#sir = SIR(0,10,.1,.2,0.3,100);
#sir.Initialize(999,1,0);
#sir.Simulate();
#sir.PlotSIR(1);


vis._AllInfected = False;  # Exposed + Infected. I usually want False
vis._AllPopulations = True;
vis._PeaksOnly = False;
vis.getHistoryData()

popOnly = False;

vis.PlotHistory(1)
vis.Render()
