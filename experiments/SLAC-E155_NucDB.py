from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class SLACE155Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.iValueRowErr=5

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
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.iValueRowErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()

class SLACE155Extractorxg2p(SLACE155Extractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.iValueRowErr=5

class SLACE155Extractorxg2d(SLACE155Extractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=6
        self.iValueRowErr=7

class SLACE155Extractor2(SLACE155Extractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=6
        self.iValueRowErr=7
        
    def ParseLine(self):
        """ See g1_pdn_Q2_5_comb.txt
        """
        ixMin=0
        ixMax=1
        ixbjorken=2
        values = self.currentline.split()
        deltax=float(values[ixMax])-float(values[ixMin])
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(5.0,0.5)
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.iValueRowErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()

class SLACE155Extractorxg1p(SLACE155Extractor2):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=3
        self.iValueRowErr=4

class SLACE155Extractorxg1d(SLACE155Extractor2):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=5
        self.iValueRowErr=6
        
class SLACE155Extractorxg1n(SLACE155Extractor2):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=7
        self.iValueRowErr=8

manager = NucDBManager.GetManager()

experiment = manager.GetExperiment("SLAC_E155")
if not experiment :
    experiment = NucDBExperiment("SLAC_E155","SLAC_E155")

#g1pOverF1p = experiment.GetMeasurement("g1pOverF1p")
#if not g1pOverF1p :
    #g1pOverF1p = NucDBMeasurement("g1pOverF1p","g_{1}^{p}/F_{1}^{p}")
#experiment.fMeasurements.Add(g1pOverF1p)
#g1pOverF1p.fColor=2

g1p = experiment.GetMeasurement("g1p")
if not g1p :
    g1p = NucDBMeasurement("g1p","g_{1}^{p}")
experiment.fMeasurements.Add(g1p)
g1p.fColor=2

g1d = experiment.GetMeasurement("g1d")
if not g1d :
    g1d = NucDBMeasurement("g1d","g_{1}^{d}")
experiment.fMeasurements.Add(g1d)
g1d.fColor=2

g1n = experiment.GetMeasurement("g1n")
if not g1n :
    g1n = NucDBMeasurement("g1n","g_{1}^{n}")
experiment.fMeasurements.Add(g1n)
g1n.fColor=2

g1pExtractor = SLACE155Extractorxg1p()
g1pExtractor.SetMeasurement(g1p)
g1pExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt")
g1pExtractor.linestoskip=20
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g1pExtractor.Initialize()
g1pExtractor.ExtractAllValues()
g1p.BuildGraph()

g1nExtractor = SLACE155Extractorxg1n()
g1nExtractor.SetMeasurement(g1n)
g1nExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt")
g1nExtractor.linestoskip=20
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1nExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g1nExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g1nExtractor.Initialize()
g1nExtractor.ExtractAllValues()
g1n.BuildGraph()


g1dExtractor = SLACE155Extractorxg1d()
g1dExtractor.SetMeasurement(g1d)
g1dExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt")
g1dExtractor.linestoskip=20
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1dExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g1dExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g1dExtractor.Initialize()
g1dExtractor.ExtractAllValues()
g1d.BuildGraph()




g2p = experiment.GetMeasurement("g2p")
if not g2p :
    g2p = NucDBMeasurement("g2p","g_{2}^{p}")
experiment.fMeasurements.Add(g2p)
g2p.fColor=2

g2d = experiment.GetMeasurement("g2d")
if not g2d :
    g2d = NucDBMeasurement("g2d","g_{2}^{d}")
experiment.fMeasurements.Add(g2d)
g2d.fColor=2

g2pExtractor = SLACE155Extractorxg2p()
g2pExtractor.SetMeasurement(g2p)
g2pExtractor.SetInputFile("experiments/SLAC-E155/xg2_pd_comb.txt")
g2pExtractor.linestoskip=18
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g2pExtractor.Initialize()
g2pExtractor.ExtractAllValues()
g2p.BuildGraph()

g2dExtractor = SLACE155Extractorxg2d()
g2dExtractor.SetMeasurement(g2d)
g2dExtractor.SetInputFile("experiments/SLAC-E155/xg2_pd_comb.txt")
g2dExtractor.linestoskip=18
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2dExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g2dExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g2dExtractor.Initialize()
g2dExtractor.ExtractAllValues()
g2d.BuildGraph()


#g1pOverF1pExtractor = SLACE143Extractor_g1pOverF1p()
#g1pOverF1pExtractor.SetMeasurement(g1pOverF1p)
#g1pOverF1pExtractor.fCurrentDataPoint.fName = "g1p/F1p"
## create x variable
#Xbjorken = NucDBBinnedVariable("x","x")
#g1pOverF1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
## create x variable
#Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#g1pOverF1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
#g1pOverF1pExtractor.SetInputFile("experiments/SLAC-E143/G1F1_AV.txt")
#g1pOverF1pExtractor.linestoskip=0
#g1pOverF1pExtractor.Initialize()
#g1pOverF1pExtractor.ExtractAllValues()
#g1pOverF1p.BuildGraph()

#datapoint = NucDBDataPoint(1.0,0.2)
#Aperp.AddDataPoint(datapoint)
experiment.Print()

manager.SaveExperiment(experiment)



