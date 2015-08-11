from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class SLACE155Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.iValueRowErr=5
        self.iSystErr=5

    def ParseLine(self):
        """ See input file for column structures
        """
        ixMin=0
        ixMax=1
        ixbjorken=2
        iQsq=3
        values = self.currentline.split()
        deltax=float(values[ixMax])-float(values[ixMin])
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iValueRowErr]))
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
        self.fCurrentDataPoint.Print()

class SLACE155ExtractorxA1p(SLACE155Extractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.iValueRowErr=5

class SLACE155Extractorxg2p(SLACE155Extractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.iValueRowErr=5
    def ParseLine(self):
        """ See input file for column structures
        """
        ixMin=0
        ixMax=1
        ixbjorken=2
        iQsq=3
        values = self.currentline.split()
        deltax=float(values[ixMax])-float(values[ixMin])
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow])/float(values[ixbjorken]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iValueRowErr])/float(values[ixbjorken]))
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

class SLACE155Extractorxg2d(SLACE155Extractorxg2p):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=6
        self.iValueRowErr=7
# -----------------------------------------------------------
class SLACE155Extractorxg1pAll(SLACE155Extractorxg2p):
    """ http://www.slac.stanford.edu/exp/e155/e155_results/tables/g1_p_all.txt
    """
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow    = 5
        self.iStatErr     = 6
        self.iSystErr     = 7
        self.iThetaSpec   = 0
        self.ixMin        = 1
        self.ixMax        = 2
        self.ixbjorken    = 3
        self.iQsq         = 4

    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        deltax =float(values[self.ixMax])-float(values[self.ixMin])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[self.ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQsq]),0.5)
        #
        Theta = self.fCurrentDataPoint.GetBinVariable("theta")
        Theta.SetBinValueSize(float(values[self.iThetaSpec]),0.01)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iStatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.iSystErr]))
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

class SLACE155Extractorxg1pOverF1pAll(SLACE155Extractorxg1pAll):
    """ http://www.slac.stanford.edu/exp/e155/e155_results/tables/g1_p_all.txt
    """
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow    = 8
        self.iStatErr     = 9
        self.iSystErr     = 10 
        self.iThetaSpec   = 0
        self.ixMin        = 1
        self.ixMax        = 2
        self.ixbjorken    = 3
        self.iQsq         = 4
# -----------------------------------------------------------
class SLACE155Extractor2(SLACE155Extractor):
    """ Data evolved to Q2 = 5GeV/c^2
    """
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=6
        self.iValueRowErr=7
        
    def ParseLine(self):
        """ See g1_pdn_Q2_5_comb.txt
        """
        ixMin=0
        ixMax=1
        ixbjorken=2
        values = self.currentline.split()
        deltax=float(values[ixMax])-float(values[ixMin])
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(5.0,0.5)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iValueRowErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        #self.fCurrentDataPoint.Print()
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

class SLACE155Extractorxg1p(SLACE155Extractor2):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=3
        self.iValueRowErr=4

class SLACE155Extractorxg1d(SLACE155Extractor2):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=5
        self.iValueRowErr=6
        
class SLACE155Extractorxg1n(SLACE155Extractor2):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=7
        self.iValueRowErr=8

# -----------------------------------------------------------
class SLACE155xExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow      = 9
        self.iValueRowErrHi = 10
        self.iValueRowErrLo = 11

    def SystErrxg2p(self,x):
        return 0.0016 - 0.0012*x

    def SystErrxg2d(self,x):
        return 0.0009 - 0.0008*x


    def ParseLine(self):
        """ See input file for column structures
        """
        ixMin     = 0
        ixMax     = 1
        ixbjorken = 2
        iQsq      = 3
        iQsqMin   = 4
        iQsqMax   = 5
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign

        xval = float(values[ixbjorken])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetMean(float(values[ixbjorken]))
        x.SetBinMinimum(float(values[ixMin]))
        x.SetBinMaximum(float(values[ixMax]))
        #x.Print()

        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetMean(float(values[iQsq]))
        Qsq.SetBinMinimum(float(values[iQsqMin]))
        Qsq.SetBinMaximum(float(values[iQsqMax]))
        #Qsq.Print()

        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow])/float(values[ixbjorken]))
        self.fCurrentDataPoint.GetStatError().SetErrorSize(float(values[self.iValueRowErrHi])/float(values[ixbjorken]),abs(float(values[self.iValueRowErrLo]))/float(values[ixbjorken]))
        self.fCurrentDataPoint.GetSystError().SetError(self.SystErrxg2p(xval)/xval)
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

