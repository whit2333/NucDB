from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

manager = NucDBManager.GetManager()
manager.ListExperiments()
manager.ListMeasurements()

experiment = manager.GetExperiment("RSS")
if not experiment :
    experiment = NucDBExperiment("RSS","RSS")

Aperp = experiment.GetMeasurement("Aperpp")
if not Aperp :
    Aperp = NucDBMeasurement("Aperpp","A_{\perp}^{p}")
Apara = experiment.GetMeasurement("Aparap")
if not Apara :
    Apara = NucDBMeasurement("Aparap","A_{\para}^{p}")

experiment.fMeasurements.Add(Aperp)
experiment.fMeasurements.Add(Apara)

AparaExtractor = NucDBRawDataExtractor()
AparaExtractor.SetMeasurement(Apara)
AparaExtractor.SetInputFile("experiments/RSS/rss_proton_b2.dat")
AparaExtractor.linestoskip=115

AparaExtractor.Initialize()

AparaExtractor.ExtractAllValues()

Apara.BuildGraph()



#datapoint = NucDBDataPoint(1.0,0.2)
#Aperp.AddDataPoint(datapoint)
experiment.Print()

manager.SaveExperiment(experiment)



