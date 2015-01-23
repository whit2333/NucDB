from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class CLASExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iQ2=0
        self.iValueRow=1
        self.istatErr=2
        self.isysErr=3
    
    def ParseLine(self):
        """ See input file for column structures
        """
        #iQsq=3
        values = self.currentline.split()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQ2]),0.01)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
        #self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr].lstrip('+')))
        #self.fCurrentDataPoint.Print()
        self.fCurrentDataPoint.CalculateTotalError()
        #self.linesRead+=1

class SLACE143Extractor(NucDBRawDataExtractor):
    ''' for rows with (xmin,xmax,x,qsq,V,Vstat,Vsys) '''
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.isysErr=5
        self.istatErr=6
        self.ixMin=0
        self.ixMax=1
        self.ixbjorken=2
        self.iQsq=3

    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        deltax=float(values[self.ixMax])-float(values[self.ixMin])
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[self.ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr]))
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

class SLACE143ExtractorR(NucDBRawDataExtractor):
    ''' for rows with (x,qsq,epsilon, V,Vstat,Vsys) '''
    def __init__(self):
        NucDBRawDataExtractor.__init__(self) 
        self.iValueRow=3
        self.isysErr=4
        self.istatErr=5

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken= 0 #
        iQsq=1
        iEpsilon=2
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetBinValueSize(float(values[ixbjorken]),0.005)
            #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq :
            Qsq.SetBinValueSize(float(values[iQsq]),0.1)
            #Qsq.Print()
        epsilon = self.fCurrentDataPoint.GetBinVariable("epsilon")
        if epsilon :
            epsilon.SetBinValueSize(float(values[iEpsilon]),0.001)
            #epsilon.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr]))
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

class SLACE143ExtractorA1p(NucDBRawDataExtractor):
    ''' for rows with (x,qsq, A,Astat,Asys) '''
    def __init__(self):
        NucDBRawDataExtractor.__init__(self) 
        self.iValueRow=2
        self.isysErr=3
        self.istatErr=4

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken= 0 #
        iQsq=1
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetBinValueSize(float(values[ixbjorken]),0.01)
            #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq :
            Qsq.SetBinValueSize(float(values[iQsq]),0.1)
            #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr]))
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

