from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV,NucDBReference
from NucDBExtractors import *
#from NucDB.Kine import *
import os

# For A1
class CLASg1Extractor(NucDBRawDataExtractor):
    # iw iq W Q2 epsln g1/F1 stat syst c_s f c_1 PBPT f_RC A_RC  D'eps*R C_g2 Aparr  err
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.linestoskip   = 1
        self.iValueRow     = 17
        self.istatErr      = 18
        self.NumberOfLines = 0
        self.linesRead     = 0
        self.iQsq          = 3
        self.iW            = 2
    
    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        W = self.fCurrentDataPoint.GetBinVariable("W")
        W.SetBinValueSize(float(values[self.iW]),0.1)
        Wval = float(W.GetMean())
        #Qsq.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
        Qsqval = float(Qsq.GetMean())
        #Qsq.Print()
        deltax=0.01
        x = self.fCurrentDataPoint.GetBinVariable("x")
        #xVal = float(xBjorken_WQsq(Wval,Qsqval))
        xVal = Qsqval / (Wval * Wval + Qsqval - 0.938 * 0.938)
        x.SetBinValueSize(xVal,deltax)
        #x.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
        #self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr].lstrip('+')))
        self.fCurrentDataPoint.CalculateTotalError()
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
        #self.linesRead+=1

# for G1/F1
class CLASg1Extractor2(CLASg1Extractor):
    def __init__(self):
        CLASg1Extractor.__init__(self)
        self.iValueRow=5
        self.istatErr=6
        self.isysErr=7


if __name__ == "__main__":

    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("CLAS-g1")
    if not experiment :
        experiment = NucDBExperiment("CLAS-g1","CLAS g1")

    Xbjorken = NucDBBinnedVariable("x","x")
    Dprime   = NucDBBinnedVariable("Dprime","D^{'}")
    Qsq      = NucDBBinnedVariable("Qsquared","Q^{2}")
    W        = NucDBBinnedVariable("W","W")
    Ebeam    = NucDBBinnedVariable("E","E")
    Ebeam.SetValueSize(5.9,0.01)

    ref = NucDBReference("CLAS_g1","Precision measurements of g1 of the proton and the deuteron with 6 GeV electrons")
    ref.SetURL("http://inspirehep.net/record/1292133")
    ref.SetDOI("http://dx.doi.org/10.1103/PhysRevC.90.025212")


    A1p = experiment.GetMeasurement("Aparap")
    if not A1p :
        A1p = NucDBMeasurement("Aparap","A_{#parallel}^{p}")
        experiment.AddMeasurement(A1p)
    A1p.ClearDataPoints()
    A1p.AddRef(ref)
    #A1p.SetColor(4018)
    extractor1 = CLASg1Extractor()
    extractor1.SetMeasurement(A1p)
    extractor1.SetInputFile("experiments/CLAS/g1/incltblwp.txt",1)
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.fCurrentDataPoint.AddBinVariable(Ebeam)
    extractor1.fCurrentDataPoint.AddBinVariable(W)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    A1p.BuildGraph()

    g1pOverF1p = experiment.GetMeasurement("g1pOverF1p")
    if not g1pOverF1p :
        g1pOverF1p = NucDBMeasurement("g1pOverF1p","g_{1}^{p}/F_{1}^{p}")
        experiment.AddMeasurement(g1pOverF1p)
    g1pOverF1p.ClearDataPoints()
    g1pOverF1p.AddRef(ref)
    #g1pOverF1p.SetColor(4018)
    extractor1 = CLASg1Extractor2()
    extractor1.SetMeasurement(g1pOverF1p)
    extractor1.SetInputFile("experiments/CLAS/g1/incltblwp.txt",1)
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.fCurrentDataPoint.AddBinVariable(Ebeam)
    extractor1.fCurrentDataPoint.AddBinVariable(W)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    g1pOverF1p.BuildGraph()
    experiment.Print()
    manager.SaveExperiment(experiment)



