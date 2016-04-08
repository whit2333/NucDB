from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class JLABE99117Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=4
        self.isysErr=7
        self.istatErr=5
        self.NumberOfLines=0
        self.linesRead=0
        self.iQsq= 3
    
    def ParseLine(self):
        """ See input file for column structures
        """
        ixMin=0
        ixMax=1
        ixbjorken=2
        values = self.currentline.split()
        if self.linesRead > self.NumberOfLines :
             self.rowcut.currentValue=1
             return
        deltax=float(values[ixMax])-float(values[ixMin])
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
        #self.linesRead+=1


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("JLAB_E99117")
    if not experiment :
        experiment = NucDBExperiment("JLAB_E99117","JLAB_E99117")

    A1n = experiment.GetMeasurement("A1n")
    if not A1n :
        A1n = NucDBMeasurement("A1n","A_{1}^{n}")
        experiment.AddMeasurement(A1n)
    A1n.ClearDataPoints()
    A1n.SetColor(4007)
    extractor1 = JLABE99117Extractor()
    extractor1.SetMeasurement(A1n)
    extractor1.SetInputFile("experiments/JLAB-E99117/plain.txt",9,12-9)
    #extractor1.linestoskip=10
    #extractor1.NumberOfLines=12-10
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    A1n.BuildGraph()
    
    g1nOverF1n = experiment.GetMeasurement("g1n/F1n")
    if not g1nOverF1n :
        g1nOverF1n = NucDBMeasurement("g1n/F1n","g_{1}^{n}/F_{1}^{n}")
        experiment.AddMeasurement(g1nOverF1n)
    g1nOverF1n.ClearDataPoints()
    g1nOverF1n.SetColor(4007)
    extractor2 = JLABE99117Extractor()
    extractor2.SetMeasurement(g1nOverF1n)
    extractor2.SetInputFile("experiments/JLAB-E99117/plain.txt",22,25-22)
    #extractor2.linestoskip=23
    #extractor2.NumberOfLines=25-23
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor2.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    g1nOverF1n.BuildGraph()

    A1He3 = experiment.GetMeasurement("A1He3")
    if not A1He3 :
        A1He3 = NucDBMeasurement("A1He3","A_{1}^{He3}")
        experiment.AddMeasurement(g1nOverF1n)
    A1He3.ClearDataPoints()
    A1He3.SetColor(4007)
    extractor3 = JLABE99117Extractor()
    extractor3.SetMeasurement(A1He3)
    extractor3.SetInputFile("experiments/JLAB-E99117/plain.txt.1",9,12-9)
    extractor3.QsqMin=0.27
    extractor3.QsqMax=0.5
    #extractor3.linestoskip=10
    #extractor3.NumberOfLines=12-10
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor3.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor3.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    A1He3.BuildGraph()

    A2He3 = experiment.GetMeasurement("A1He3")
    if not A2He3 :
        A2He3 = NucDBMeasurement("A2He3","A_{2}^{He3}")
        experiment.AddMeasurement(g1nOverF1n)
    A2He3.ClearDataPoints()
    A2He3.SetColor(4007)
    extractor4 = JLABE99117Extractor()
    extractor4.SetMeasurement(A2He3)
    extractor4.SetInputFile("experiments/JLAB-E99117/plain.txt.1",22,25-22)
    #extractor4.linestoskip=23
    #extractor4.NumberOfLines=25-23
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor4.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor4.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor4.Initialize()
    extractor4.ExtractAllValues()
    A2He3.BuildGraph()

    g1He3OverF1He3 = experiment.GetMeasurement("g1He3/F1He3")
    if not g1He3OverF1He3 :
        g1He3OverF1He3 = NucDBMeasurement("g1He3/F1He3","g_{1}^{He3}/F_{1}^{He3}")
        experiment.AddMeasurement(g1He3OverF1He3)
    g1He3OverF1He3.ClearDataPoints()
    g1He3OverF1He3.SetColor(4007)
    extractor7 = JLABE99117Extractor()
    extractor7.SetMeasurement(g1He3OverF1He3)
    extractor7.SetInputFile("experiments/JLAB-E99117/plain.txt.1",35,38-35)
    #extractor7.linestoskip=36
    #extractor7.NumberOfLines=38-36
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor7.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor7.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor7.Initialize()
    extractor7.ExtractAllValues()
    g1He3OverF1He3.BuildGraph()

    g2He3OverF2He3 = experiment.GetMeasurement("g2He3/F2He3")
    if not g2He3OverF2He3 :
        g2He3OverF2He3 = NucDBMeasurement("g2He3/F2He3","g_{2}^{He3}/F_{2}^{He3}")
        experiment.AddMeasurement(g2He3OverF2He3)
    g2He3OverF2He3.ClearDataPoints()
    g2He3OverF2He3.SetColor(4007)
    extractor8 = JLABE99117Extractor()
    extractor8.SetMeasurement(g2He3OverF2He3)
    extractor8.SetInputFile("experiments/JLAB-E99117/plain.txt.1",48,51-48)
    #extractor8.linestoskip=49
    #extractor8.NumberOfLines=51-49
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor8.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor8.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor8.Initialize()
    extractor8.ExtractAllValues()
    g2He3OverF2He3.BuildGraph()

    g1He3 = experiment.GetMeasurement("g1He3")
    if not g1He3 :
        g1He3 = NucDBMeasurement("g1He3","g_{1}^{He3}")
        experiment.AddMeasurement(g1He3)
    g1He3.ClearDataPoints()
    g1He3.SetColor(4007)
    extractor9 = JLABE99117Extractor()
    extractor9.SetMeasurement(g1He3)
    extractor9.SetInputFile("experiments/JLAB-E99117/plain.txt.1",61,64-61)
    #extractor9.linestoskip=62
    #extractor9.NumberOfLines=64-62
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor9.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor9.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor9.Initialize()
    extractor9.ExtractAllValues()
    g1He3.BuildGraph()

    g2He3 = experiment.GetMeasurement("g2He3")
    if not g2He3 :
        g2He3 = NucDBMeasurement("g2He3","g_{2}^{He3}")
        experiment.AddMeasurement(g2He3)
    g2He3.ClearDataPoints()
    g2He3.SetColor(4007)
    extractor10 = JLABE99117Extractor()
    extractor10.SetMeasurement(g2He3)
    extractor10.SetInputFile("experiments/JLAB-E99117/plain.txt.1",74,77-74)
    #extractor10.linestoskip=75
    #extractor10.NumberOfLines=77-75
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor10.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor10.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor10.Initialize()
    extractor10.ExtractAllValues()
    g2He3.BuildGraph()

   # repeat of same data from E01012????? 
    A1n = experiment.GetMeasurement("A1n")
    if not A1n :
        A1n = NucDBMeasurement("A1n","A_{1}^{n}")
        experiment.AddMeasurement(A1n)
    A1n.ClearDataPoints()
    A1n.SetColor(4007)
    extractor11 = JLABE99117Extractor()
    extractor11.SetMeasurement(A1n)
    extractor11.SetInputFile("experiments/JLAB-E99117/plain.txt.1",87,90-87)
    #extractor11.linestoskip=88
    #extractor11.NumberOfLines=90-88
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor11.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor11.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor11.Initialize()
    extractor11.ExtractAllValues()
    A1n.BuildGraph()


    A2n = experiment.GetMeasurement("A2n")
    if not A2n :
        A2n = NucDBMeasurement("A2n","A_{2}^{n}")
        experiment.AddMeasurement(A2n)
    A2n.ClearDataPoints()
    A2n.SetColor(4007)
    extractor12 = JLABE99117Extractor()
    extractor12.SetMeasurement(A2n)
    extractor12.SetInputFile("experiments/JLAB-E99117/plain.txt.1",100,103-100)
    #extractor12.linestoskip=101
    #extractor12.NumberOfLines=103-101
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor12.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor12.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor12.Initialize()
    extractor12.ExtractAllValues()
    A2n.BuildGraph()

    # Repeat of same data from E01012
    g1nOverF1n = experiment.GetMeasurement("g1n/F1n")
    if not g1nOverF1n :
        g1nOverF1n = NucDBMeasurement("g1n/F1n","g_{1}^{n}/F_{1}^{n}")
        experiment.AddMeasurement(g1nOverF1n)
    g1nOverF1n.ClearDataPoints()
    g1nOverF1n.SetColor(4007)
    extractor13 = JLABE99117Extractor()
    extractor13.SetMeasurement(g1nOverF1n)
    extractor13.SetInputFile("experiments/JLAB-E99117/plain.txt.1",113,116-113)
    #extractor13.linestoskip=114
    #extractor13.NumberOfLines=116-114
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor13.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor13.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor13.Initialize()
    extractor13.ExtractAllValues()
    g1nOverF1n.BuildGraph()

    g2nOverF2n = experiment.GetMeasurement("g2n/F2n")
    if not g2nOverF2n :
        g2nOverF2n = NucDBMeasurement("g2n/F2n","g_{2}^{n}/F_{2}^{n}")
        experiment.AddMeasurement(g2nOverF2n)
    g2nOverF2n.ClearDataPoints()
    g2nOverF2n.SetColor(4007)
    extractor14 = JLABE99117Extractor()
    extractor14.SetMeasurement(g2nOverF2n)
    extractor14.SetInputFile("experiments/JLAB-E99117/plain.txt.1",113,116-113)
    #extractor14.linestoskip=114
    #extractor14.NumberOfLines=116-114
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor14.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor14.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor14.Initialize()
    extractor14.ExtractAllValues()
    g2nOverF2n.BuildGraph()

    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.SetColor(4007)
    extractor5 = JLABE99117Extractor()
    extractor5.SetMeasurement(g1n)
    extractor5.SetInputFile("experiments/JLAB-E99117/plain.txt.1",139,142-139)
    #extractor5.linestoskip=140
    #extractor5.NumberOfLines=142-140
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor5.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor5.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor5.Initialize()
    extractor5.ExtractAllValues()
    g1n.BuildGraph()

    g2n = experiment.GetMeasurement("g2n")
    if not g2n :
        g2n = NucDBMeasurement("g2n","g_{2}^{n}")
        experiment.AddMeasurement(g2n)
    g2n.ClearDataPoints()
    g2n.SetColor(4007)
    extractor6 = JLABE99117Extractor()
    extractor6.SetMeasurement(g2n)
    extractor6.SetInputFile("experiments/JLAB-E99117/plain.txt.1",152,155-152)
    #extractor6.linestoskip=153
    #extractor6.NumberOfLines=155-153
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor6.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor6.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor6.Initialize()
    extractor6.ExtractAllValues()
    g2n.BuildGraph()
    
    experiment.Print()
    manager.SaveExperiment(experiment)



