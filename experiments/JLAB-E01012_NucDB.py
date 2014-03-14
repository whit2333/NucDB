from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
from ROOT import NucDBPaper,NucDBReference
import os

""" @package JLAB-E01012_NucDB.py
    @deprecated  Replaced by E99117 results
"""


class JLABE01012Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.ixbjorken=0
        self.iQsq= 1
        self.iValueRow=2
        self.istatErr=3
        self.isysErr=4
        self.isysErrPlus=4
        self.isysErrMinus=5
        self.NumberOfLines=9999
        self.linesRead=0

    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        if self.linesRead > self.NumberOfLines :
             self.rowcut.currentValue=1
             return
        deltax=float(0.01)
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
        #self.fCurrentDataPoint.GetSystError().SetErrorSize(float(values[self.isysErrPlus]),float(values[self.isysErrMinus]))
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


class JLABE01012Extractor2(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        #E,theta,W,x,Q2,A1,A1(stat),A1(sys)
        self.iEbeam        = 0
        self.itheta        = 1
        self.iW            = 2
        self.ixbjorken     = 3
        self.iQsq          = 4
        self.iValueRow     = 5
        self.istatErr      = 6
        self.isysErr       = 7
        #self.NumberOfLines = 9999
        #self.linesRead     = 0

    def ParseLine(self):
        """ Columns: E,theta,W,x,Q2,A1,A1(stat),A1(sys)
        """
        values = self.currentline.split(',')
        deltax = float(0.01)
        Ebeam  = self.fCurrentDataPoint.GetBinVariable('Ebeam')
        if Ebeam :
            Ebeam.SetBinValueSize(float(values[self.iEbeam]),deltax)
        theta = self.fCurrentDataPoint.GetBinVariable('theta')
        if theta :
            theta.SetBinValueSize(float(values[self.itheta]),deltax)
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetBinValueSize(float(values[self.ixbjorken]),deltax)
        W = self.fCurrentDataPoint.GetBinVariable('W')
        if W :
            W.SetBinValueSize(float(values[self.iW]),deltax)
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq : 
            Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr].lstrip('+')))
        self.fCurrentDataPoint.CalculateTotalError()
        nu = self.fCurrentDataPoint.GetDependentVariable("nu")
        if not nu :
            nu   = NucDBPhotonEnergyDV()
            self.fCurrentDataPoint.AddDependentVariable(nu)
        if nu :
            nu.SetVariable(0,x)
            nu.SetVariable(1,Qsq)
        self.fCurrentDataPoint.CalculateDependentVariables()

if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("JLAB-E01012")
    if not experiment :
        experiment = NucDBExperiment("JLAB-E01012","JLAB E01012")

    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    W        = NucDBBinnedVariable("W","W")
    theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam    = NucDBBinnedVariable("E","E")

    A1He3 = experiment.GetMeasurement("A1He3")
    if not A1He3 :
        A1He3 = NucDBMeasurement("A1He3","A_{1}^{3He}")
        experiment.AddMeasurement(A1He3)
    A1He3.ClearDataPoints()

    paper = NucDBPaper("Paper-JLAB-E01012","Paper from JLAB-E01012")
    paper.SetShortRef("P. Solvignon et al., Phys. Rev. Lett. 101, 182502 (2008)")
    paper.SetJournal("Phys.Rev.Lett.")
    paper.SetInSpire("http://inspirehep.net/record/782108")
    paper.SetDOI("http://dx.doi.org/10.1103/PhysRevLett.101.182502")

    ref = NucDBReference("Ref-JLAB-E01012","Helium-3 results from E01012")
    ref.SetDescription("A1 and g1 data from JAM website")
    ref.SetURL("http://inspirehep.net/record/782108 https://misportal.jlab.org/jamDb/experiments/10043")
    ref.SetDOI("http://dx.doi.org/10.1103/PhysRevLett.101.182502")
    ref.GetPapers().Add(paper)

    A1He3.AddRef(ref)

    extractor = JLABE01012Extractor2()
    extractor.SetMeasurement(A1He3)
    extractor.SetInputFile("experiments/JLAB-E01012/jam_dataset_3He_A1.csv",1)
    extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor.fCurrentDataPoint.AddBinVariable(W)
    extractor.fCurrentDataPoint.AddBinVariable(theta)
    extractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    extractor.Initialize()
    extractor.ExtractAllValues()
    A1He3.BuildGraph()
    
    #A1n = experiment.GetMeasurement("A1n")
    #if not A1n :
    #    A1n = NucDBMeasurement("A1n","A_{1}^{n}")
    #    experiment.AddMeasurement(A1n)
    #A1n.ClearDataPoints()

    ## Not sure about this data set....
    #extractor1 = JLABE01012Extractor()
    #extractor1.SetMeasurement(A1n)
    ##extractor1.SetInputFile("experiments/JLAB-E01012/plain.txt",11)
    #extractor1.SetInputFile("experiments/JLAB-E01012/E01012.dat",0)
    ##extractor1.linestoskip=2
    #extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    #extractor1.Initialize()
    #extractor1.ExtractAllValues()
    #A1n.BuildGraph()
    
    experiment.Print()
    if not experiment.GetRefs().GetEntries() : 
        print " Warning: Experiment contains no references!"

    manager.SaveExperiment(experiment)

