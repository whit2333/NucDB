from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class E94110Extractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.istatErr=3

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=1
        iQsq=0
        print self.currentline
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.001)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(0)
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

class E94110Extractor2(E94110Extractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.istatErr=5

class E94110Extractor3(E94110Extractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=6
        self.istatErr=7





if __name__ == "__main__":
    
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("JLAB-E94110")
    if not experiment :
        experiment = NucDBExperiment("JLAB-E94110","JLAB-E94110")
    
    F2p = experiment.GetMeasurement("F2p")
    if not F2p :
        F2p = NucDBMeasurement("F2p","F_{2}^{p}")
        experiment.AddMeasurement(F2p)
    F2p.ClearDataPoints()
    F2p.SetColor(4019)
    F2pExtractor = E94110Extractor()
    F2pExtractor.SetMeasurement(F2p)
    F2pExtractor.SetInputFile("experiments/JLAB-E94110/e94110_sfcalc.dat")
    F2pExtractor.linestoskip=2
    F2pExtractor.NumberOfLines=168-1
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    F2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    F2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    F2pExtractor.Initialize()
    F2pExtractor.ExtractAllValues()
    F2p.BuildGraph()

    
    F1p = experiment.GetMeasurement("F1p")
    if not F1p :
        F1p = NucDBMeasurement("F1p","F_{1}^{p}")
        experiment.AddMeasurement(F1p)
    F1p.ClearDataPoints()
    F1p.SetColor(4019)
    F1pExtractor = E94110Extractor2()
    F1pExtractor.SetMeasurement(F1p)
    F1pExtractor.SetInputFile("experiments/JLAB-E94110/e94110_sfcalc.dat")
    F1pExtractor.linestoskip=2
    F1pExtractor.NumberOfLines=168-1
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    F1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    F1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    F1pExtractor.Initialize()
    F1pExtractor.ExtractAllValues()
    F1p.BuildGraph()

    
    FLp = experiment.GetMeasurement("FLp")
    if not FLp :
        FLp = NucDBMeasurement("FLp","F_{L}^{p}")
        experiment.AddMeasurement(FLp)
    FLp.ClearDataPoints()
    FLp.SetColor(4019)
    FLpExtractor = E94110Extractor3()
    FLpExtractor.SetMeasurement(FLp)
    FLpExtractor.SetInputFile("experiments/JLAB-E94110/e94110_sfcalc.dat")
    FLpExtractor.linestoskip=2
    FLpExtractor.NumberOfLines=168-1
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    FLpExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    FLpExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    FLpExtractor.Initialize()
    FLpExtractor.ExtractAllValues()
    FLp.BuildGraph()

    experiment.Print()
    manager.SaveExperiment(experiment)



