from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from ROOT import NucDBPaper, NucDBReference
from NucDBExtractors import *
import os

class HERMESExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.ixbjorken     = 0
        self.iQsq          = 1
        self.iValueRow     = 6
        self.isysErr       = 8
        self.istatErr      = 7
    
    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        deltax = 0.01
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetBinValueSize(float(values[self.ixbjorken]),deltax)
            #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq :
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
        #self.fCurrentDataPoint.Print()

class HERMESExtractor2(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.ixbjorken     = 0
        self.iQsq          = 1
        self.iValueRow     = 6
        self.isysErr       = 8
        self.istatErr      = 7
    
    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split(',')
        deltax=0.01
        xTemp = float(values[self.ixbjorken])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetBinValueSize(xTemp,deltax)
            #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq :
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
        #self.fCurrentDataPoint.Print()

class HERMESExtractor3(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.ixbjorken     = 0
        self.iQsq          = 1
        self.iValueRow     = 6
        self.isysErr       = 8
        self.istatErr      = 7
    
    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split(',')
        deltax=0.01
        xTemp = float(values[self.ixbjorken])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetBinValueSize(xTemp,deltax)
            #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq :
            Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
            #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow])/xTemp)
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+'))/xTemp)
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr].lstrip('+'))/xTemp)
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

    experiment = manager.GetExperiment("HERMES")
    if not experiment :
        experiment = NucDBExperiment("HERMES","HERMES")

    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")

    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4003)
    extractor1 = HERMESExtractor()
    extractor1.SetMeasurement(g1p)
    extractor1.SetInputFile("experiments/HERMES/g1p",24,20)
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    g1p.BuildGraph()
    
    g1pOverF1p = experiment.GetMeasurement("g1p/F1p")
    if not g1pOverF1p :
        g1pOverF1p = NucDBMeasurement("g1p/F1p","g_{1}^{p}/F_{1}^{p}")
        experiment.AddMeasurement(g1pOverF1p)
    g1pOverF1p.ClearDataPoints()
    g1pOverF1p.SetColor(4003)
    extractor2 = HERMESExtractor()
    extractor2.iValueRow = 2
    extractor2.istatErr  = 3
    extractor2.isysErr   = 4
    extractor2.SetMeasurement(g1pOverF1p)
    extractor2.SetInputFile("experiments/HERMES/g1p",24,20)
    #extractor2.linestoskip=25
    #extractor2.NumberOfLines=44-25
    extractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor2.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    g1pOverF1p.BuildGraph()
    
    #Using data set with most bins .... @todo add all binning schemes?
    A1p = experiment.GetMeasurement("A1p")
    if not A1p :
        A1p = NucDBMeasurement("A1p","A_{1}^{p}")
        experiment.AddMeasurement(A1p)
    A1p.ClearDataPoints()
    A1p.SetColor(4004)
    extractor3 = HERMESExtractor()
    extractor3.iValueRow=2
    extractor3.istatErr=3
    extractor3.isysErr=4
    extractor3.SetMeasurement(A1p)
    extractor3.SetInputFile("experiments/HERMES/a1pd.dat",25, 70-25)
    extractor3.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor3.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    A1p.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    A1d = experiment.GetMeasurement("A1d")
    if not A1d :
        A1d = NucDBMeasurement("A1d","A_{1}^{d}")
        experiment.AddMeasurement(A1d)
    A1d.ClearDataPoints()
    A1d.SetColor(4004)
    extractor4 = HERMESExtractor()
    extractor4.iValueRow = 6
    extractor4.istatErr  = 7
    extractor4.isysErr   = 8
    extractor4.SetMeasurement(A1d)
    extractor4.SetInputFile("experiments/HERMES/a1pd.dat",25,70-25)
    extractor4.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor4.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor4.Initialize()
    extractor4.ExtractAllValues()
    A1d.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.Clear()
    g1p.SetColor(4004)
    extractor5 = HERMESExtractor()
    extractor5.iValueRow=2
    extractor5.istatErr=3
    extractor5.isysErr=4
    extractor5.SetMeasurement(g1p)
    extractor5.SetInputFile("experiments/HERMES/g1pd.dat",25,70-25)
    extractor5.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor5.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor5.Initialize()
    extractor5.ExtractAllValues()
    g1p.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    g1d = experiment.GetMeasurement("g1d")
    if not g1d :
        g1d = NucDBMeasurement("g1d","g_{1}^{d}")
        experiment.AddMeasurement(g1d)
    g1d.Clear()
    g1d.SetColor(4004)
    extractor6           = HERMESExtractor()
    extractor6.iValueRow = 6
    extractor6.istatErr  = 7
    extractor6.isysErr   = 8
    extractor6.SetMeasurement(g1d)
    extractor6.SetInputFile("experiments/HERMES/g1pd.dat",25,70-25)
    extractor6.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor6.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor6.Initialize()
    extractor6.ExtractAllValues()
    g1d.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.SetColor(4004)
    extractor7           = HERMESExtractor()
    extractor7.iValueRow = 2
    extractor7.istatErr  = 3
    extractor7.isysErr   = 4
    extractor7.SetMeasurement(g1n)
    extractor7.SetInputFile("experiments/HERMES/g1n.dat",26,71-26)
    extractor7.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor7.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor7.Initialize()
    extractor7.ExtractAllValues()
    g1n.BuildGraph()


    # Refs for A2 and g2
    paper = NucDBPaper("paper-HERMES-proton","Measurement of the virtual-photon asymmetry A2 and the spin-structure function g2 of the proton")
    paper.SetShortRef("A. Airapetian et al., Eur. Phys. J. C 72, 1921 (2012)")
    paper.SetJournal("Eur.Phys.J.")
    paper.SetInSpire("http://inspirehep.net/record/1082840")
    paper.SetDOI("http://dx.doi.org/10.1140/epjc/s10052-012-1921-5")

    ref = NucDBReference("Ref-HERMES-g2-A2","HERMES Proton g2 and A2 reference")
    ref.SetDescription("HERMES A2p and g2p data from JAM website")
    ref.SetURL("https://misportal.jlab.org/jamDb/experiments/10100")
    ref.AddComment("A_perp is not available.")
    ref.AddComment("All systematic uncertainties are combined in the reported systematic errors. ")
    ref.GetPapers().Add(paper)

    # A2p 
    A2p = experiment.GetMeasurement("A2p")
    if not A2p :
        A2p = NucDBMeasurement("A2p","A_{2}^{p}")
        experiment.AddMeasurement(A2p)
    A2p.Clear()
    A2p.SetColor(4004)
    A2p.AddRef(ref)
    extractor = HERMESExtractor2()
    extractor.iValueRow = 5
    extractor.istatErr  = 6
    extractor.isysErr   = 7
    extractor.SetMeasurement(A2p)
    extractor.SetInputFile("experiments/HERMES/jam_dataset_A2_g2.csv",1)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.Initialize()
    extractor.ExtractAllValues()
    A2p.BuildGraph()

    # g2p
    g2p = experiment.GetMeasurement("g2p")
    if not g2p :
        g2p = NucDBMeasurement("g2p","g_{2}^{p}")
        experiment.AddMeasurement(g2p)
    g2p.Clear()
    g2p.AddRef(ref)
    g2p.SetColor(4004)
    extractor = HERMESExtractor3()
    extractor.iValueRow = 2
    extractor.istatErr  = 3
    extractor.isysErr   = 4
    extractor.SetMeasurement(g2p)
    extractor.SetInputFile("experiments/HERMES/jam_dataset_A2_g2.csv",1)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.Initialize()
    extractor.ExtractAllValues()
    g2p.BuildGraph()

    experiment.Print()
    manager.SaveExperiment(experiment)



