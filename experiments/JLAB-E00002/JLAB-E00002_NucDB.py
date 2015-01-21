from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class JLABResDataExtractor(NucDBRawDataExtractor) :
    #Full data extractor.Format :
    #E        EP       Theta       Q^2           W^2       Nu       Epsilon    Gamma         x        Sigma         Uncertainty
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=9
        self.istatErr=10
        #self.isystErr=10
        self.scale = 1000.0 # to convert ubarn into nbarn

    def ParseLine(self):
        """ See input file for column structures
        """
        iEbeam=0
        iEprime=1
        iTheta=2
        iQsq=3
        iW2=4
        iNu=5
        iEpsilon=6
        iGamma=7
        ixbjorken=8
        #print self.currentline
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        Eb = self.fCurrentDataPoint.GetBinVariable('E')
        Eb.SetBinValueSize(float(values[iEbeam]),0.001)
        #Eb.Print()
        Ep = self.fCurrentDataPoint.GetBinVariable('Eprime')
        Ep.SetBinValueSize(float(values[iEprime]),0.001)
        #Ep.Print()
        th = self.fCurrentDataPoint.GetBinVariable('theta')
        th.SetBinValueSize(float(values[iTheta]),0.001)
        #th.Print()
        nu = self.fCurrentDataPoint.GetBinVariable('nu')
        nu.SetBinValueSize(float(values[iNu]),0.001)
        #nu.Print()
        eps = self.fCurrentDataPoint.GetBinVariable('epsilon')
        eps.SetBinValueSize(float(values[iEpsilon]),0.001)
        #eps.Print()
        ga = self.fCurrentDataPoint.GetBinVariable('Gamma')
        ga.SetBinValueSize(float(values[iGamma]),0.001)
        #ga.Print()
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.001)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        #Qsq.Print()
        W2 = self.fCurrentDataPoint.GetBinVariable("W2")
        W2.SetBinValueSize(float(values[iW2]),0.001)
        #W2.Print()
        W = self.fCurrentDataPoint.GetBinVariable("W")
        W.SetBinValueSize(math.sqrt(float(values[iW2])),0.001)
        #
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow])*self.scale)
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr])*self.scale)
        #self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isystErr])*self.scale)
        # TODO add rad cor errors
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




if __name__ == "__main__":
    
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("JLAB-E00002")
    if not experiment :
        experiment = NucDBExperiment("JLAB-E00002","JLAB-E00002")

    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4020)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/JLAB-E00002/edwin.dat")
    #sigmaExtractor.linestoskip=5
    #sigmaExtractor.NumberOfLines=1204-5
    # Variables
    Ebeam    = NucDBBinnedVariable("E","E")
    Eprime   = NucDBBinnedVariable("Eprime","E'")
    theta    = NucDBBinnedVariable("theta","#theta")
    nu       = NucDBBinnedVariable("nu","#nu")
    Gamma    = NucDBBinnedVariable("Gamma","#Gamma")
    epsilon  = NucDBBinnedVariable("epsilon","#epsilon")
    Xbjorken = NucDBBinnedVariable("x","x")
    W2       = NucDBBinnedVariable("W2","W2")
    W       = NucDBBinnedVariable("W","W")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Eprime)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(theta)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(nu)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Gamma)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(epsilon)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(W2)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(W)
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
   
    experiment.Print()
    manager.SaveExperiment(experiment)

