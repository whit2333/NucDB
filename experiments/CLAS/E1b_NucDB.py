from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class CLASExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iQ2=0
        self.iValueRow=1
        self.istatErr=2
        self.isysErr=3
    
    def ParseLine(self):
        """ See input file for column structures
        """
        #iQsq=3
        values = self.currentline.split()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQ2]),0.01)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr].lstrip('+')))
        #self.fCurrentDataPoint.Print()
        self.fCurrentDataPoint.CalculateTotalError()
        #self.linesRead+=1


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("CLAS")
    if not experiment :
        experiment = NucDBExperiment("CLAS","CLAS")
    
    
    # first moment of g1p = Gamma1p
    g1p = experiment.GetMeasurement("Gamma1p")
    if not g1p :
        g1p = NucDBMeasurement("Gamma1p","#Gamma_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4001)
    extractor1 = CLASExtractor()
    extractor1.SetMeasurement(g1p)
    extractor1.SetInputFile("experiments/CLAS/E1b/clasdb_Gamma1_tot.txt",8)
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()

    g1p = experiment.GetMeasurement("Gamma1d")
    if not g1p :
        g1p = NucDBMeasurement("Gamma1d","#Gamma_{1}^{d}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4001)

    extractor1 = CLASExtractor()
    extractor1.SetMeasurement(g1p)
    extractor1.SetInputFile("experiments/CLAS/E1b/clasdb_Gamma1_d_meas.txt",8)
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    
    
    
    experiment.Print()
    manager.SaveExperiment(experiment)
    
    
    
