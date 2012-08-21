from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class SLACE154Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.isysErr=5
        self.istatErr=6

    def ParseLine(self):
        """ See input file for column structures
        """
        ixMin=0
        ixMax=1
        ixbjorken=2
        iQsq=3
        values = self.currentline.split()
        deltax=float(values[ixMax])-float(values[ixMin])
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.1)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        #self.fCurrentDataPoint.Print()

class SLACE155ExtractorAXn(SLACE154Extractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.isysErr=5
        self.istatErr=6

class SLACE155ExtractorgXn(SLACE154Extractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=7
        self.isysErr=8
        self.istatErr=9


manager = NucDBManager.GetManager(1)

experiment = manager.GetExperiment("SLAC_E154")
if not experiment :
    experiment = NucDBExperiment("SLAC_E154","SLAC_E154")

g1n = experiment.GetMeasurement("g1n")
if not g1n :
    g1n = NucDBMeasurement("g1n","g_{1}^{n}")
    experiment.AddMeasurement(g1n)
g1n.ClearDataPoints()
g1n.fColor=6

A1n = experiment.GetMeasurement("A1n")
if not A1n :
    A1n = NucDBMeasurement("A1n","A_{1}^{n}")
    experiment.AddMeasurement(A1n)
A1n.ClearDataPoints()
A1n.fColor=6

g2n = experiment.GetMeasurement("g2n")
if not g2n :
    g2n = NucDBMeasurement("g2n","g_{2}^{n}")
    experiment.AddMeasurement(g2n)
g2n.ClearDataPoints()
g2n.fColor=6

A2n = experiment.GetMeasurement("A2n")
if not A2n :
    A2n = NucDBMeasurement("A2n","A_{2}^{n}")
    experiment.AddMeasurement(A2n)
A2n.ClearDataPoints()
A2n.fColor=6

g1nExtractor = SLACE155ExtractorgXn()
g1nExtractor.SetMeasurement(g1n)
g1nExtractor.SetInputFile("experiments/SLAC-E154/a1ng1n.dat")
g1nExtractor.linestoskip=0
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1nExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
g1nExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
g1nExtractor.Initialize()
g1nExtractor.ExtractAllValues()
g1n.BuildGraph()

A1nExtractor = SLACE155ExtractorAXn()
A1nExtractor.SetMeasurement(A1n)
A1nExtractor.SetInputFile("experiments/SLAC-E154/a1ng1n.dat")
A1nExtractor.linestoskip=0
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
A1nExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
A1nExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
A1nExtractor.Initialize()
A1nExtractor.ExtractAllValues()
A1n.BuildGraph()

g2nExtractor = SLACE155ExtractorgXn()
g2nExtractor.SetMeasurement(g2n)
g2nExtractor.SetInputFile("experiments/SLAC-E154/a2ng2n.dat")
g2nExtractor.linestoskip=0
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2nExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
g2nExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
g2nExtractor.Initialize()
g2nExtractor.ExtractAllValues()
g2n.BuildGraph()

A2nExtractor = SLACE155ExtractorAXn()
A2nExtractor.SetMeasurement(A2n)
A2nExtractor.SetInputFile("experiments/SLAC-E154/a2ng2n.dat")
A2nExtractor.linestoskip=0
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
A2nExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
A2nExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
A2nExtractor.Initialize()
A2nExtractor.ExtractAllValues()
A2n.BuildGraph()


experiment.Print()
manager.SaveExperiment(experiment)



