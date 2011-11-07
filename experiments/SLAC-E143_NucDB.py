from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class SLACE143Extractor_g1dOverF1d(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=0
        iQsq=1
        ig1pOverF1p=2
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        #for i in range(9):
            #print str(i) + " = " + str(values[i])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.05)
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[ig1pOverF1p])
        self.fCurrentDataPoint.Print()

class SLACE143Extractor_g1pOverF1p(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=0
        iQsq=1
        ig1pOverF1p=2
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        #for i in range(9):
            #print str(i) + " = " + str(values[i])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.05)
        x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        Qsq.Print()
        self.fCurrentDataPoint.fValue=float(values[ig1pOverF1p])
        self.fCurrentDataPoint.Print()

manager = NucDBManager.GetManager()

experiment = manager.GetExperiment("SLAC_E143")
if not experiment :
    experiment = NucDBExperiment("SLAC_E143","SLAC_E143")

g1pOverF1p = experiment.GetMeasurement("g1pOverF1p")
if not g1pOverF1p :
    g1pOverF1p = NucDBMeasurement("g1pOverF1p","g_{1}^{p}/F_{1}^{p}")
experiment.fMeasurements.Add(g1pOverF1p)
g1pOverF1p.fColor=2

# create an extractor
g1pOverF1pExtractor = SLACE143Extractor_g1pOverF1p()
g1pOverF1pExtractor.SetMeasurement(g1pOverF1p)
g1pOverF1pExtractor.fCurrentDataPoint.fName = "g1p/F1p"
# create x variable
Xbjorken = NucDBBinnedVariable("x","x")
g1pOverF1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
# create x variable
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1pOverF1pExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)

g1pOverF1pExtractor.SetInputFile("experiments/SLAC-E143/G1F1_AV.txt")
g1pOverF1pExtractor.linestoskip=0

g1pOverF1pExtractor.Initialize()

g1pOverF1pExtractor.ExtractAllValues()

g1pOverF1p.BuildGraph()

#datapoint = NucDBDataPoint(1.0,0.2)
#Aperp.AddDataPoint(datapoint)
experiment.Print()

manager.SaveExperiment(experiment)



