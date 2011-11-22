from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class Pion2008Extractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iz=0
        self.iValueRow=1
        self.iStatErr=2
        self.xValue = 0.32
        self.QsquaredValue = 2.30;

    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        deltax=0.01
        if values[self.iValueRow]=="-" :
            self.rowcut.currentValue=int(1) # data will not be used
            return
        else: 
            self.rowcut.currentValue=int(0) # data will be used
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x : 
            x.SetBinValueSize(float(self.xValue),deltax)
            x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq : 
            Qsq.SetBinValueSize(float(self.QsquaredValue),0.1)
            Qsq.Print()
        z = self.fCurrentDataPoint.GetBinVariable("z")
        if z : 
            z.SetBinValueSize(float(values[self.iz]),0.01)
            z.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.iStatErr]))
        self.fCurrentDataPoint.fSystematicError.SetError(float(0.0))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()

#class SMCExtractorA2p(Pion2008Extractor) :
    #def __init__(self):
        #NucDBRawDataExtractor.__init__(self)
        #self.iValueRow=4
        #self.istatErr=5

#class SMCExtractorAXN(Pion2008Extractor) :
    #def __init__(self):
        #SMCExtractor.__init__(self)
        #self.iValueRow=4
        #self.isysErr=5
        #self.istatErr=6

#class SMCExtractorgXN(Pion2008Extractor) :
    #def __init__(self):
        #SMCExtractor.__init__(self)
        #self.iValueRow=7
        #self.isysErr=8
        #self.istatErr=9


if __name__ == "__main__":
    manager = NucDBManager.GetManager()

    experiment = manager.GetExperiment("JLAB_E00108")
    if not experiment :
        experiment = NucDBExperiment("JLAB_E00108","JLAB_E00108")

    #proton 
    sigpPlus = experiment.GetMeasurement("sig_p(pi+)")
    if not sigpPlus :
        sigpPlus = NucDBMeasurement("sig_p(pi+)","#frac{d #sigma_{p}}{dE_{e}d#Omega_{e}dzdP_{t}^{2}d#phi} nb/GeV^{3}-sr")
        experiment.AddMeasurement(sigpPlus)
    sigpPlus.ClearDataPoints()
    sigpPlus.fColor=8
    extractor1 = Pion2008Extractor()
    extractor1.SetMeasurement(sigpPlus)
    extractor1.SetInputFile("experiments/JLAB-E00108/inc_elec_production.dat")
    extractor1.linestoskip=16
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    z = NucDBBinnedVariable("z","z")
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(z)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    sigpPlus.BuildGraph()

    sigpMinus = experiment.GetMeasurement("sig_p(pi-)")
    if not sigpMinus :
        sigpMinus = NucDBMeasurement("sig_p(pi-)","#frac{d #sigma_{p}}{dE_{e}d#Omega_{e}dzdP_{t}^{2}d#phi} nb/GeV^{3}-sr")
        experiment.AddMeasurement(sigpMinus)
    sigpMinus.ClearDataPoints()
    sigpMinus.fColor=4
    extractor2 = Pion2008Extractor()
    extractor2.iValueRow=3
    extractor2.iStatErr=4
    extractor2.SetMeasurement(sigpMinus)
    extractor2.SetInputFile("experiments/JLAB-E00108/inc_elec_production.dat")
    extractor2.linestoskip=16
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    z = NucDBBinnedVariable("z","z")
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(z)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    sigpMinus.BuildGraph()

    # deuteron 
    sigdPlus = experiment.GetMeasurement("sig_d(pi+)")
    if not sigdPlus :
        sigdPlus = NucDBMeasurement("sig_d(pi+)","#frac{d #sigma_{d}}{dE_{e}d#Omega_{e}dzdP_{t}^{2}d#phi} nb/GeV^{3}-sr")
        experiment.AddMeasurement(sigdPlus)
    sigdPlus.ClearDataPoints()
    sigdPlus.fColor=2
    extractor3 = Pion2008Extractor()
    extractor3.iValueRow=5
    extractor3.iStatErr=6
    extractor3.SetMeasurement(sigdPlus)
    extractor3.SetInputFile("experiments/JLAB-E00108/inc_elec_production.dat")
    extractor3.linestoskip=16
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    z = NucDBBinnedVariable("z","z")
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(z)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    sigdPlus.BuildGraph()

    sigdMinus = experiment.GetMeasurement("sig_d(pi-)")
    if not sigdMinus :
        sigdMinus = NucDBMeasurement("sig_d(pi-)","#frac{d #sigma_{p}}{dE_{e}d#Omega_{e}dzdP_{t}^{2}d#phi} nb/GeV^{3}-sr")
        experiment.AddMeasurement(sigdMinus)
    sigdMinus.ClearDataPoints()
    sigdMinus.fColor=6
    extractor4 = Pion2008Extractor()
    extractor4.iValueRow=7
    extractor4.iStatErr=8
    extractor4.SetMeasurement(sigdMinus)
    extractor4.SetInputFile("experiments/JLAB-E00108/inc_elec_production.dat")
    extractor4.linestoskip=16
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    z = NucDBBinnedVariable("z","z")
    extractor4.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor4.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor4.fCurrentDataPoint.fBinnedVariables.Add(z)
    extractor4.Initialize()
    extractor4.ExtractAllValues()
    sigdMinus.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)



