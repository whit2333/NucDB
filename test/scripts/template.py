from ROOT import gROOT,gSystem
gSystem.Load( 'libFoam' )
gSystem.Load( 'libTreeViewer' )
gSystem.Load( 'libInSANE' )
gSystem.Load( 'libNucDB' )
import os
from ROOT import TCanvas,NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *


manager = NucDBManager.GetManager()

manager.ListExperiments()

manager.ListMeasurements()