if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("SLAC_E143")
    if not experiment :
        experiment = NucDBExperiment("SLAC_E143","SLAC_E143")

    R = experiment.GetMeasurement("R") # get measurement if it already exists
    if not R :
        R = NucDBMeasurement("R","R")
        experiment.AddMeasurement(R)
    R.ClearDataPoints() # in case datapoints already exist
    R.SetColor(4012)# default color used for plotting

    RExtractor = SLACE143ExtractorR()
    RExtractor.SetMeasurement(R)
    RExtractor.fCurrentDataPoint.fName = "R"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Epsilon = NucDBBinnedVariable("epsilon","#epsilon")
    RExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    RExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    RExtractor.fCurrentDataPoint.AddBinVariable(Epsilon)
    RExtractor.SetInputFile("experiments/SLAC-E143/R.dat",14)
    #RExtractor.linestoskip=15
    RExtractor.Initialize()
    RExtractor.ExtractAllValues()
    R.BuildGraph()

    # PROTON DATA 
    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4012)# default color used for plotting
    g1pExtractor = SLACE143Extractor()
    g1pExtractor.SetMeasurement(g1p)
    g1pExtractor.fCurrentDataPoint.fName = "g1p"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g1pExtractor.SetInputFile("experiments/SLAC-E143/g1p.dat",10)
    #g1pExtractor.linestoskip=11
    g1pExtractor.Initialize()
    g1pExtractor.ExtractAllValues()
    g1p.BuildGraph()


    g2p = experiment.GetMeasurement("g2p")
    if not g2p :
        g2p = NucDBMeasurement("g2p","g_{2}^{p}")
        experiment.AddMeasurement(g2p)
    g2p.ClearDataPoints()
    g2p.SetColor(4012)# default color used for plotting
    g2pExtractor = SLACE143Extractor()
    g2pExtractor.SetMeasurement(g2p)
    g2pExtractor.fCurrentDataPoint.fName = "g2p"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    g2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g2pExtractor.SetInputFile("experiments/SLAC-E143/g2p.dat",10)
    #g2pExtractor.linestoskip=11
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()
    g2p.BuildGraph()

    A1p = experiment.GetMeasurement("A1p")
    if not A1p :
        A1p = NucDBMeasurement("A1p","A_{1}^{p}")
        experiment.AddMeasurement(A1p)
    A1p.ClearDataPoints()
    A1p.SetColor(4012)
    Extractor3 = SLACE143ExtractorA1p()
    Extractor3.SetMeasurement(A1p)
    Extractor3.fCurrentDataPoint.fName = "A1p"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor3.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor3.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor3.SetInputFile("experiments/SLAC-E143/a1p.dat",10)
    #Extractor3.linestoskip=11
    Extractor3.Initialize()
    Extractor3.ExtractAllValues()
    A1p.BuildGraph()
    #A1p.Print("data");

    A2p = experiment.GetMeasurement("A2p")
    if not A2p :
        A2p = NucDBMeasurement("A2p","A_{2}^{p}")
        experiment.AddMeasurement(A2p)
    A2p.ClearDataPoints()
    A2p.SetColor(4012)# default color used for plotting
    Extractor4 = SLACE143Extractor()
    Extractor4.SetMeasurement(A2p)
    Extractor4.fCurrentDataPoint.fName = "A2p"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor4.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor4.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor4.SetInputFile("experiments/SLAC-E143/a2p.dat",12)
    #Extractor4.linestoskip=13
    Extractor4.Initialize()
    Extractor4.ExtractAllValues()
    A2p.BuildGraph()

    # NEUTRON DATA 
    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.SetColor(4012)# default color used for plotting
    Extractor5 = SLACE143Extractor()
    Extractor5.SetMeasurement(g1n)
    Extractor5.fCurrentDataPoint.fName = "g1n"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor5.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor5.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor5.SetInputFile("experiments/SLAC-E143/g1n.dat",27)
    #Extractor5.linestoskip=28
    Extractor5.Initialize()
    Extractor5.ExtractAllValues()
    g1n.BuildGraph()


    g2n = experiment.GetMeasurement("g2n")
    if not g2n :
        g2n = NucDBMeasurement("g2n","g_{2}^{n}")
        experiment.AddMeasurement(g2n)
    g2n.ClearDataPoints()
    g2n.SetColor(4012)# default color used for plotting
    Extractor6 = SLACE143Extractor()
    Extractor6.SetMeasurement(g2n)
    Extractor6.fCurrentDataPoint.fName = "g2n"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor6.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor6.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor6.SetInputFile("experiments/SLAC-E143/g2n.dat",28)
    #Extractor6.linestoskip=29
    Extractor6.Initialize()
    Extractor6.ExtractAllValues()
    g2n.BuildGraph()

    A1n = experiment.GetMeasurement("A1n")
    if not A1n :
        A1n = NucDBMeasurement("A1n","A_{1}^{n}")
        experiment.AddMeasurement(A1n)
    A1n.ClearDataPoints()
    A1n.SetColor(4012)# default color used for plotting
    Extractor7 = SLACE143ExtractorA1p()
    Extractor7.SetMeasurement(A1n)
    Extractor7.fCurrentDataPoint.fName = "A1n"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor7.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor7.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor7.SetInputFile("experiments/SLAC-E143/a1n.dat",8)
    #Extractor7.linestoskip=9
    Extractor7.Initialize()
    Extractor7.ExtractAllValues()
    A1n.BuildGraph()

    A2n = experiment.GetMeasurement("A2n")
    if not A2n :
        A2n = NucDBMeasurement("A2n","A_{2}^{n}")
        experiment.AddMeasurement(A2n)
    A2n.ClearDataPoints()
    A2n.SetColor(4012)# default color used for plotting
    Extractor8 = SLACE143Extractor()
    Extractor8.SetMeasurement(A2n)
    Extractor8.fCurrentDataPoint.fName = "A2n"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor8.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor8.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor8.SetInputFile("experiments/SLAC-E143/a2n.dat",28)
    #Extractor8.linestoskip=29
    Extractor8.Initialize()
    Extractor8.ExtractAllValues()
    A2n.BuildGraph()

    # DEUTERON DATA 
    g1d = experiment.GetMeasurement("g1d")
    if not g1d :
        g1d = NucDBMeasurement("g1d","g_{1}^{d}")
        experiment.AddMeasurement(g1d)
    g1d.ClearDataPoints()
    g1d.SetColor(4012)# default color used for plotting
    Extractor5 = SLACE143Extractor()
    Extractor5.SetMeasurement(g1d)
    Extractor5.fCurrentDataPoint.fName = "g1d"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor5.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor5.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor5.SetInputFile("experiments/SLAC-E143/g1d.dat",27)
    #Extractor5.linestoskip=28
    Extractor5.Initialize()
    Extractor5.ExtractAllValues()
    g1d.BuildGraph()


    g2d = experiment.GetMeasurement("g2d")
    if not g2d :
        g2d = NucDBMeasurement("g2d","g_{2}^{d}")
        experiment.AddMeasurement(g2d)
    g2d.ClearDataPoints()
    g2d.SetColor(4012)# default color used for plotting
    Extractor6 = SLACE143Extractor()
    Extractor6.SetMeasurement(g2d)
    Extractor6.fCurrentDataPoint.fName = "g2d"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor6.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor6.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor6.SetInputFile("experiments/SLAC-E143/g2d.dat",28)
    #Extractor6.linestoskip=29
    Extractor6.Initialize()
    Extractor6.ExtractAllValues()
    g2d.BuildGraph()

    A1d = experiment.GetMeasurement("A1d")
    if not A1d :
        A1d = NucDBMeasurement("A1d","A_{1}^{d}")
        experiment.AddMeasurement(A1d)
    A1d.ClearDataPoints()
    A1d.SetColor(4012)# default color used for plotting
    Extractor7 = SLACE143ExtractorA1p()
    Extractor7.SetMeasurement(A1d)
    Extractor7.fCurrentDataPoint.fName = "A1d"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor7.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor7.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor7.SetInputFile("experiments/SLAC-E143/a1d.dat",28)
    #Extractor7.linestoskip=9
    Extractor7.Initialize()
    Extractor7.ExtractAllValues()
    A1d.BuildGraph()

    A2d = experiment.GetMeasurement("A2d")
    if not A2d :
        A2d = NucDBMeasurement("A2d","A_{2}^{d}")
        experiment.AddMeasurement(A2d)
    A2d.ClearDataPoints()
    A2d.SetColor(4012)# default color used for plotting
    Extractor8 = SLACE143Extractor()
    Extractor8.SetMeasurement(A2d)
    Extractor8.fCurrentDataPoint.fName = "A2d"
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    Extractor8.fCurrentDataPoint.AddBinVariable(Xbjorken)
    Extractor8.fCurrentDataPoint.AddBinVariable(Qsq)
    Extractor8.SetInputFile("experiments/SLAC-E143/a2d.dat",28)
    #Extractor8.linestoskip=29
    Extractor8.Initialize()
    Extractor8.ExtractAllValues()
    A2d.BuildGraph()


    g1p = experiment.GetMeasurement("Gamma1p")
    if not g1p :
        g1p = NucDBMeasurement("Gamma1p","#Gamma_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4001)
    extractor1 = CLASExtractor()
    extractor1.SetMeasurement(g1p)
    extractor1.SetInputFile("experiments/SLAC-E143/Gamma1_p.txt",1)
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()

    g1p = experiment.GetMeasurement("Gamma1d")
    if not g1p :
        g1p = NucDBMeasurement("Gamma1d","#Gamma_{1}^{d}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4001)
    extractor1 = CLASExtractor()
    extractor1.SetMeasurement(g1p)
    extractor1.SetInputFile("experiments/SLAC-E143/Gamma1_d.txt",1)
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()

    g1p = experiment.GetMeasurement("Gamma1n")
    if not g1p :
        g1p = NucDBMeasurement("Gamma1n","#Gamma_{1}^{n}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4001)
    extractor1 = CLASExtractor()
    extractor1.SetMeasurement(g1p)
    extractor1.SetInputFile("experiments/SLAC-E143/Gamma1_n.txt",1)
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    experiment.Print()

    manager.SaveExperiment(experiment)



