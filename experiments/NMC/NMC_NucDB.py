from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class NMCExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=3
        self.istatErr=4
        self.isysErr=5
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
        deltax=0.001
        x = self.fCurrentDataPoint.GetBinVariable('x')
        #print values[ixbjorken]
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

    experiment = manager.GetExperiment("NMC")
    if not experiment :
        experiment = NucDBExperiment("NMC","NMC")

    F2p = experiment.GetMeasurement("F2p")
    if not F2p :
        F2p = NucDBMeasurement("F2p","F_{2}^{p}")
        experiment.AddMeasurement(F2p)
    F2p.ClearDataPoints()
    F2p.SetColor(4008)
    extractor1 = NMCExtractor()
    extractor1.SetMeasurement(F2p)
    extractor1.SetInputFile("experiments/NMC/f2pmerge.dat",0,158)
    #extractor1.linestoskip=0
    #extractor1.NumberOfLines=158
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()

    # F2C/F2D
    F2COverF2D = experiment.GetMeasurement("F2C/F2D")
    if not F2COverF2D :
        F2COverF2D = NucDBMeasurement("F2C/F2D","F_{2}^{C}/F_{2}^{D}")
        experiment.AddMeasurement(F2COverF2D)
    F2COverF2D.ClearDataPoints()
    extractor1 = NMCExtractor()
    extractor1.SetMeasurement(F2COverF2D)
    extractor1.SetInputFile("experiments/NMC/f2cf2deut200.txt",3)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    
    extractor1 = NMCExtractor()
    extractor1.SetMeasurement(F2COverF2D)
    extractor1.iValueRow=2
    extractor1.istatErr=3
    extractor1.isysErr=4
    extractor1.SetInputFile("experiments/NMC/f2cf2deut.txt",3)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    F2COverF2D.BuildGraph()

    F2nOverF2p = experiment.GetMeasurement("F2n/F2p")
    if not F2nOverF2p :
        F2nOverF2p = NucDBMeasurement("F2n/F2p","F_{2}^{n}/F_{2}^{p}")
        experiment.AddMeasurement(F2nOverF2p)
    F2nOverF2p.ClearDataPoints()
    extractor1 = NMCExtractor()
    extractor1.iValueRow=2
    extractor1.istatErr=3
    extractor1.isysErr=4
    extractor1.SetMeasurement(F2nOverF2p)
    extractor1.SetInputFile("experiments/NMC/f2nOverf2pdeut_final.txt",28)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()

    experiment.Print()
    manager.SaveExperiment(experiment)



