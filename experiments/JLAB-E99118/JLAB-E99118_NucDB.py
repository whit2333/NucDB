from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class E99118FullSigmaExtractor(NucDBRawDataExtractor) :
   #Full data extractor.Format :
   #   E     theta    E'      x       Q2     eps      W2    Gamma_v      sigma       stat       syst       rad1       rad2
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=8
        self.istatErr=9
        self.isystErr=10
        self.scale = 1000.0 # to convert ubarn into nbarn

    def ParseLine(self):
        """ See input file for column structures
        """
        iEbeam=0
        iEprime=2
        iTheta=1
        iQsq=4
        iW2=6
        #iNu=5
        iEpsilon=5
        iGamma=7
        ixbjorken=3
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
        #nu = self.fCurrentDataPoint.GetBinVariable('nu')
        #nu.SetBinValueSize(float(values[iNu]),0.001)
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
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow])*self.scale)
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr])*self.scale)
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isystErr])*self.scale)
        # TODO add rad cor errors
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

class E99118SigmaExtractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=3
        self.istatErr=4

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=2
        iQsq=0
        iW2=1
        #print self.currentline
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.001)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        #Qsq.Print()
        W2 = self.fCurrentDataPoint.GetBinVariable("W2")
        W2.SetBinValueSize(float(values[iW2]),0.001)
        #W2.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(0)
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

class E99118Extractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.istatErr=3

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=1
        iQsq=0
        #print self.currentline
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.001)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.001)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(0)
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



