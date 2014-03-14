from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class JLABE03103DataExtractor(NucDBRawDataExtractor) :
    #Full data extractor.Format :
    #E        EP       Theta       Q^2           W^2       Nu       Epsilon    Gamma         x        Sigma         Uncertainty
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=3
        self.istatErr=4
        #self.isystErr=10

    def ParseLine(self):
        """ See input file for column structures
        """
        iPt = 2 
        iEbeam=0
        iEprime=1
        iTheta=2
        iQsq=1
        iW2=4
        iNu=5
        iEpsilon=6
        iGamma=7
        ixbjorken=0
        print self.currentline
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        #Eb = self.fCurrentDataPoint.GetBinVariable('E')
        #Eb.SetBinValueSize(float(values[iEbeam]),0.001)
        #Eb.Print()
        #Ep = self.fCurrentDataPoint.GetBinVariable('Eprime')
        #Ep.SetBinValueSize(float(values[iEprime]),0.001)
        #Ep.Print()
        #th = self.fCurrentDataPoint.GetBinVariable('theta')
        #th.SetBinValueSize(float(values[iTheta]),0.001)
        #th.Print()
        #nu = self.fCurrentDataPoint.GetBinVariable('nu')
        #nu.SetBinValueSize(float(values[iNu]),0.001)
        #nu.Print()
        #eps = self.fCurrentDataPoint.GetBinVariable('epsilon')
        #eps.SetBinValueSize(float(values[iEpsilon]),0.001)
        #eps.Print()
        #ga = self.fCurrentDataPoint.GetBinVariable('Gamma')
        #ga.SetBinValueSize(float(values[iGamma]),0.001)
        #ga.Print()
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.001)
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        Qsq.Print()
        Pt = self.fCurrentDataPoint.GetBinVariable("Pt")
        Pt.SetBinValueSize(float(values[iPt]),0.001)
        Pt.Print()
        #W2 = self.fCurrentDataPoint.GetBinVariable("W2")
        #W2.SetBinValueSize(float(values[iW2]),0.001)
        #W2.Print()
        #
        W = self.fCurrentDataPoint.GetDependentVariable("W")
        if not W :
            W   = NucDBInvariantMassDV()
            self.fCurrentDataPoint.AddDependentVariable(W)
        if W :
            W.SetVariable(0,x)
            W.SetVariable(1,Qsq)
        #nu = self.fCurrentDataPoint.GetDependentVariable("nu")
        #if not nu :
        #    nu   = NucDBPhotonEnergyDV()
        #    self.fCurrentDataPoint.AddDependentVariable(nu)
        #if nu :
        #    nu.SetVariable(0,x)
        #    nu.SetVariable(1,Qsq)
        self.fCurrentDataPoint.CalculateDependentVariables()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))

        #self.fCurrentDataPoint.Print()




if __name__ == "__main__":
    
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("JLAB-E03103")
    if not experiment :
        experiment = NucDBExperiment("JLAB-E03103","JLAB-E03103")

    # sigma full 
    R = experiment.GetMeasurement("R(positron/electron)")
    if not R :
        R = NucDBMeasurement("R(positron/electron)","R(e^+/e^-)")
        experiment.AddMeasurement(R)
    R.ClearDataPoints()
    R.SetColor(4020)
    RExtractor = JLABE03103DataExtractor() 
    RExtractor.SetMeasurement(R)
    RExtractor.SetInputFile("experiments/JLAB-E03103/HallC_E03103.dat")
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
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    Pt       = NucDBBinnedVariable("Pt","P_t")
    #RExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #RExtractor.fCurrentDataPoint.AddBinVariable(Eprime)
    #RExtractor.fCurrentDataPoint.AddBinVariable(theta)
    #RExtractor.fCurrentDataPoint.AddBinVariable(nu)
    #RExtractor.fCurrentDataPoint.AddBinVariable(Gamma)
    #RExtractor.fCurrentDataPoint.AddBinVariable(epsilon)
    RExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    RExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    RExtractor.fCurrentDataPoint.AddBinVariable(Pt)
    #RExtractor.fCurrentDataPoint.AddBinVariable(W2)
    RExtractor.Initialize()
    RExtractor.ExtractAllValues()
    R.BuildGraph()
   
    experiment.Print()
    manager.SaveExperiment(experiment)

