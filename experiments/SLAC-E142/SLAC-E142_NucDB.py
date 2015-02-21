from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

class SLACE142Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iQsq=3
        self.iValueRow=4
        self.istatErr=5
        self.isysErr=6
        self.NumberOfLines=0
        self.linesRead=0
    
    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=2
        ixMin=0
        ixMax=1
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
        #self.fCurrentDataPoint.Print()
        self.linesRead+=1

if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("SLAC E142")
    if not experiment :
        experiment = NucDBExperiment("SLAC E142","SLAC E142")

    #HELIUM-3 
    g1He3 = experiment.GetMeasurement("g1He3")
    if not g1He3 :
        g1He3 = NucDBMeasurement("g1He3","g_{1}^{He3}")
        experiment.AddMeasurement(g1He3)
    g1He3.ClearDataPoints()
    g1He3.SetColor(4011)
    extractor1 = SLACE142Extractor()
    extractor1.iValueRow=7
    extractor1.istatErr=8
    extractor1.isysErr=9
    extractor1.SetMeasurement(g1He3)
    extractor1.SetInputFile("experiments/SLAC-E142/a1g1He3.dat",16,24-16)
    #extractor1.linestoskip=17
    #extractor1.NumberOfLines=24-17
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    g1He3.BuildGraph()

    A1He3 = experiment.GetMeasurement("A1He3")
    if not A1He3 :
        A1He3 = NucDBMeasurement("A1He3","A_{1}^{He3}")
        experiment.AddMeasurement(A1He3)
    A1He3.ClearDataPoints()
    A1He3.SetColor(4011)
    extractor2 = SLACE142Extractor()
    extractor2.iValueRow=4
    extractor2.isysErr=5
    extractor2.istatErr=6
    extractor2.SetMeasurement(A1He3)
    extractor2.SetInputFile("experiments/SLAC-E142/a1g1He3.dat",16,24-16)
    #extractor2.linestoskip=17
    #extractor2.NumberOfLines=24-17
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor2.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    A1He3.BuildGraph()
    
    g2He3 = experiment.GetMeasurement("g2He3")
    if not g2He3 :
        g2He3 = NucDBMeasurement("g2He3","g_{2}^{He3}")
        experiment.AddMeasurement(g2He3)
    g2He3.ClearDataPoints()
    g2He3.SetColor(4011)
    extractor3 = SLACE142Extractor()
    extractor3.iValueRow=7
    extractor3.istatErr=8
    extractor3.isysErr=9
    extractor3.SetMeasurement(g2He3)
    extractor3.SetInputFile("experiments/SLAC-E142/a2g2He3.dat",16,24-16)
    #extractor3.linestoskip=17
    #extractor3.NumberOfLines=24-17
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor3.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor3.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    g2He3.BuildGraph()

    A2He3 = experiment.GetMeasurement("A2He3")
    if not A2He3 :
        A2He3 = NucDBMeasurement("A2He3","A_{2}^{He3}")
        experiment.AddMeasurement(A2He3)
    A2He3.ClearDataPoints()
    A2He3.SetColor(4011)
    extractor4 = SLACE142Extractor()
    extractor4.iValueRow=4
    extractor4.isysErr=5
    extractor4.istatErr=6
    extractor4.SetMeasurement(A2He3)
    extractor4.SetInputFile("experiments/SLAC-E142/a1g1He3.dat",16,24-16)
    #extractor4.linestoskip=17
    #extractor4.NumberOfLines=24-17
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor4.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor4.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor4.Initialize()
    extractor4.ExtractAllValues()
    A2He3.BuildGraph()
    
    
    #Neutron 
    g1n = experiment.GetMeasurement("g1n")
    if not g1n :
        g1n = NucDBMeasurement("g1n","g_{1}^{n}")
        experiment.AddMeasurement(g1n)
    g1n.ClearDataPoints()
    g1n.SetColor(4011)
    extractor11 = SLACE142Extractor()
    extractor11.iValueRow=7
    extractor11.istatErr=8
    extractor11.isysErr=9
    extractor11.SetMeasurement(g1n)
    extractor11.SetInputFile("experiments/SLAC-E142/a1g1n.dat",18,26-18)
    #extractor11.linestoskip=19
    #extractor11.NumberOfLines=26-19
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor11.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor11.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor11.Initialize()
    extractor11.ExtractAllValues()
    g1n.BuildGraph()

    A1n = experiment.GetMeasurement("A1n")
    if not A1n :
        A1n = NucDBMeasurement("A1n","A_{1}^{n}")
        experiment.AddMeasurement(A1n)
    A1n.ClearDataPoints()
    A1n.SetColor(4011)
    extractor12 = SLACE142Extractor()
    extractor12.iValueRow=4
    extractor12.isysErr=5
    extractor12.istatErr=6
    extractor12.SetMeasurement(A1n)
    extractor12.SetInputFile("experiments/SLAC-E142/a1g1n.dat",18,26-18)
    #extractor12.linestoskip=19
    #extractor12.NumberOfLines=26-19
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor12.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor12.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor12.Initialize()
    extractor12.ExtractAllValues()
    A1n.BuildGraph()
    
    g2n = experiment.GetMeasurement("g2n")
    if not g2n :
        g2n = NucDBMeasurement("g2n","g_{2}^{n}")
        experiment.AddMeasurement(g2n)
    g2n.ClearDataPoints()
    g2n.SetColor(4011)
    extractor13 = SLACE142Extractor()
    extractor13.iValueRow=7
    extractor13.istatErr=8
    extractor13.isysErr=9
    extractor13.SetMeasurement(g2n)
    extractor13.SetInputFile("experiments/SLAC-E142/a2g2n.dat",17,25-17)
    #extractor13.linestoskip=18
    #extractor13.NumberOfLines=25-18
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor13.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor13.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor13.Initialize()
    extractor13.ExtractAllValues()
    g2n.BuildGraph()

    A2n = experiment.GetMeasurement("A2n")
    if not A2n :
        A2n = NucDBMeasurement("A2n","A_{2}^{n}")
        experiment.AddMeasurement(A2n)
    A2n.ClearDataPoints()
    A2n.SetColor(4011)
    extractor14 = SLACE142Extractor()
    extractor14.iValueRow=4
    extractor14.isysErr=5
    extractor14.istatErr=6
    extractor14.SetMeasurement(A2n)
    extractor14.SetInputFile("experiments/SLAC-E142/a2g2n.dat",17,25-17)
    #extractor14.linestoskip=18
    #extractor14.NumberOfLines=25-18
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor14.fCurrentDataPoint.AddBinVariable(Xbjorken)
    extractor14.fCurrentDataPoint.AddBinVariable(Qsq)
    extractor14.Initialize()
    extractor14.ExtractAllValues()
    A2n.BuildGraph()
    
    
    experiment.Print()
    manager.SaveExperiment(experiment)