class E99118Extractor2(E99118Extractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.istatErr=5

class E99118Extractor3(E99118Extractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=6
        self.istatErr=7

class E99118SigmaExtractor2(E99118SigmaExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=5
        self.istatErr=6



if __name__ == "__main__":
    
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("JLAB-E99118")
    if not experiment :
        experiment = NucDBExperiment("JLAB-E99118","JLAB-E99118")

    # sigma full 
    sigma = experiment.GetMeasurement("sigma")
    if not sigma :
        sigma = NucDBMeasurement("sigma","#sigma^{p}")
        experiment.AddMeasurement(sigma)
    sigma.ClearDataPoints()
    sigma.SetColor(4020)
    sigmaExtractor = E99118FullSigmaExtractor()
    sigmaExtractor.SetMeasurement(sigma)
    sigmaExtractor.SetInputFile("experiments/JLAB-E99118/hydrogen.global.table.text",4)
    #sigmaExtractor.linestoskip=5
    #sigmaExtractor.NumberOfLines=1204-5
    Ebeam    = NucDBBinnedVariable("E","E")
    Eprime   = NucDBBinnedVariable("Eprime","E'")
    theta    = NucDBBinnedVariable("theta","#theta")
    #nu       = NucDBBinnedVariable("nu","#nu")
    Gamma    = NucDBBinnedVariable("Gamma","#Gamma")
    epsilon  = NucDBBinnedVariable("epsilon","#epsilon")
    Xbjorken = NucDBBinnedVariable("x","x")
    W2       = NucDBBinnedVariable("W2","W2")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Eprime)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(theta)
    #sigmaExtractor.fCurrentDataPoint.AddBinVariable(nu)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Gamma)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(epsilon)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    sigmaExtractor.fCurrentDataPoint.AddBinVariable(W2)
    sigmaExtractor.Initialize()
    sigmaExtractor.ExtractAllValues()
    sigma.BuildGraph()
   
#    # sigma_L 
#    sigmaL = experiment.GetMeasurement("sigma_L")
#    if not sigmaL :
#        sigmaL = NucDBMeasurement("sigma_L","#sigma_{L}^{p}")
#        experiment.AddMeasurement(sigmaL)
#    sigmaL.ClearDataPoints()
#    sigmaL.SetColor(4019)
#    sigmaLExtractor = E99118SigmaExtractor2()
#    sigmaLExtractor.SetMeasurement(sigmaL)
#    sigmaLExtractor.SetInputFile("experiments/JLAB-E99118/E99118_final.dat")
#    sigmaLExtractor.linestoskip=3
#    sigmaLExtractor.NumberOfLines=169-3
#    Xbjorken = NucDBBinnedVariable("x","x")
#    W2 = NucDBBinnedVariable("W2","W2")
#    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#    sigmaLExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
#    sigmaLExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
#    sigmaLExtractor.fCurrentDataPoint.AddBinVariable(W2)
#    sigmaLExtractor.Initialize()
#    sigmaLExtractor.ExtractAllValues()
#    sigmaL.BuildGraph()
#
#    # sigma_T 
#    sigmaT = experiment.GetMeasurement("sigma_T")
#    if not sigmaT :
#        sigmaT = NucDBMeasurement("sigma_T","#sigma_{T}^{p}")
#        experiment.AddMeasurement(sigmaT)
#    sigmaT.ClearDataPoints()
#    sigmaT.SetColor(4019)
#    sigmaTExtractor = E99118SigmaExtractor()
#    sigmaTExtractor.SetMeasurement(sigmaT)
#    sigmaTExtractor.SetInputFile("experiments/JLAB-E99118/E99118_final.dat")
#    sigmaTExtractor.linestoskip=3
#    sigmaTExtractor.NumberOfLines=169-3
#    Xbjorken = NucDBBinnedVariable("x","x")
#    W2 = NucDBBinnedVariable("W2","W2")
#    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#    sigmaTExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
#    sigmaTExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
#    sigmaTExtractor.fCurrentDataPoint.AddBinVariable(W2)
#    sigmaTExtractor.Initialize()
#    sigmaTExtractor.ExtractAllValues()
#    sigmaT.BuildGraph()
#
#    F2p = experiment.GetMeasurement("F2p")
#    if not F2p :
#        F2p = NucDBMeasurement("F2p","F_{2}^{p}")
#        experiment.AddMeasurement(F2p)
#    F2p.ClearDataPoints()
#    F2p.SetColor(4019)
#    F2pExtractor = E99118Extractor()
#    F2pExtractor.SetMeasurement(F2p)
#    F2pExtractor.SetInputFile("experiments/JLAB-E99118/E99118_sfcalc.dat")
#    F2pExtractor.linestoskip=2
#    F2pExtractor.NumberOfLines=168-1
#    Xbjorken = NucDBBinnedVariable("x","x")
#    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#    F2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
#    F2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
#    F2pExtractor.Initialize()
#    F2pExtractor.ExtractAllValues()
#    F2p.BuildGraph()
#
#    
#    F1p = experiment.GetMeasurement("F1p")
#    if not F1p :
#        F1p = NucDBMeasurement("F1p","F_{1}^{p}")
#        experiment.AddMeasurement(F1p)
#    F1p.ClearDataPoints()
#    F1p.SetColor(4019)
#    F1pExtractor = E99118Extractor2()
#    F1pExtractor.SetMeasurement(F1p)
#    F1pExtractor.SetInputFile("experiments/JLAB-E99118/E99118_sfcalc.dat")
#    F1pExtractor.linestoskip=2
#    F1pExtractor.NumberOfLines=168-1
#    Xbjorken = NucDBBinnedVariable("x","x")
#    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#    F1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
#    F1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
#    F1pExtractor.Initialize()
#    F1pExtractor.ExtractAllValues()
#    F1p.BuildGraph()
#
#    
#    FLp = experiment.GetMeasurement("FLp")
#    if not FLp :
#        FLp = NucDBMeasurement("FLp","F_{L}^{p}")
#        experiment.AddMeasurement(FLp)
#    FLp.ClearDataPoints()
#    FLp.SetColor(4019)
#    FLpExtractor = E99118Extractor3()
#    FLpExtractor.SetMeasurement(FLp)
#    FLpExtractor.SetInputFile("experiments/JLAB-E99118/E99118_sfcalc.dat")
#    FLpExtractor.linestoskip=2
#    FLpExtractor.NumberOfLines=168-1
#    Xbjorken = NucDBBinnedVariable("x","x")
#    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#    FLpExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
#    FLpExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
#    FLpExtractor.Initialize()
#    FLpExtractor.ExtractAllValues()
#    FLp.BuildGraph()
#
    experiment.Print()
    manager.SaveExperiment(experiment)



