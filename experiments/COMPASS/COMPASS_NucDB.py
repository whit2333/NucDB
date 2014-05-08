from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
from ROOT import NucDBPaper,NucDBReference
import os


class COMPASSExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iEbeam        = 0
        self.ixbj_min      = 1
        self.ixbj_max      = 2
        self.ixbj          = 3
        self.iQsq          = 4
        self.iValueRow     = 5
        self.istatErr      = 6
        self.isysErr       = 7

    def ParseLine(self):
        """ Columns: E,x_min,x_max,<x>,Q2,A1p,A1p(stat),A1p(syst),g1p,g1p(stat),g1p(syst)
        """
        values = self.currentline.split(',')
        deltax=float(0.01)
        Ebeam  = self.fCurrentDataPoint.GetBinVariable('Ebeam')
        if Ebeam :
            Ebeam.SetBinValueSize(float(values[self.iEbeam]),deltax)
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetMeanLimits(float(values[self.ixbj]),float(values[self.ixbj_min]),float(values[self.ixbj_max]))
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq : 
            Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
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
        #self.fCurrentDataPoint.Print()



if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("COMPASS")
    if not experiment :
        experiment = NucDBExperiment("COMPASS","COMPASS")

    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    W        = NucDBBinnedVariable("W","W")
    #theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam    = NucDBBinnedVariable("E","E")

    paper = NucDBPaper("paper-COMPAS-proton","The Spin-dependent Structure Function of the Proton g_1^p and a Test of the Bjorken Sum Rule")
    paper.SetShortRef("M. G. Alekseev et al., Phys. Lett. B 690, 466 (2010)")
    paper.SetJournal("Phys.Lett.B")
    paper.SetInSpire("http://inspirehep.net/record/843494")
    paper.SetDOI("http://dx.doi.org/10.1016/j.physletb.2010.05.069")

    paper2 = NucDBPaper("paper-COMPAS-deuteron","The Deuteron Spin-dependent Structure Function g1(d) and its First Moment")
    paper2.SetShortRef("V. Yu. Alexakhin et al., Phys. Lett. B 647, 8 (2007)")
    paper2.SetJournal("Phys.Lett.B")
    paper2.SetInSpire("http://inspirehep.net/record/726688")
    paper2.SetDOI("http://dx.doi.org/10.1016/j.physletb.2006.12.076")

    ref = NucDBReference("Ref-COMPASS","COMPAS Proton g1 and A1 reference")
    ref.SetDescription("COMPAS A1p and g1p data from JAM website")
    ref.SetURL("https://misportal.jlab.org/jamDb/experiments/10024")
    ref.AddComment("Muon energy between 140 and 180 GeV.")
    ref.AddComment("g1p evaluated from A1p using SMC parametrization of F2p [B. Adeva et al., Phys. Rev. D58, 112001 (1998)], and R1998 value for sigmaL/sigmaT ratio.")
    ref.GetPapers().Add(paper)

    ref2 = NucDBReference("Ref-COMPASS","COMPAS deuteron g1 and A1 reference")
    ref2.SetDescription("COMPAS A1d and g1d data from JAM website")
    ref2.SetURL("https://misportal.jlab.org/jamDb/experiments/10024")
    ref2.AddComment("Muon energy between 140 and 180 GeV.")
    ref2.AddComment("g1d values evaluated from A1d using SMC parametrization of F2d [B. Adeva et al., Phys. Rev. D58, 112001 (1998)], and R1998 value for sigmaL/sigmaT ratio.")
    ref2.GetPapers().Add(paper2)

    # A1p
    A1p = experiment.GetMeasurement("A1p")
    if not A1p :
        A1p = NucDBMeasurement("A1p","A_{1}^{p}")
        experiment.AddMeasurement(A1p)
    A1p.Clear()
    A1p.AddRef(ref)
    A1p.AddComment("Muon energy between 140 and 180 GeV.")

    extractor = COMPASSExtractor()
    extractor.SetMeasurement(A1p)
    extractor.SetInputFile("experiments/COMPASS/jam_dataset_proton.csv",1)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    extractor.Initialize()
    extractor.ExtractAllValues()
    A1p.BuildGraph()
    

    # g1p
    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.Clear()
    g1p.AddRef(ref)
    g1p.AddComment("Muon energy between 140 and 180 GeV.")

    extractor = COMPASSExtractor()
    extractor.SetMeasurement(g1p)
    extractor.iValueRow     = 8
    extractor.istatErr      = 9
    extractor.isysErr       = 10
    extractor.SetInputFile("experiments/COMPASS/jam_dataset_proton.csv",1)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    extractor.Initialize()
    extractor.ExtractAllValues()
    g1p.BuildGraph()

    # A1d
    A1d = experiment.GetMeasurement("A1d")
    if not A1d :
        A1d = NucDBMeasurement("A1d","A_{1}^{d}")
        experiment.AddMeasurement(A1d)
    A1d.Clear()
    A1d.AddRef(ref2)
    A1d.AddComment("Muon energy between 140 and 180 GeV.")

    extractor = COMPASSExtractor()
    extractor.SetMeasurement(A1d)
    extractor.SetInputFile("experiments/COMPASS/jam_dataset_deuteron.csv",1)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    extractor.Initialize()
    extractor.ExtractAllValues()
    A1d.BuildGraph()
    

    # g1d
    g1d = experiment.GetMeasurement("g1d")
    if not g1d :
        g1d = NucDBMeasurement("g1d","g_{1}^{d}")
        experiment.AddMeasurement(g1d)
    g1d.Clear()
    g1d.AddRef(ref2)
    g1d.AddComment("Muon energy between 140 and 180 GeV.")

    extractor = COMPASSExtractor()
    extractor.SetMeasurement(g1d)
    extractor.iValueRow     = 8
    extractor.istatErr      = 9
    extractor.isysErr       = 10
    extractor.SetInputFile("experiments/COMPASS/jam_dataset_deuteron.csv",1)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    extractor.Initialize()
    extractor.ExtractAllValues()
    g1d.BuildGraph()

    experiment.Print("comm")
    if not experiment.GetRefs().GetEntries() : 
        print " Warning: Experiment contains no references!"

    manager.SaveExperiment(experiment)

