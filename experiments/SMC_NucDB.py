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


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("SMC")
    if not experiment :
        experiment = NucDBExperiment("SMC","SMC")

    #PROTON DATA
    # A1p
    A1p = experiment.GetMeasurement("A1p")
    if not A1p :
        A1p = NucDBMeasurement("A1p","A_{1}^{p}")
        experiment.AddMeasurement(A1p)
    A1p.ClearDataPoints()
    A1p.fColor=8
    extractor1 = SMCExtractorAXN()
    extractor1.SetMeasurement(A1p)
    extractor1.SetInputFile("experiments/SMC/a1pg1p.dat")
    extractor1.linestoskip=4
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    A1p.BuildGraph()
    # g1p
    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.fColor=8
    extractor2 = SMCExtractorgXN()
    extractor2.SetMeasurement(g1p)
    extractor2.SetInputFile("experiments/SMC/a1pg1p.dat")
    extractor2.linestoskip=4
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    g1p.BuildGraph()
    # A2p
    A2p = experiment.GetMeasurement("A2p")
    if not A2p :
        A2p = NucDBMeasurement("A2p","A_{2}^{p}")
        experiment.AddMeasurement(A2p)
    A2p.ClearDataPoints()
    A2p.fColor=8
    extractor3 = SMCExtractorA2p()
    extractor3.SetMeasurement(A2p)
    extractor3.SetInputFile("experiments/SMC/a2p.dat")
    extractor3.linestoskip=4
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    A2p.BuildGraph()
    # g2p
    g2p = experiment.GetMeasurement("g2p")
    if not g2p :
        g2p = NucDBMeasurement("g2p","g_{2}^{p}")
        experiment.AddMeasurement(g2p)
    g2p.ClearDataPoints()
    g2p.fColor=8
    extractor4 = SMCExtractorg2p()
    extractor4.SetMeasurement(g2p)
    extractor4.SetInputFile("experiments/SMC/g2p.dat")
    extractor4.linestoskip=4
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor4.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor4.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor4.Initialize()
    extractor4.ExtractAllValues()
    g2p.BuildGraph()

    #DEUTERON
    # TODO 
   
   
    #NEUTRON
    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.fColor=8
    extractor5 = SMCExtractorAXN()
    extractor5.SetMeasurement(g1n)
    extractor5.SetInputFile("experiments/SMC/g1n.dat")
    extractor5.linestoskip=18
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor5.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor5.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor5.Initialize()
    extractor5.ExtractAllValues()
    g1n.BuildGraph()

    experiment.Print()

    manager.SaveExperiment(experiment)



