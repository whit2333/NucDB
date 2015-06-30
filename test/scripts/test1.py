from ROOT import gROOT,gSystem
gSystem.Load( 'libFoam' )
gSystem.Load( 'libTreeViewer' )
gSystem.Load( 'libInSANE' )
gSystem.Load( 'libNucDB' )

import os
from ROOT import TCanvas,NucDBManager
manager = NucDBManager.GetManager()

exp=manager.GetExperiment("SLAC_E143")

exp.PlotMeasurements()

#g1n=exp.GetMeasurement("A1n")
#g1n.BuildGraph()
#g1n.fGraph.Draw("ap")

