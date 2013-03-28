from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class JLABExtractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.isysErr=6
        self.istatErr=5

    def ParseLine(self):
        """ See input file for column structures
        """
        iEbeam=0
        inu=1
        values = self.currentline.split()
        #for v in values:
            #print v
        self.rowcut.currentValue=int(0) # does nothign
        nu = self.fCurrentDataPoint.GetBinVariable('nu')
        nu.SetBinValueSize(float(values[inu])/1000.0,0.001)
        #nu.Print()
        theta = self.fCurrentDataPoint.GetBinVariable('theta')
        theta.SetBinValueSize(float(15.5),0.001)
        #theta.Print()
        Ebeam = self.fCurrentDataPoint.GetBinVariable("E")
        Ebeam.SetBinValueSize(float(values[iEbeam])/1000.0,0.001)
        #Ebeam.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        #self.fCurrentDataPoint.Print()

class JLABExtractorXSi(JLABExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.isysErr=6
        self.istatErr=5

class JLABExtractorXSf(JLABExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=3
        self.isysErr=6
        self.istatErr=5

class JLABExtractorXSfc(JLABExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.isysErr=6
        self.istatErr=5

if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("JLAB_E94010")
    if not experiment :
        experiment = NucDBExperiment("JLAB_E94010","JLAB_E94010")
    
    sigi_He3 = experiment.GetMeasurement("sigi_He3")
    if not sigi_He3 :
        sigi_He3 = NucDBMeasurement("sigi_He3","#frac{d #sigma_{i}}{dE^{'}d#Omega} pb/MeV-sr")
        experiment.AddMeasurement(sigi_He3)
    sigi_He3.ClearDataPoints()
    sigi_He3.SetColor(4006)
    
    sigi_He3Extractor = JLABExtractorXSi()
    sigi_He3Extractor.SetMeasurement(sigi_He3)
    sigi_He3Extractor.SetInputFile("experiments/JLAB-E94010/E94010_xs_unfolded.dat_official",9)
    #sigi_He3Extractor.linestoskip=10
    beamE = NucDBBinnedVariable("E","E_{0}")
    theta = NucDBBinnedVariable("theta","#theta")
    nu = NucDBBinnedVariable("nu","#nu")
    sigi_He3Extractor.fCurrentDataPoint.AddBinVariable(beamE)
    sigi_He3Extractor.fCurrentDataPoint.AddBinVariable(theta)
    sigi_He3Extractor.fCurrentDataPoint.AddBinVariable(nu)
    sigi_He3Extractor.Initialize()
    sigi_He3Extractor.ExtractAllValues()
    sigi_He3.BuildGraph("nu")
    
    
    sigf_He3 = experiment.GetMeasurement("sigf_He3")
    if not sigf_He3 :
        sigf_He3 = NucDBMeasurement("sigf_He3","#frac{d #sigma_{f}}{dE^{'}d#Omega} pb/MeV-sr")
        experiment.AddMeasurement(sigf_He3)
    sigf_He3.ClearDataPoints()
    sigf_He3.SetColor(4006)
    
    sigf_He3Extractor = JLABExtractorXSf()
    sigf_He3Extractor.SetMeasurement(sigf_He3)
    sigf_He3Extractor.SetInputFile("experiments/JLAB-E94010/E94010_xs_unfolded.dat_official",9)
    #sigf_He3Extractor.linestoskip=10
    beamE = NucDBBinnedVariable("E","E_{0}")
    theta = NucDBBinnedVariable("theta","#theta")
    nu = NucDBBinnedVariable("nu","#nu")
    sigf_He3Extractor.fCurrentDataPoint.AddBinVariable(beamE)
    sigf_He3Extractor.fCurrentDataPoint.AddBinVariable(theta)
    sigf_He3Extractor.fCurrentDataPoint.AddBinVariable(nu)
    sigf_He3Extractor.Initialize()
    sigf_He3Extractor.ExtractAllValues()
    sigf_He3.BuildGraph("nu")
    
    sigfc_He3 = experiment.GetMeasurement("sigfc_He3")
    if not sigfc_He3 :
        sigfc_He3 = NucDBMeasurement("sigfc_He3","#frac{d #sigma_{fc}}{dE^{'}d#Omega} pb/MeV-sr")
        experiment.AddMeasurement(sigfc_He3)
    sigfc_He3.ClearDataPoints()
    sigfc_He3.SetColor(4006)
    
    sigfc_He3Extractor = JLABExtractorXSfc()
    sigfc_He3Extractor.SetMeasurement(sigfc_He3)
    sigfc_He3Extractor.SetInputFile("experiments/JLAB-E94010/E94010_xs_unfolded.dat_official",9)
    #sigfc_He3Extractor.linestoskip=10
    beamE = NucDBBinnedVariable("E","E_{0}")
    theta = NucDBBinnedVariable("theta","#theta")
    nu = NucDBBinnedVariable("nu","#nu")
    sigfc_He3Extractor.fCurrentDataPoint.AddBinVariable(beamE)
    sigfc_He3Extractor.fCurrentDataPoint.AddBinVariable(theta)
    sigfc_He3Extractor.fCurrentDataPoint.AddBinVariable(nu)
    sigfc_He3Extractor.Initialize()
    sigfc_He3Extractor.ExtractAllValues()
    sigfc_He3.BuildGraph("nu")
    
    
    #datapoint = NucDBDataPoint(1.0,0.2)
    #Aperp.AddDataPoint(datapoint)
    experiment.Print()
    
    manager.SaveExperiment(experiment)
    
    
    
