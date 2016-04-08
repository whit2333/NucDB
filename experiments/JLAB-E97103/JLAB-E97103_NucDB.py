from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class JLABE97103DataExtractor(NucDBRawDataExtractor) :
    #Full data extractor.Format :
    #E        EP       Theta       Q^2           W^2       Nu       Epsilon    Gamma         x        Sigma         Uncertainty
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=5
        self.istatErr=6
        self.isystErr=7

    def ParseLine(self):
        """ See input file for column structures
        """
        iEbeam    = 0
        iEprime   = 1
        iTheta    = 2
        ixbjorken = 3
        iQsq      = 4
        #print self.currentline
        values = self.currentline.split(',')
        print values[10]
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
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        #Qsq.Print()
        #Pt = self.fCurrentDataPoint.GetBinVariable("Pt")
        #Pt.SetBinValueSize(float(values[iPt]),0.001)
        #Pt.Print()
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
        nu = self.fCurrentDataPoint.GetDependentVariable("nu")
        if not nu :
            nu   = NucDBPhotonEnergyDV()
            self.fCurrentDataPoint.AddDependentVariable(nu)
        if nu :
            nu.SetVariable(0,x)
            nu.SetVariable(1,Qsq)
        self.fCurrentDataPoint.CalculateDependentVariables()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        #print float(values[self.istatErr])
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isystErr]))
        self.fCurrentDataPoint.CalculateTotalError()


if __name__ == "__main__":
    
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("JLAB-E97103")
    if not experiment :
        experiment = NucDBExperiment("JLAB-E97103","JLAB-E97103")

    # Variables
    Ebeam    = NucDBBinnedVariable("E","E")
    Eprime   = NucDBBinnedVariable("Eprime","E'")
    theta    = NucDBBinnedVariable("theta","#theta")
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    W2       = NucDBBinnedVariable("W2","W2")
    nu       = NucDBBinnedVariable("nu","#nu")
    Gamma    = NucDBBinnedVariable("Gamma","#Gamma")
    epsilon  = NucDBBinnedVariable("epsilon","#epsilon")
    Pt       = NucDBBinnedVariable("Pt","P_t")

    ###########################################################
    # g1n 
    A = experiment.GetMeasurement("g1n")
    if not A:
        A = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(A)
    A.ClearDataPoints()
    AExtractor = JLABE97103DataExtractor() 
    AExtractor.SetMeasurement(A)
    AExtractor.SetInputFile("experiments/JLAB-E97103/JLabE97103_He",23,5)
    AExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    AExtractor.fCurrentDataPoint.AddBinVariable(Eprime)
    AExtractor.fCurrentDataPoint.AddBinVariable(theta)
    AExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    AExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #AExtractor.fCurrentDataPoint.AddBinVariable(nu)
    #AExtractor.fCurrentDataPoint.AddBinVariable(Gamma)
    #AExtractor.fCurrentDataPoint.AddBinVariable(epsilon)
    #AExtractor.fCurrentDataPoint.AddBinVariable(Pt)
    #AExtractor.fCurrentDataPoint.AddBinVariable(W2)
    AExtractor.Initialize()
    AExtractor.ExtractAllValues()
    A.BuildGraph()
   
    # g2n 
    A = experiment.GetMeasurement("g2n")
    if not A:
        A = NucDBMeasurement("g2n","g_{2}^{n}")
        experiment.AddMeasurement(A)
    A.ClearDataPoints()
    AExtractor = JLABE97103DataExtractor() 
    AExtractor.SetMeasurement(A)
    AExtractor.iValueRow = 8
    AExtractor.istatErr  = 9
    AExtractor.isystErr  = 10 
    AExtractor.SetInputFile("experiments/JLAB-E97103/JLabE97103_He",23,5)
    AExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    AExtractor.fCurrentDataPoint.AddBinVariable(Eprime)
    AExtractor.fCurrentDataPoint.AddBinVariable(theta)
    AExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    AExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #AExtractor.fCurrentDataPoint.AddBinVariable(nu)
    #AExtractor.fCurrentDataPoint.AddBinVariable(Gamma)
    #AExtractor.fCurrentDataPoint.AddBinVariable(epsilon)
    #AExtractor.fCurrentDataPoint.AddBinVariable(Pt)
    #AExtractor.fCurrentDataPoint.AddBinVariable(W2)
    AExtractor.Initialize()
    AExtractor.ExtractAllValues()
    A.BuildGraph()

    ###########################################################
    # g1He3
    A = experiment.GetMeasurement("g1He3")
    if not A:
        A = NucDBMeasurement("g1He3","g_{1}^{^{3}He}")
        experiment.AddMeasurement(A)
    A.ClearDataPoints()
    AExtractor = JLABE97103DataExtractor() 
    AExtractor.SetMeasurement(A)
    AExtractor.iValueRow = 11 
    AExtractor.istatErr  = 12
    AExtractor.isystErr  = 13 
    AExtractor.SetInputFile("experiments/JLAB-E97103/JLabE97103_He",11,5)
    AExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    AExtractor.fCurrentDataPoint.AddBinVariable(Eprime)
    AExtractor.fCurrentDataPoint.AddBinVariable(theta)
    AExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    AExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    AExtractor.Initialize()
    AExtractor.ExtractAllValues()
   
    # g2He3
    A = experiment.GetMeasurement("g2He3")
    if not A:
        A = NucDBMeasurement("g2He3","g_{2}^{^{3}He}")
        experiment.AddMeasurement(A)
    A.ClearDataPoints()
    AExtractor = JLABE97103DataExtractor() 
    AExtractor.SetMeasurement(A)
    AExtractor.iValueRow = 14 
    AExtractor.istatErr  = 15
    AExtractor.isystErr  = 16 
    AExtractor.SetInputFile("experiments/JLAB-E97103/JLabE97103_He",11,5)
    AExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    AExtractor.fCurrentDataPoint.AddBinVariable(Eprime)
    AExtractor.fCurrentDataPoint.AddBinVariable(theta)
    AExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    AExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    AExtractor.Initialize()
    AExtractor.ExtractAllValues()

    experiment.Print()
    manager.SaveExperiment(experiment)

