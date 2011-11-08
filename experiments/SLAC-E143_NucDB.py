from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class SLACE143Extractor(NucDBRawDataExtractor):
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
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.1)
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.fSystematicError.SetError(float(values[self.isysErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()

manager = NucDBManager.GetManager()

experiment = manager.GetExperiment("SLAC_E143")
if not experiment :
    experiment = NucDBExperiment("SLAC_E143","SLAC_E143")

g1p = experiment.GetMeasurement("g1p")
if not g1p :
    g1p = NucDBMeasurement("g1p","g_{1}^{p}")
    experiment.AddMeasurement(g1p)
g1p.ClearDataPoints()
g1p.fColor=2


g1pExtractor = SLACE143Extractor()
g1pExtractor.SetMeasurement(g1p)
g1pExtractor.fCurrentDataPoint.fName = "g1p"
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g1pExtractor.SetInputFile("experiments/SLAC-E143/g1p.dat")
g1pExtractor.linestoskip=11
g1pExtractor.Initialize()
g1pExtractor.ExtractAllValues()
g1p.BuildGraph()


g2p = experiment.GetMeasurement("g2p")
if not g2p :
    g2p = NucDBMeasurement("g2p","g_{2}^{p}")
    experiment.AddMeasurement(g2p)
g2p.ClearDataPoints()
g2p.fColor=2

g2pExtractor = SLACE143Extractor()
g2pExtractor.SetMeasurement(g2p)
g2pExtractor.fCurrentDataPoint.fName = "g2p"
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g2pExtractor.SetInputFile("experiments/SLAC-E143/g2p.dat")
g2pExtractor.linestoskip=11
g2pExtractor.Initialize()
g2pExtractor.ExtractAllValues()
g2p.BuildGraph()

experiment.Print()

manager.SaveExperiment(experiment)



