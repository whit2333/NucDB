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
        self.scale = 1.0 # to convert ubarn into nbarn

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
        print self.currentline
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        Eb = self.fCurrentDataPoint.GetBinVariable('E')
        Eb.SetBinValueSize(float(values[iEbeam]),0.001)
        Eb.Print()
        Ep = self.fCurrentDataPoint.GetBinVariable('Eprime')
        Ep.SetBinValueSize(float(values[iEprime]),0.001)
        Ep.Print()
        th = self.fCurrentDataPoint.GetBinVariable('theta')
        th.SetBinValueSize(float(values[iTheta]),0.001)
        th.Print()
        nu = self.fCurrentDataPoint.GetBinVariable('nu')
        nu.SetBinValueSize(float(values[iNu]),0.001)
        nu.Print()
        eps = self.fCurrentDataPoint.GetBinVariable('epsilon')
        eps.SetBinValueSize(float(values[iEpsilon]),0.001)
        eps.Print()
        ga = self.fCurrentDataPoint.GetBinVariable('Gamma')
        ga.SetBinValueSize(float(values[iGamma]),0.001)
        ga.Print()
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.001)
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        Qsq.Print()
        W2 = self.fCurrentDataPoint.GetBinVariable("W2")
        W2.SetBinValueSize(float(values[iW2]),0.001)
        W2.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow])*self.scale)
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr])*self.scale)
        #self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isystErr])*self.scale)
        # TODO add rad cor errors
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
        #self.fCurrentDataPoint.Print()

    def DeclareVariables(self) :
        Ebeam    = NucDBBinnedVariable("E","E")
        Eprime   = NucDBBinnedVariable("Eprime","E'")
        theta    = NucDBBinnedVariable("theta","#theta")
        nu       = NucDBBinnedVariable("nu","#nu")
        Gamma    = NucDBBinnedVariable("Gamma","#Gamma")
        epsilon  = NucDBBinnedVariable("epsilon","#epsilon")
        Xbjorken = NucDBBinnedVariable("x","x")
        W2       = NucDBBinnedVariable("W2","W2")
        Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
        self.fCurrentDataPoint.AddBinVariable(Ebeam)
        self.fCurrentDataPoint.AddBinVariable(Eprime)
        self.fCurrentDataPoint.AddBinVariable(theta)
        self.fCurrentDataPoint.AddBinVariable(nu)
        self.fCurrentDataPoint.AddBinVariable(Gamma)
        self.fCurrentDataPoint.AddBinVariable(epsilon)
        self.fCurrentDataPoint.AddBinVariable(Xbjorken)
        self.fCurrentDataPoint.AddBinVariable(Qsq)
        self.fCurrentDataPoint.AddBinVariable(W2)



if __name__ == "__main__":
    
    manager = NucDBManager.GetManager(1)
   
    # SLAC E140x
    experiment = manager.GetExperiment("SLAC-E140x")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E140x","SLAC-E140x")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e140x.txt",5)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)

    # SLAC E140
    experiment = manager.GetExperiment("SLAC-E140")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E140","SLAC-E140")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e140.txt",5)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)

    # SLAC NE11 
    experiment = manager.GetExperiment("SLAC-NE11")
    if not experiment :
        experiment = NucDBExperiment("SLAC-NE11","SLAC-NE11")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/ne11.txt",5)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)

    # SLAC E133
    experiment = manager.GetExperiment("SLAC-E133")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E133","SLAC-E133")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e133.txt",6)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)

    # SLAC E8920
    experiment = manager.GetExperiment("SLAC-E8920")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E8920","SLAC-E8920")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e8920.txt",6)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)


    # SLAC E891
    experiment = manager.GetExperiment("SLAC-E891")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E891","SLAC-E891")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e891.txt",6)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)


    # SLAC E87
    experiment = manager.GetExperiment("SLAC-E87")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E87","SLAC-E87")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e87.txt",6)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)


    # SLAC E61
    experiment = manager.GetExperiment("SLAC-E61")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E61","SLAC-E61")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e61.txt",6)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)


    # SLAC E49b
    experiment = manager.GetExperiment("SLAC-E49b")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E49b","SLAC-E49b")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e49b.txt",6)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)


    # SLAC E49a10
    experiment = manager.GetExperiment("SLAC-E49a10")
    if not experiment :
        experiment = NucDBExperiment("SLAC-E49a10","SLAC-E49a10")
    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4021)
    sigmaExtractor = JLABResDataExtractor() 
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/RESData/e49a10.txt",6)
    sigmaExtractor.DeclareVariables()
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)

