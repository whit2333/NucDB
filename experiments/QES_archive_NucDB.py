from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV,NucDBReference
from NucDBExtractors import *
import os

class QESArchiveDataExtractor(NucDBRawDataExtractor) :
    #Full data extractor.Format :
    # Z,A E, Theta,nu,sigma, err,citation
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=5
        self.istatErr=6
        #self.isystErr=10
        self.scale = 1.0 # to convert ubarn into nbarn

    def ParseLine(self):
        """ See input file for column structures
        """
        iZ=0
        iA=1
        iEprime=2
        iTheta=3
        iNu=4
        #print self.currentline
        values = self.currentline.split()
        Ebeam=float(values[iNu])+flaot(values[iEprime])
        self.rowcut.currentValue=int(0) # does nothign
        Eb = self.fCurrentDataPoint.GetBinVariable('E')
        Eb.SetBinValueSize(float(Ebeam),0.001)
        #Eb.Print()
        Ep = self.fCurrentDataPoint.GetBinVariable('Eprime')
        Ep.SetBinValueSize(float(values[iEprime]),0.001)
        #Ep.Print()
        th = self.fCurrentDataPoint.GetBinVariable('theta')
        th.SetBinValueSize(float(values[iTheta]),0.001)
        #th.Print()
        nu = self.fCurrentDataPoint.GetBinVariable('nu')
        nu.SetBinValueSize(float(values[iNu]),0.001)
        Z = self.fCurrentDataPoint.GetBinVariable('Z')
        Z.SetBinValueSize(float(values[iZ]),0.001)
        A = self.fCurrentDataPoint.GetBinVariable('A')
        A.SetBinValueSize(float(values[iA]),0.001)
        #nu.Print()
        #eps = self.fCurrentDataPoint.GetBinVariable('epsilon')
        #eps.SetBinValueSize(float(values[iEpsilon]),0.001)
        #eps.Print()
        #ga = self.fCurrentDataPoint.GetBinVariable('Gamma')
        #ga.SetBinValueSize(float(values[iGamma]),0.001)
        #ga.Print()
        #x = self.fCurrentDataPoint.GetBinVariable('x')
        #x.SetBinValueSize(float(values[ixbjorken]),0.001)
        #x.Print()
        #Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        #Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        #Qsq.Print()
        #W2 = self.fCurrentDataPoint.GetBinVariable("W2")
        #W2.SetBinValueSize(float(values[iW2]),0.001)
        #W2.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow])*self.scale)
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr])*self.scale)
        #self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isystErr])*self.scale)
        #
        #W = self.fCurrentDataPoint.GetDependentVariable("W")
        #if not W :
        #    W   = NucDBInvariantMassDV()
        #    self.fCurrentDataPoint.AddDependentVariable(W)
        #if W :
        #    W.SetVariable(0,x)
        #    W.SetVariable(1,Qsq)
        #nu = self.fCurrentDataPoint.GetDependentVariable("nu")
        #if not nu :
        #    nu   = NucDBPhotonEnergyDV()
        #    self.fCurrentDataPoint.AddDependentVariable(nu)
        #if nu :
        #    nu.SetVariable(0,x)
        #    nu.SetVariable(1,Qsq)
        self.fCurrentDataPoint.CalculateDependentVariables()
        #self.fCurrentDataPoint.Print()

    def DeclareVariables(self) :
        Ebeam    = NucDBBinnedVariable("E","E")
        Eprime   = NucDBBinnedVariable("Eprime","E'")
        theta    = NucDBBinnedVariable("theta","#theta")
        nu       = NucDBBinnedVariable("nu","#nu")
        Z        = NucDBBinnedVariable("Z","Z")
        A        = NucDBBinnedVariable("A","A")
        Gamma    = NucDBBinnedVariable("Gamma","#Gamma")
        epsilon  = NucDBBinnedVariable("epsilon","#epsilon")
        Xbjorken = NucDBBinnedVariable("x","x")
        W2       = NucDBBinnedVariable("W2","W2")
        Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
        self.fCurrentDataPoint.AddBinVariable(Ebeam)
        self.fCurrentDataPoint.AddBinVariable(Eprime)
        self.fCurrentDataPoint.AddBinVariable(theta)
        self.fCurrentDataPoint.AddBinVariable(nu)
        self.fCurrentDataPoint.AddBinVariable(Z)
        self.fCurrentDataPoint.AddBinVariable(A)
        #self.fCurrentDataPoint.AddBinVariable(Gamma)
        #self.fCurrentDataPoint.AddBinVariable(epsilon)
        #self.fCurrentDataPoint.AddBinVariable(Xbjorken)
        #self.fCurrentDataPoint.AddBinVariable(Qsq)
        #self.fCurrentDataPoint.AddBinVariable(W2)



if __name__ == "__main__":
    
    manager = NucDBManager.GetManager(1)
   
    #3He_Dow1988rk.dat
    experiment = manager.GetExperiment("QES-Archive")
    if not experiment :
        experiment = NucDBExperiment("QES-Archive","QES-Archive")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma_3He")
    if not sigma :
        sigma = NucDBMeasurement("sigma_3He","#sigma^{^3He}")
        experiment.AddMeasurement(sigma)

    refname="Dow1988rk"
    sigma.ClearDataPoints()
    ref = NucDBReference("QES-Archive","QES Archive website")
    ref.SetURL("http://faculty.virginia.edu/qes-archive/index.html")
    ref.SetRefType(NucDBReference.kWebsite)
    ref.SetDescription("Quasielastic electron Nucleus scattering archive website.")
    sigma.AddRef(ref)
    ref1 = NucDBReference(refname,refname)
    ref.SetRefType(NucDBReference.kPublished)
    sigma.SetColor(1)
    sigmaExtractor = QESArchiveDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/QES_Archive/data/3He_"+refname+".dat")
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()

    experiment.Print()
    manager.SaveExperiment(experiment)

