from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV
from NucDBExtractors import *
import os

class HERMESExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=6
        self.isysErr=8
        self.istatErr=7
        self.NumberOfLines=0
        self.linesRead=0
        self.iQsq=1
    
    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=0
        values = self.currentline.split()
        if self.linesRead > self.NumberOfLines :
             self.rowcut.currentValue=1
             return
        deltax=0.01
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),deltax)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
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
        self.fCurrentDataPoint.CalculateDependentVariables()
        #
        #self.fCurrentDataPoint.Print()
        self.linesRead+=1


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("HERMES")
    if not experiment :
        experiment = NucDBExperiment("HERMES","HERMES")

    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.fColor=6
    extractor1 = HERMESExtractor()
    extractor1.SetMeasurement(g1p)
    extractor1.SetInputFile("experiments/HERMES/g1p")
    extractor1.linestoskip=25
    extractor1.NumberOfLines=44-25
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    g1p.BuildGraph()
    
    g1pOverF1p = experiment.GetMeasurement("g1p/F1p")
    if not g1pOverF1p :
        g1pOverF1p = NucDBMeasurement("g1p/F1p","g_{1}^{p}/F_{1}^{p}")
        experiment.AddMeasurement(g1pOverF1p)
    g1pOverF1p.ClearDataPoints()
    g1pOverF1p.fColor=6
    extractor2 = HERMESExtractor()
    extractor2.iValueRow=2
    extractor2.istatErr=3
    extractor2.isysErr=4
    extractor2.SetMeasurement(g1pOverF1p)
    extractor2.SetInputFile("experiments/HERMES/g1p")
    extractor2.linestoskip=25
    extractor2.NumberOfLines=44-25
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor2.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    g1pOverF1p.BuildGraph()
    
    #Using data set with most bins .... @todo add all binning schemes?
    A1p = experiment.GetMeasurement("A1p")
    if not A1p :
        A1p = NucDBMeasurement("A1p","A_{1}^{p}")
        experiment.AddMeasurement(A1p)
    A1p.ClearDataPoints()
    A1p.fColor=6
    extractor3 = HERMESExtractor()
    extractor3.iValueRow=2
    extractor3.istatErr=3
    extractor3.isysErr=4
    extractor3.SetMeasurement(A1p)
    extractor3.SetInputFile("experiments/HERMES/a1pd.dat")
    extractor3.linestoskip=26
    extractor3.NumberOfLines=70-26
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor3.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor3.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    A1p.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    A1d = experiment.GetMeasurement("A1d")
    if not A1d :
        A1d = NucDBMeasurement("A1d","A_{1}^{d}")
        experiment.AddMeasurement(A1d)
    A1d.ClearDataPoints()
    A1d.fColor=6
    extractor4 = HERMESExtractor()
    extractor4.iValueRow=6
    extractor4.istatErr=7
    extractor4.isysErr=8
    extractor4.SetMeasurement(A1d)
    extractor4.SetInputFile("experiments/HERMES/a1pd.dat")
    extractor4.linestoskip=26
    extractor4.NumberOfLines=70-26
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor4.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor4.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor4.Initialize()
    extractor4.ExtractAllValues()
    A1d.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(A1p)
    #g1p.ClearDataPoints()
    g1p.fColor=6
    extractor5 = HERMESExtractor()
    extractor5.iValueRow=2
    extractor5.istatErr=3
    extractor5.isysErr=4
    extractor5.SetMeasurement(g1p)
    extractor5.SetInputFile("experiments/HERMES/g1pd.dat")
    extractor5.linestoskip=26
    extractor5.NumberOfLines=70-26
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor5.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor5.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor5.Initialize()
    extractor5.ExtractAllValues()
    g1p.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    g1d = experiment.GetMeasurement("g1d")
    if not g1d :
        g1d = NucDBMeasurement("g1d","g_{1}^{d}")
        experiment.AddMeasurement(g1d)
    #A1d.ClearDataPoints()
    g1d.fColor=6
    extractor6 = HERMESExtractor()
    extractor6.iValueRow=6
    extractor6.istatErr=7
    extractor6.isysErr=8
    extractor6.SetMeasurement(g1d)
    extractor6.SetInputFile("experiments/HERMES/g1pd.dat")
    extractor6.linestoskip=26
    extractor6.NumberOfLines=70-26
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor6.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor6.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor6.Initialize()
    extractor6.ExtractAllValues()
    g1d.BuildGraph()

    #Using data set with most bins .... @todo add all binning schemes?
    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.fColor=6
    extractor7 = HERMESExtractor()
    extractor7.iValueRow=2
    extractor7.istatErr=3
    extractor7.isysErr=4
    extractor7.SetMeasurement(g1n)
    extractor7.SetInputFile("experiments/HERMES/g1n.dat")
    extractor7.linestoskip=27
    extractor7.NumberOfLines=71-27
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor7.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor7.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor7.Initialize()
    extractor7.ExtractAllValues()
    g1n.BuildGraph()

    experiment.Print()
    manager.SaveExperiment(experiment)



