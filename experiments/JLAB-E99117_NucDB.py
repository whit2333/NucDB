from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
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
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.istatErr].lstrip('+')))
        self.fCurrentDataPoint.fSystematicError.SetError(float(values[self.isysErr].lstrip('+')))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()
        self.linesRead+=1


if __name__ == "__main__":
    manager = NucDBManager.GetManager()

    experiment = manager.GetExperiment("JLAB_E99117")
    if not experiment :
        experiment = NucDBExperiment("JLAB_E99117","JLAB_E99117")

    A1n = experiment.GetMeasurement("A1n")
    if not A1n :
        A1n = NucDBMeasurement("A1n","A_{1}^{n}")
        experiment.AddMeasurement(A1n)
    A1n.ClearDataPoints()
    A1n.fColor=6

    extractor1 = JLABE99117Extractor()
    extractor1.SetMeasurement(A1n)
    extractor1.SetInputFile("experiments/JLAB-E99117/plain.txt")
    extractor1.linestoskip=10
    extractor1.NumberOfLines=12-10
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    A1n.BuildGraph()
    
    g1nOverF1n = experiment.GetMeasurement("g1n/F1n")
    if not g1nOverF1n :
        g1nOverF1n = NucDBMeasurement("g1n/F1n","g_{1}^{n}/F_{1}^{n}")
        experiment.AddMeasurement(g1nOverF1n)
    g1nOverF1n.ClearDataPoints()
    g1nOverF1n.fColor=6
    extractor2 = JLABE99117Extractor()
    extractor2.SetMeasurement(g1nOverF1n)
    extractor2.SetInputFile("experiments/JLAB-E99117/plain.txt")
    extractor2.linestoskip=23
    extractor2.NumberOfLines=25-23
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    g1nOverF1n.BuildGraph()

    A1He3 = experiment.GetMeasurement("A1He3")
    if not A1He3 :
        A1He3 = NucDBMeasurement("A1He3","A_{1}^{He3}")
        experiment.AddMeasurement(g1nOverF1n)
    A1He3.ClearDataPoints()
    A1He3.fColor=6
    extractor3 = JLABE99117Extractor()
    extractor3.SetMeasurement(A1He3)
    extractor3.SetInputFile("experiments/JLAB-E99117/plain.txt.1")
    extractor3.QsqMin=0.27
    extractor3.QsqMax=0.5
    extractor3.linestoskip=10
    extractor3.NumberOfLines=12-10
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    A1He3.BuildGraph()

    A2He3 = experiment.GetMeasurement("A1He3")
    if not A2He3 :
        A2He3 = NucDBMeasurement("A2He3","A_{2}^{He3}")
        experiment.AddMeasurement(g1nOverF1n)
    A2He3.ClearDataPoints()
    A2He3.fColor=6
    extractor4 = JLABE99117Extractor()
    extractor4.SetMeasurement(A2He3)
    extractor4.SetInputFile("experiments/JLAB-E99117/plain.txt.1")
    extractor4.linestoskip=23
    extractor4.NumberOfLines=25-23
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor4.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor4.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor4.Initialize()
    extractor4.ExtractAllValues()
    A2He3.BuildGraph()

    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.fColor=6
    extractor5 = JLABE99117Extractor()
    extractor5.SetMeasurement(g1n)
    extractor5.SetInputFile("experiments/JLAB-E99117/plain.txt.1")
    extractor5.linestoskip=140
    extractor5.NumberOfLines=142-140
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor5.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor5.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor5.Initialize()
    extractor5.ExtractAllValues()
    g1n.BuildGraph()

    g2n = experiment.GetMeasurement("g2n")
    if not g2n :
        g2n = NucDBMeasurement("g2n","g_{2}^{n}")
        experiment.AddMeasurement(g2n)
    g2n.ClearDataPoints()
    g2n.fColor=6
    extractor6 = JLABE99117Extractor()
    extractor6.SetMeasurement(g2n)
    extractor6.SetInputFile("experiments/JLAB-E99117/plain.txt.1")
    extractor6.linestoskip=153
    extractor6.NumberOfLines=155-153
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor6.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
    extractor6.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    extractor6.Initialize()
    extractor6.ExtractAllValues()
    g2n.BuildGraph()
    
    experiment.Print()
    manager.SaveExperiment(experiment)



