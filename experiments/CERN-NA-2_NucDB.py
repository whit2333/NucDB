from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class CERNNA2Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.istatErr=3
        self.isysErr=4
        self.NumberOfLines=0
        self.linesRead=0
        self.iQsq=1
    
    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=0
        #print self.currentline
        values = self.currentline.split()
        if self.linesRead > self.NumberOfLines :
             self.rowcut.currentValue=1
             return
        deltax=0.01
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr].lstrip('+')))
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
        self.linesRead+=1


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("CERN-NA-2")
    if not experiment :
        experiment = NucDBExperiment("CERN-NA-2","CERN-NA-2")

    F2n = experiment.GetMeasurement("F2n")
    if not F2n :
        F2n = NucDBMeasurement("F2n","F_{2}^{n}")
        experiment.AddMeasurement(F2n)
    F2n.ClearDataPoints()
    F2n.SetColor(4017)
    extractor1 = CERNNA2Extractor()
    extractor1.SetMeasurement(F2n)
    extractor1.SetInputFile("experiments/CERN-NA-2/f2neutron.dat")
    extractor1.linestoskip=0
    extractor1.NumberOfLines=65
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    F2n.BuildGraph()
    

    experiment.Print()
    manager.SaveExperiment(experiment)



