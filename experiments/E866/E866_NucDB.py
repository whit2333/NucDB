from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from ROOT import NucDBPaper, NucDBReference, NucDBErrorBar
from NucDBExtractors import *
import os

class E866Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.ixbjorken     = 0
        self.iQsq          = 9
        self.iValueRow     = 20
        self.istatErrPlus      = 21 
        self.istatErrMinus      = 22 
        self.isysErrPlus       = 23
        self.isysErrMinus       = 24
    
    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        deltax = 0.01
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetMeanLimits(float(values[self.ixbjorken]),float(values[self.ixbjorken+1]),float(values[self.ixbjorken+2]))
            #x.SetBinValueSize(float(values[self.ixbjorken]),deltax)
            #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq :
            Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
            #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        #statErr = NucDBErrorBar(float(values[self.istatErrPlus].lstrip('+')),float(values[self.istatErrMinus].lstrip('+')))
        #systErr = NucDBErrorBar(float(values[self.isysErrPlus].lstrip('+')),float(values[self.isysErrMinus].lstrip('+')))
        #statErr.Print()
        #systErr.Print()
        #self.fCurrentDataPoint.SetStatError(statErr);
        #self.fCurrentDataPoint.SetSystError(systErr);
        self.fCurrentDataPoint.GetStatError().SetErrorSize(float(values[self.istatErrPlus].lstrip('+')),float(values[self.istatErrMinus].lstrip('+')))
        self.fCurrentDataPoint.GetSystError().SetErrorSize(float(values[self.isysErrPlus].lstrip('+')),float(values[self.isysErrMinus].lstrip('+')))
        self.fCurrentDataPoint.CalculateTotalError()
        #self.fCurrentDataPoint.Dump()
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
        #self.fCurrentDataPoint.Print()

if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("E866")
    if not experiment :
        experiment = NucDBExperiment("E866","E866")

    # note that the dilepton mass is equal to Q2
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")

    dbarMinusubar = experiment.GetMeasurement("dbarMinusubar")
    if not dbarMinusubar :
        dbarMinusubar = NucDBMeasurement("dbarMinusubar","#bar{d}-#bar{u}")
        experiment.AddMeasurement(dbarMinusubar)

    paper = NucDBPaper("paper-E866/NuSea","Improved measurement of the anti-d / anti-u asymmetry in the nucleon sea ")
    paper.SetShortRef("Phys.Rev. D64 (2001) 052002")
    paper.SetJournal("Phys.Rev.D")
    paper.SetInSpire("http://inspirehep.net/record/554316")
    paper.SetDOI("http://dx.doi.org/10.1103/PhysRevD.64.052002")

    ref = NucDBReference("Ref-E866/NuSea","E866 nucleon sea asymmetry results")
    ref.SetDescription("NuSea collaboration results on dbar-ubar and dbar/ubar")
    ref.SetURL("")
    #ref.AddComment("All systematic uncertainties are combined in the reported systematic errors. ")
    ref.GetPapers().Add(paper)

    dbarMinusubar.ClearDataPoints()
    dbarMinusubar.AddRef(ref)

    extractor = E866Extractor()
    extractor.SetMeasurement(dbarMinusubar)
    extractor.SetInputFile("experiments/E866/dbar-ubar.txt",13)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.Initialize()
    extractor.ExtractAllValues()
    dbarMinusubar.BuildGraph()

    dbarMinusubar.Print("data")

    experiment.Print()
    manager.SaveExperiment(experiment)



