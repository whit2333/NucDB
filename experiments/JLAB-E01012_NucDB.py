from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

""" @package JLAB-E01012_NucDB.py
    @deprecated  Replaced by E99117 results
"""


class JLABE01012Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.ixbjorken=0
        self.iQsq= 1
        self.iValueRow=2
        self.istatErr=3
        self.isysErrPlus=4
        self.isysErrMinus=5
        self.NumberOfLines=9999
        self.linesRead=0

    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        if self.linesRead > self.NumberOfLines :
             self.rowcut.currentValue=1
             return
        deltax=float(0.01)
        x = self.fCurrentDataPoint.GetBinVariable('x')
        if x :
            x.SetBinValueSize(float(values[self.ixbjorken]),deltax)
            #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq : 
            Qsq.SetBinValueSize(float(values[self.iQsq]),0.1)
            #Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
        self.fCurrentDataPoint.GetSystError().SetErrorSize(float(values[self.isysErrPlus]),float(values[self.isysErrMinus]))
        self.fCurrentDataPoint.CalculateTotalError()
        #self.fCurrentDataPoint.Print()
        self.linesRead+=1


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("JLAB_E01012")
    if not experiment :
        experiment = NucDBExperiment("JLAB_E01012","JLAB_E01012")

    A1n = experiment.GetMeasurement("A1n")
    if not A1n :
        A1n = NucDBMeasurement("A1n","A_{1}^{n}")
        experiment.AddMeasurement(A1n)
    A1n.ClearDataPoints()
    A1n.fColor=6

    extractor1 = JLABE01012Extractor()
    extractor1.SetMeasurement(A1n)
    extractor1.SetInputFile("experiments/JLAB-E01012/results.txt")
    extractor1.linestoskip=2
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
    g1nOverF1n.fColor=6
    extractor2 = JLABE01012Extractor()
    extractor2.iValueRow=6
    extractor2.istatErr=7
    extractor2.isysErrPlus=8
    extractor2.isysErrMinus=9
    extractor2.SetMeasurement(g1nOverF1n)
    extractor2.SetInputFile("experiments/JLAB-E01012/results.txt")
    extractor2.linestoskip=2
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor2.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    g1nOverF1n.BuildGraph()
    
    experiment.Print()
    manager.SaveExperiment(experiment)



