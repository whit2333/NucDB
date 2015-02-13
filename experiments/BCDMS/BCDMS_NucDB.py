from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class BCDMSExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.isysErr=3
        self.istatErr=4
        self.NumberOfLines=0
        self.linesRead=0
        self.iQsq=1
    
    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=0
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

    experiment = manager.GetExperiment("BCDMS")
    if not experiment :
        experiment = NucDBExperiment("BCDMS","BCDMS")

    # F2p
    F2p = experiment.GetMeasurement("F2p")
    if not F2p :
        F2p = NucDBMeasurement("F2p","F_{2}^{p}")
        experiment.AddMeasurement(F2p)
    F2p.ClearDataPoints()
    F2p.SetColor(4000)
    extractor1 = BCDMSExtractor()
    extractor1.SetMeasurement(F2p)
    extractor1.SetInputFile("experiments/BCDMS/f2protcomb.dat",0,178)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    F2p.BuildGraph()
    
    # F2n/F2p
    F2nOverF2p = experiment.GetMeasurement("F2n/F2p")
    if not F2nOverF2p :
        F2nOverF2p = NucDBMeasurement("F2n/F2p","F_{2}^{n}/F_{2}^{p}")
        experiment.AddMeasurement(F2nOverF2p)
    F2nOverF2p.ClearDataPoints()
    #F2nOverF2p.SetColor(4000)
    extractor1 = BCDMSExtractor()
    extractor1.SetMeasurement(F2nOverF2p)
    extractor1.SetInputFile("experiments/BCDMS/f2nf2pdeut.txt",3)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    F2nOverF2p.BuildGraph()

    # F2p-F2n
    F2pMinusF2n = experiment.GetMeasurement("F2p-F2n")
    if not F2pMinusF2n :
        F2pMinusF2n = NucDBMeasurement("F2p-F2n","F_{2}^{p}-F_{2}^{n}")
        experiment.AddMeasurement(F2pMinusF2n)
    F2pMinusF2n.ClearDataPoints()
    #F2pMinusF2n.SetColor(4000)
    extractor1 = BCDMSExtractor()
    extractor1.SetMeasurement(F2pMinusF2n)
    extractor1.SetInputFile("experiments/BCDMS/f2nf2pdeut.txt",3)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    F2pMinusF2n.BuildGraph()

    experiment.Print()
    manager.SaveExperiment(experiment)



