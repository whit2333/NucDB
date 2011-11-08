from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class SMCExtractor(NucDBRawDataExtractor) :
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
        if values[self.iValueRow]=="-" :
            self.rowcut.currentValue=int(1) # data will not be used
            return
        else: 
            self.rowcut.currentValue=int(0) # data will be used
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

class SMCExtractorg2p(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=5
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
        #if values[self.iValueRow]=="-" :
            #self.rowcut.currentValue=int(1) # data will not be used
            #return
        #else: 
        self.rowcut.currentValue=int(0) # data will be used
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.1)
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.fSystematicError.SetError(float(0))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()

class SMCExtractorA2p(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.istatErr=5

class SMCExtractorAXN(SMCExtractor) :
    def __init__(self):
        SMCExtractor.__init__(self)
        self.iValueRow=4
        self.isysErr=5
        self.istatErr=6

class SMCExtractorgXN(SMCExtractor) :
    def __init__(self):
        SMCExtractor.__init__(self)
        self.iValueRow=7
        self.isysErr=8
        self.istatErr=9


manager = NucDBManager.GetManager()

experiment = manager.GetExperiment("SMC")
if not experiment :
    experiment = NucDBExperiment("SMC","SMC")

# A1p
A1p = experiment.GetMeasurement("A1p")
if not A1p :
    A1p = NucDBMeasurement("A1p","A_{1}^{p}")
    experiment.AddMeasurement(A1p)
A1p.ClearDataPoints()
A1p.fColor=8

A1pExtractor = SMCExtractorAXN()
A1pExtractor.SetMeasurement(A1p)
A1pExtractor.SetInputFile("experiments/SMC/a1pg1p.dat")
A1pExtractor.linestoskip=4
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
A1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
A1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
A1pExtractor.Initialize()
A1pExtractor.ExtractAllValues()
A1p.BuildGraph()

# g1p
g1p = experiment.GetMeasurement("g1p")
if not g1p :
    g1p = NucDBMeasurement("g1p","g_{1}^{p}")
    experiment.AddMeasurement(g1p)
g1p.ClearDataPoints()
g1p.fColor=8

g1pExtractor = SMCExtractorgXN()
g1pExtractor.SetMeasurement(g1p)
g1pExtractor.SetInputFile("experiments/SMC/a1pg1p.dat")
g1pExtractor.linestoskip=4
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g1pExtractor.Initialize()
g1pExtractor.ExtractAllValues()
g1p.BuildGraph()

# A2p
A2p = experiment.GetMeasurement("A2p")
if not A2p :
    A2p = NucDBMeasurement("A2p","A_{2}^{p}")
    experiment.AddMeasurement(A2p)
A2p.ClearDataPoints()
A2p.fColor=8
A2pExtractor = SMCExtractorA2p()
A2pExtractor.SetMeasurement(A2p)
A2pExtractor.SetInputFile("experiments/SMC/a2p.dat")
A2pExtractor.linestoskip=4
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
A2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
A2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
A2pExtractor.Initialize()
A2pExtractor.ExtractAllValues()
A2p.BuildGraph()

# g2p
g2p = experiment.GetMeasurement("g2p")
if not g2p :
    g2p = NucDBMeasurement("g2p","g_{2}^{p}")
    experiment.AddMeasurement(g2p)
g2p.ClearDataPoints()
g2p.fColor=8

g2pExtractor = SMCExtractorg2p()
g2pExtractor.SetMeasurement(g2p)
g2pExtractor.SetInputFile("experiments/SMC/g2p.dat")
g2pExtractor.linestoskip=4
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g2pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g2pExtractor.Initialize()
g2pExtractor.ExtractAllValues()
g2p.BuildGraph()

experiment.Print()

manager.SaveExperiment(experiment)



