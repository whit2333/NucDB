from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class SLACExtractor(NucDBRawDataExtractor) :
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

    ref = NucDBReference("Ref-SLAC-E004","High-Energy Inelastic e p Scattering at 6-Degrees and 10-Degrees")
    ref.SetDescription("")
    ref.SetURL("http://inspirehep.net/record/54874")
    ref.SetDOI("http://dx.doi.org/10.1103/PhysRevLett.23.930")
    #ref.GetPapers().Add(paper)

    
    #------------------------------------------------------------
    # Cross sections

    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    theta    = NucDBBinnedVariable("theta","#theta")

    sig = experiment.GetMeasurement("sigma_p")
    if not sig :
        sig = NucDBMeasurement("sigma_p","#frac{d#sigma^{p}}{dEd#Omega}")
        experiment.AddMeasurement(sig)
    sig.ClearDataPoints()
    sig.SetColor(1)
    
    experiment.Print()
    manager.SaveExperiment(experiment)
    
    
    
