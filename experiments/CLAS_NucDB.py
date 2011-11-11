from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class CLASExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=3
        self.isysErr=6
        self.istatErr=4
        self.NumberOfLines=0
        self.linesRead=0
        self.QsqMin= 0.15
        self.QsqMax= 0.27
    
    def ParseLine(self):
        """ See input file for column structures
        """
        ixMin=0
        ixMax=1
        ixbjorken=2
        self.QsqMean=float(self.QsqMin+self.QsqMax)/2.0
        #iQsq=3
        values = self.currentline.split()
        if self.linesRead >= self.NumberOfLines :
             self.rowcut.currentValue=1
             return
        deltax=float(values[ixMax])-float(values[ixMin])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(self.QsqMean),float(self.QsqMax-self.QsqMin))
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.istatErr].lstrip('+')))
        self.fCurrentDataPoint.fSystematicError.SetError(float(values[self.isysErr].lstrip('+')))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()
        self.linesRead+=1


manager = NucDBManager.GetManager()

experiment = manager.GetExperiment("CLAS")
if not experiment :
    experiment = NucDBExperiment("CLAS","CLAS")

g1p = experiment.GetMeasurement("g1p")
if not g1p :
    g1p = NucDBMeasurement("g1p","g_{1}^{p}")
    experiment.AddMeasurement(g1p)
g1p.ClearDataPoints()
g1p.fColor=6


extractor1 = CLASExtractor()
extractor1.SetMeasurement(g1p)
extractor1.SetInputFile("experiments/CLAS/g1_E91023.dat")
extractor1.linestoskip=10
extractor1.NumberOfLines=35-10
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
extractor1.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
extractor1.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
extractor1.Initialize()
extractor1.ExtractAllValues()

extractor2 = CLASExtractor()
extractor2.SetMeasurement(g1p)
extractor2.SetInputFile("experiments/CLAS/g1_E91023.dat")
extractor2.QsqMin=0.27
extractor2.QsqMax=0.5
extractor2.linestoskip=46
extractor2.NumberOfLines=83-46
extractor2.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
extractor2.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
extractor2.Initialize()
extractor2.ExtractAllValues()

extractor3 = CLASExtractor()
extractor3.SetMeasurement(g1p)
extractor3.SetInputFile("experiments/CLAS/g1_E91023.dat")
extractor3.QsqMin=0.5
extractor3.QsqMax=0.74
extractor3.linestoskip=94
extractor3.NumberOfLines=129-94
extractor3.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
extractor3.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
extractor3.Initialize()
extractor3.ExtractAllValues()

extractor4 = CLASExtractor()
extractor4.SetMeasurement(g1p)
extractor4.SetInputFile("experiments/CLAS/g1_E91023.dat")
extractor4.QsqMin=0.74
extractor4.QsqMax=1.1
extractor4.linestoskip=140
extractor4.NumberOfLines=172-140
extractor4.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
extractor4.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
extractor4.Initialize()
extractor4.ExtractAllValues()

extractor5 = CLASExtractor()
extractor5.SetMeasurement(g1p)
extractor5.SetInputFile("experiments/CLAS/g1_E91023.dat")
extractor5.QsqMin=1.1
extractor5.QsqMax=1.64
extractor5.linestoskip=183
extractor5.NumberOfLines=212-183
extractor5.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
extractor5.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
extractor5.Initialize()
extractor5.ExtractAllValues()

g1p.BuildGraph()


experiment.Print()
manager.SaveExperiment(experiment)



