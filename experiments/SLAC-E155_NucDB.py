from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
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
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iValueRowErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        #
        W = self.fCurrentDataPoint.GetDependentVariable("W")
        if not W :
            W   = NucDBInvariantMassDV()
            self.fCurrentDataPoint.AddDependentVariable(W)
        if W :
            W.SetVariable(0,x)
            W.SetVariable(1,Qsq)
        nu = self.fCurrentDataPoint.GetDependentVariable("nu")
        if not nu :
            nu   = NucDBPhotonEnergyDV()
            self.fCurrentDataPoint.AddDependentVariable(nu)
        if nu :
            nu.SetVariable(0,x)
            nu.SetVariable(1,Qsq)
        self.fCurrentDataPoint.CalculateDependentVariables()
        #
        #self.fCurrentDataPoint.Print()

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
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(5.0,0.5)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iValueRowErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        #self.fCurrentDataPoint.Print()
        #
        W = self.fCurrentDataPoint.GetDependentVariable("W")
        if not W :
            W   = NucDBInvariantMassDV()
            self.fCurrentDataPoint.AddDependentVariable(W)
        if W :
            W.SetVariable(0,x)
            W.SetVariable(1,Qsq)
        nu = self.fCurrentDataPoint.GetDependentVariable("nu")
        if not nu :
            nu   = NucDBPhotonEnergyDV()
            self.fCurrentDataPoint.AddDependentVariable(nu)
        if nu :
            nu.SetVariable(0,x)
            nu.SetVariable(1,Qsq)
        self.fCurrentDataPoint.CalculateDependentVariables()
        #

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

manager = NucDBManager.GetManager(1)

experiment = manager.GetExperiment("SLAC_E155")
if not experiment :
    experiment = NucDBExperiment("SLAC_E155","SLAC_E155")

#g1pOverF1p = experiment.GetMeasurement("g1pOverF1p")
#if not g1pOverF1p :
    #g1pOverF1p = NucDBMeasurement("g1pOverF1p","g_{1}^{p}/F_{1}^{p}")
#experiment.AddMeasurement(g1pOverF1p)
#g1pOverF1p.fColor=2

g1p = experiment.GetMeasurement("g1p")
if not g1p :
    g1p = NucDBMeasurement("g1p","g_{1}^{p}")
    experiment.AddMeasurement(g1p)
g1p.ClearDataPoints()
g1p.SetColor(1)

g1d = experiment.GetMeasurement("g1d")
if not g1d :
    g1d = NucDBMeasurement("g1d","g_{1}^{d}")
    experiment.AddMeasurement(g1d)
g1d.ClearDataPoints()
g1d.SetColor(1)

g1n = experiment.GetMeasurement("g1n")
if not g1n :
    g1n = NucDBMeasurement("g1n","g_{1}^{n}")
    experiment.AddMeasurement(g1n)
g1n.ClearDataPoints()
g1n.SetColor(1)

g1pExtractor = SLACE155Extractorxg1p()
g1pExtractor.SetMeasurement(g1p)
g1pExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt")
g1pExtractor.linestoskip=20
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
g1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
g1pExtractor.Initialize()
g1pExtractor.ExtractAllValues()
g1p.BuildGraph()

g1nExtractor = SLACE155Extractorxg1n()
g1nExtractor.SetMeasurement(g1n)
g1nExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt")
g1nExtractor.linestoskip=20
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1nExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
g1nExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
g1nExtractor.Initialize()
g1nExtractor.ExtractAllValues()
g1n.BuildGraph()


g1dExtractor = SLACE155Extractorxg1d()
g1dExtractor.SetMeasurement(g1d)
g1dExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt")
g1dExtractor.linestoskip=20
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1dExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
g1dExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
g1dExtractor.Initialize()
g1dExtractor.ExtractAllValues()
g1d.BuildGraph()




g2p = experiment.GetMeasurement("g2p")
if not g2p :
    g2p = NucDBMeasurement("g2p","g_{2}^{p}")
    experiment.AddMeasurement(g2p)
g2p.ClearDataPoints()
g2p.SetColor(1)

g2d = experiment.GetMeasurement("g2d")
if not g2d :
    g2d = NucDBMeasurement("g2d","g_{2}^{d}")
    experiment.AddMeasurement(g2d)
g2d.ClearDataPoints()
g2d.SetColor(1)

g2pExtractor = SLACE155Extractorxg2p()
g2pExtractor.SetMeasurement(g2p)
g2pExtractor.SetInputFile("experiments/SLAC-E155/xg2_pd_comb.txt")
g2pExtractor.linestoskip=18
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
g2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
g2pExtractor.Initialize()
g2pExtractor.ExtractAllValues()
g2p.BuildGraph()

g2dExtractor = SLACE155Extractorxg2d()
g2dExtractor.SetMeasurement(g2d)
g2dExtractor.SetInputFile("experiments/SLAC-E155/xg2_pd_comb.txt")
g2dExtractor.linestoskip=18
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2dExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
g2dExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
g2dExtractor.Initialize()
g2dExtractor.ExtractAllValues()
g2d.BuildGraph()



experiment.Print()
manager.SaveExperiment(experiment)



