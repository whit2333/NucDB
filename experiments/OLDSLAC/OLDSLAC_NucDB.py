from ROOT import gROOT,gSystem
from NucDBExtractors import NucDBRawDataExtractor
#gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager
from ROOT import NucDBExperiment
from ROOT import NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from ROOT import NucDBBinnedVariable
import os
import ROOT

class OLDSLACExtractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.isysErr=3
        self.istatErr=4

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=0
        iQsq=1
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.01)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.1)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr]))
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

if __name__ == "__main__":

    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("OLDSLAC")
    if not experiment :
        experiment = NucDBExperiment("OLDSLAC","OLDSLAC")
    
    F2p = experiment.GetMeasurement("F2p")
    if not F2p :
        F2p = NucDBMeasurement("F2p","F_{2}^{p}")
        experiment.AddMeasurement(F2p)
    F2p.ClearDataPoints()
    F2p.SetColor(4009)
    
    F2pExtractor = OLDSLACExtractor()
    F2pExtractor.SetMeasurement(F2p)
    F2pExtractor.SetInputFile("experiments/OLDSLAC/f2prot.dat")
    #F2pExtractor.linestoskip=0
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    
    #F2pExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Xbjorken)
    #F2pExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Qsq)
    F2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    F2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    F2pExtractor.Initialize()
    F2pExtractor.ExtractAllValues()
    F2p.BuildGraph()
    print "testing 1,2,3"
    
    F2d = experiment.GetMeasurement("F2d")
    if not F2d :
        F2d = NucDBMeasurement("F2d","F_{2}^{d}")
        experiment.AddMeasurement(F2d)
    F2d.ClearDataPoints()
    F2d.SetColor(4009)
    
    F2dExtractor = OLDSLACExtractor()
    F2dExtractor.SetMeasurement(F2d)
    F2dExtractor.SetInputFile("experiments/OLDSLAC/f2nucl.dat")
    #F2dExtractor.linestoskip=0
    #Xbjorken = NucDBBinnedVariable("x","x")
    #Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    #W   = NucDBInvariantMassDV()
    #W.SetVariable(0,Xbjorken)
    #W.SetVariable(1,Qsq)
    #F2dExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Xbjorken)
    #F2dExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Qsq)
    F2dExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    F2dExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    
    F2dExtractor.Initialize()
    F2dExtractor.ExtractAllValues()
    F2d.BuildGraph()

    R = experiment.GetMeasurement("R")
    if not R :
        R = NucDBMeasurement("R","R^{p}")
        experiment.AddMeasurement(R)
    R.ClearDataPoints()
    R.SetColor(1)
    RExtractor = OLDSLACExtractor()
    RExtractor.SetMeasurement(R)
    RExtractor.SetInputFile("experiments/OLDSLAC/R_prot_deut.dat")
    #RExtractor.linestoskip=0
    #RExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Xbjorken)
    #RExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Qsq)
    RExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    RExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    RExtractor.Initialize()
    RExtractor.ExtractAllValues()
    R.BuildGraph()
    
    
    experiment.Print()
    manager.SaveExperiment(experiment)
    
    
    