class SLACE155xExtractorA(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow      = 6
        self.iValueRowErrHi = 7
        self.iValueRowErrLo = 8 

    def ParseLine(self):
        """ See input file for column structures
        """
        ixMin     = 0
        ixMax     = 1
        ixbjorken = 2
        iQsq      = 3
        iQsqMin   = 4
        iQsqMax   = 5
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign

        xval = float(values[ixbjorken])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetMean(float(values[ixbjorken]))
        x.SetBinMinimum(float(values[ixMin]))
        x.SetBinMaximum(float(values[ixMax]))
        #x.Print()

        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetMean(float(values[iQsq]))
        Qsq.SetBinMinimum(float(values[iQsqMin]))
        Qsq.SetBinMaximum(float(values[iQsqMax]))
        #Qsq.Print()

        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iValueRowErrHi]))
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
    
    experiment = manager.GetExperiment("SLAC E155")
    if not experiment :
        experiment = NucDBExperiment("SLAC E155","SLAC E155")
    
    g1pOverF1p = experiment.GetMeasurement("g1pOverF1p")
    if not g1pOverF1p :
        g1pOverF1p = NucDBMeasurement("g1pOverF1p","g_{1}^{p}/F_{1}^{p}")
    experiment.AddMeasurement(g1pOverF1p)
    g1pOverF1p.ClearDataPoints()
    g1pOverF1p.SetColor(4)
    
    g1dOverF1d = experiment.GetMeasurement("g1dOverF1d")
    if not g1dOverF1d :
        g1dOverF1d = NucDBMeasurement("g1dOverF1d","g_{1}^{d}/F_{1}^{d}")
    experiment.AddMeasurement(g1dOverF1d)
    g1dOverF1d.ClearDataPoints()
    g1dOverF1d.SetColor(4)

    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4)
    
    g1d = experiment.GetMeasurement("g1d")
    if not g1d :
        g1d = NucDBMeasurement("g1d","g_{1}^{d}")
        experiment.AddMeasurement(g1d)
    g1d.ClearDataPoints()
    g1d.SetColor(4)
    
    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.SetColor(4)
   
    #
    # SLAC E155 g1 at fixed Q2=5
    #
    #g1pExtractor = SLACE155Extractorxg1p()
    #g1pExtractor.SetMeasurement(g1p)
    #g1pExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt",19)
    #Ebeam    = NucDBBinnedVariable("Ebeam","E")
    #Ebeam.SetBinValueSize(48.35,0.0001)
    #Xbjorken = NucDBBinnedVariable("x","x")
    #Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    #g1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #g1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #g1pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #g1pExtractor.Initialize()
    #g1pExtractor.ExtractAllValues()
    #g1p.BuildGraph()
    #
    #g1nExtractor = SLACE155Extractorxg1n()
    #g1nExtractor.SetMeasurement(g1n)
    #g1nExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt",19)
    ##g1nExtractor.linestoskip=20
    #Xbjorken = NucDBBinnedVariable("x","x")
    #Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    #g1nExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #g1nExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #g1nExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #g1nExtractor.Initialize()
    #g1nExtractor.ExtractAllValues()
    #g1n.BuildGraph()
    #
    #
    #g1dExtractor = SLACE155Extractorxg1d()
    #g1dExtractor.SetMeasurement(g1d)
    #g1dExtractor.SetInputFile("experiments/SLAC-E155/g1_pdn_Q2_5_comb.txt",19)
    ##g1dExtractor.linestoskip=20
    #Xbjorken = NucDBBinnedVariable("x","x")
    #Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    #g1dExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #g1dExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #g1dExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #g1dExtractor.Initialize()
    #g1dExtractor.ExtractAllValues()
    #g1d.BuildGraph()
   
    # SLAC E155 g1p varing Q2 
    g1pExtractor = SLACE155Extractorxg1pAll()
    g1pExtractor.SetMeasurement(g1p)
    g1pExtractor.SetInputFile("experiments/SLAC-E155/g1_p_all.txt",23)
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    Ebeam.SetBinValueSize(48.35,0.0001)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    theta    = NucDBBinnedVariable("theta","#theta",0.001,0.001)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    g1pExtractor.Initialize()
    g1pExtractor.ExtractAllValues()
    g1p.BuildGraph()

    # SLAC E155 g1d varing Q2 
    g1pExtractor = SLACE155Extractorxg1pAll()
    g1pExtractor.SetMeasurement(g1d)
    g1pExtractor.SetInputFile("experiments/SLAC-E155/g1_d_all.txt",23)
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    Ebeam.SetBinValueSize(48.35,0.0001)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    theta    = NucDBBinnedVariable("theta","#theta")
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    g1pExtractor.Initialize()
    g1pExtractor.ExtractAllValues()
    g1d.BuildGraph()

    # SLAC E155 g1/F1 varing Q2 
    g1pExtractor = SLACE155Extractorxg1pOverF1pAll()
    g1pExtractor.SetMeasurement(g1pOverF1p)
    g1pExtractor.SetInputFile("experiments/SLAC-E155/g1_p_all.txt",23)
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    Ebeam.SetBinValueSize(48.35,0.0001)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    theta    = NucDBBinnedVariable("theta","#theta")
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    g1pExtractor.Initialize()
    g1pExtractor.ExtractAllValues()
    g1pOverF1p.BuildGraph()

    # SLAC E155 g1d/F1d varing Q2 
    g1pExtractor = SLACE155Extractorxg1pOverF1pAll()
    g1pExtractor.SetMeasurement(g1dOverF1d)
    g1pExtractor.SetInputFile("experiments/SLAC-E155/g1_d_all.txt",23)
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    Ebeam.SetBinValueSize(48.35,0.0001)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    theta    = NucDBBinnedVariable("theta","#theta")
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    g1pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    g1pExtractor.Initialize()
    g1pExtractor.ExtractAllValues()
    g1dOverF1d.BuildGraph()


    #
    # xg2
    #

    g2p = experiment.GetMeasurement("g2p")
    if not g2p :
        g2p = NucDBMeasurement("g2p","g_{2}^{p}")
        experiment.AddMeasurement(g2p)
    #g2p.ClearDataPoints()
    g2p.SetColor(4)
    
    g2d = experiment.GetMeasurement("g2d")
    if not g2d :
        g2d = NucDBMeasurement("g2d","g_{2}^{d}")
        experiment.AddMeasurement(g2d)
    #g2d.ClearDataPoints()
    g2d.SetColor(4)
    
    g2pExtractor = SLACE155Extractorxg2p()
    g2pExtractor.SetMeasurement(g2p)
    g2pExtractor.SetInputFile("experiments/SLAC-E155/xg2_pd_comb.txt",17)
    #g2pExtractor.linestoskip=18
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    g2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()
    g2p.BuildGraph()
    
    g2dExtractor = SLACE155Extractorxg2d()
    g2dExtractor.SetMeasurement(g2d)
    g2dExtractor.SetInputFile("experiments/SLAC-E155/xg2_pd_comb.txt",17)
    #g2dExtractor.linestoskip=18
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    g2dExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g2dExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g2dExtractor.Initialize()
    g2dExtractor.ExtractAllValues()
    g2d.BuildGraph()
    
    # ------ A1p
    A1p = experiment.GetMeasurement("A1p")
    if not A1p :
        A1p = NucDBMeasurement("A1p","A_{1}^{p}")
        experiment.AddMeasurement(A1p)
    #A1p.ClearDataPoints()
    A1p.SetColor(4)

    A1pExtractor = SLACE155ExtractorxA1p()
    A1pExtractor.SetMeasurement(A1p)
    A1pExtractor.SetInputFile("experiments/SLAC-E155/a1_p_2.txt",24)
    #A1pExtractor.linestoskip=18
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    theta    = NucDBBinnedVariable("theta","#theta",2.75,0.001)
    A1pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A1pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    A1pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    A1pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    A1pExtractor.Initialize()
    A1pExtractor.ExtractAllValues()

    A1pExtractor2 = SLACE155ExtractorxA1p()
    A1pExtractor2.SetMeasurement(A1p)
    A1pExtractor2.SetInputFile("experiments/SLAC-E155/a1_p_5.txt",24)
    theta    = NucDBBinnedVariable("theta","#theta",5.5,0.001)
    A1pExtractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A1pExtractor2.fCurrentDataPoint.AddBinVariable(Qsq)
    A1pExtractor2.fCurrentDataPoint.AddBinVariable(Ebeam)
    A1pExtractor2.fCurrentDataPoint.AddBinVariable(theta)
    A1pExtractor2.Initialize()
    A1pExtractor2.ExtractAllValues()
    
    A1pExtractor3 = SLACE155ExtractorxA1p()
    A1pExtractor3.SetMeasurement(A1p)
    A1pExtractor3.SetInputFile("experiments/SLAC-E155/a1_p_10.txt",24)
    theta    = NucDBBinnedVariable("theta","#theta",10.5,0.001)
    A1pExtractor3.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A1pExtractor3.fCurrentDataPoint.AddBinVariable(Qsq)
    A1pExtractor3.fCurrentDataPoint.AddBinVariable(Ebeam)
    A1pExtractor3.fCurrentDataPoint.AddBinVariable(theta)
    A1pExtractor3.Initialize()
    A1pExtractor3.ExtractAllValues()

    # ------ A1p
    A2p = experiment.GetMeasurement("A2p")
    if not A2p :
        A2p = NucDBMeasurement("A2p","A_{2}^{p}")
        experiment.AddMeasurement(A2p)
    #A2p.ClearDataPoints()
    A2p.SetColor(4)

    A2pExtractor = SLACE155ExtractorxA1p()
    A2pExtractor.SetMeasurement(A2p)
    A2pExtractor.SetInputFile("experiments/SLAC-E155/a2_pd_2.txt",21)
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    theta    = NucDBBinnedVariable("theta","#theta",2.75,0.001)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor.Initialize()
    A2pExtractor.ExtractAllValues()
    
    A2pExtractor2 = SLACE155ExtractorxA1p()
    A2pExtractor2.SetMeasurement(A2p)
    A2pExtractor2.SetInputFile("experiments/SLAC-E155/a2_pd_5.txt",21)
    theta    = NucDBBinnedVariable("theta","#theta",5.5,0.001)
    A2pExtractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A2pExtractor2.fCurrentDataPoint.AddBinVariable(Qsq)
    A2pExtractor2.fCurrentDataPoint.AddBinVariable(Ebeam)
    A2pExtractor2.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor2.Initialize()
    A2pExtractor2.ExtractAllValues()

    A2pExtractor3 = SLACE155ExtractorxA1p()
    A2pExtractor3.SetMeasurement(A2p)
    A2pExtractor3.SetInputFile("experiments/SLAC-E155/a2_pd_10.txt",21)
    theta    = NucDBBinnedVariable("theta","#theta",10.5,0.001)
    A2pExtractor3.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A2pExtractor3.fCurrentDataPoint.AddBinVariable(Qsq)
    A2pExtractor3.fCurrentDataPoint.AddBinVariable(Ebeam)
    A2pExtractor3.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor3.Initialize()
    A2pExtractor3.ExtractAllValues()

    experiment.Print()
    manager.SaveExperiment(experiment)
  
    # -----------------------------------------------------------
    # SLAC E155x
    # -----------------------------------------------------------
    #

    experiment = manager.GetExperiment("SLAC E155x")
    if not experiment :
        experiment = NucDBExperiment("SLAC E155x","SLAC E155x")
    
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    theta    = NucDBBinnedVariable("theta","#theta")

    # -----------------------------------------------------------
    #
    g2p = experiment.GetMeasurement("g2p")
    if not g2p :
        g2p = NucDBMeasurement("g2p","g_{2}^{p}")
        experiment.AddMeasurement(g2p)
    g2p.ClearDataPoints()
    g2p.SetColor(8)
    
    # -----------------------------------------------------------
    #
    g2d = experiment.GetMeasurement("g2d")
    if not g2d :
        g2d = NucDBMeasurement("g2d","g_{2}^{d}")
        experiment.AddMeasurement(g2d)
    g2d.ClearDataPoints()
    g2d.SetColor(8)

    # -----------------------------------------------------------
    #
    A2p = experiment.GetMeasurement("A2p")
    if not A2p :
        A2p = NucDBMeasurement("A2p","A_{2}^{p}")
        experiment.AddMeasurement(A2p)
    A2p.ClearDataPoints()
    A2p.SetColor(8)
    
    # -----------------------------------------------------------
    #
    A2d = experiment.GetMeasurement("A2d")
    if not A2d :
        A2d = NucDBMeasurement("A2d","A_{2}^{d}")
        experiment.AddMeasurement(A2d)
    A2d.SetColor(8)
    A2d.ClearDataPoints()
   
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(29.1,0.0001)
    theta.SetBinValueSize(2.75,0.01)
    g2pExtractor = SLACE155xExtractor()
    g2pExtractor.SetMeasurement(g2p)
    g2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    g2pExtractor.fCurrentDataPoint.AddBinVariable(theta)

    # E = 29.1 GeV
    Ebeam = g2pExtractor.fCurrentDataPoint.GetBinVariable("Ebeam")
    Ebeam.SetMean(29.1)
    theta = g2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    theta.SetMean(2.75)
    g2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",10,8)
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()

    theta = g2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    theta.SetMean(5.5)
    g2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",29,36-29)
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()

    theta = g2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    theta.SetMean(10.5)
    g2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",48,52-48)
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()

    # E = 32.3 GeV
    Ebeam = g2pExtractor.fCurrentDataPoint.GetBinVariable("Ebeam")
    Ebeam.SetMean(32.3)

    theta = g2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    theta.SetMean(2.75)
    g2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",63,71-63)
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()

    theta = g2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    theta.SetMean(5.5)
    g2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",82,89-82)
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()

    theta = g2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    theta.SetMean(10.5)
    g2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",100,105-100)
    g2pExtractor.Initialize()
    g2pExtractor.ExtractAllValues()


    # A2p
    Ebeam    = NucDBBinnedVariable("Ebeam","E")
    theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(29.1,0.0001)
    theta.SetBinValueSize(2.75,0.01)
    A2pExtractor = SLACE155xExtractorA()
    A2pExtractor.SetMeasurement(A2p)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    A2pExtractor.fCurrentDataPoint.AddBinVariable(theta)

    # E = 29.1 GeV
    Ebeam = A2pExtractor.fCurrentDataPoint.GetBinVariable("Ebeam")
    theta = A2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    #Ebeam    = NucDBBinnedVariable("Ebeam","E")
    #theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(29.1,0.001)
    theta.SetBinValueSize(2.75,0.001)
    A2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",10,8)
    A2pExtractor.Initialize()
    A2pExtractor.ExtractAllValues()

    theta = A2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    #Ebeam    = NucDBBinnedVariable("Ebeam","E")
    #theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(29.1,0.001)
    theta.SetBinValueSize(5.5 ,0.001)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",29,36-29)
    A2pExtractor.Initialize()
    A2pExtractor.ExtractAllValues()

    theta = A2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    #Ebeam    = NucDBBinnedVariable("Ebeam","E")
    #theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(29.1,0.001)
    theta.SetBinValueSize(10.5,0.001)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",48,52-48)
    A2pExtractor.Initialize()
    A2pExtractor.ExtractAllValues()

    # E = 32.3 GeV
    Ebeam = A2pExtractor.fCurrentDataPoint.GetBinVariable("Ebeam")
    theta = A2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    #Ebeam    = NucDBBinnedVariable("Ebeam","E")
    #theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(32.3,0.001)
    theta.SetBinValueSize(2.75,0.001)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",63,71-63)
    A2pExtractor.Initialize()
    A2pExtractor.ExtractAllValues()

    theta = A2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    #Ebeam    = NucDBBinnedVariable("Ebeam","E")
    #theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(32.3,0.001)
    theta.SetBinValueSize(5.5 ,0.001)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",82,89-82)
    A2pExtractor.Initialize()
    A2pExtractor.ExtractAllValues()

    theta = A2pExtractor.fCurrentDataPoint.GetBinVariable("theta")
    #Ebeam    = NucDBBinnedVariable("Ebeam","E")
    #theta    = NucDBBinnedVariable("theta","#theta")
    Ebeam.SetBinValueSize(32.3,0.001)
    theta.SetBinValueSize(10.5,0.001)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(Ebeam)
    #A2pExtractor.fCurrentDataPoint.AddBinVariable(theta)
    A2pExtractor.SetInputFile("experiments/SLAC-E155/E155_g2p_and_g2d.txt",100,105-100)
    A2pExtractor.Initialize()
    A2pExtractor.ExtractAllValues()

    # -----------------------------------------------------------
    #
    Gamma1p = experiment.GetMeasurement("Gamma1p")
    if not Gamma1p :
        Gamma1p = NucDBMeasurement("Gamma1p","#Gamma_{1}^{p}")
        experiment.AddMeasurement(Gamma1p)
    Gamma1p.SetColor(1)
    Gamma1p.ClearDataPoints()
    Q2    = NucDBBinnedVariable("Qsquared","Q^{2}",5.0,1.0)
    point = NucDBDataPoint(0.118,0.004+0.007)
    point.AddBinVariable(Q2)
    Gamma1p.AddDataPoint(point)


    experiment.Print()
    manager.SaveExperiment(experiment)
    

