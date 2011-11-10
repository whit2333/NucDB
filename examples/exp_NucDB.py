from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os


class SLACE143ExtractorR(NucDBRawDataExtractor):
    '''Extractor for experiment E143'''
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
            x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        if Qsq :
            Qsq.SetBinValueSize(float(values[iQsq]),0.1)
            Qsq.Print()
        epsilon = self.fCurrentDataPoint.GetBinVariable("epsilon")
        if epsilon :
            epsilon.SetBinValueSize(float(values[iEpsilon]),0.001)
            epsilon.Print()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.fSystematicError.SetError(float(values[self.isysErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()

# get the NucDB manager
manager = NucDBManager.GetManager()
# get/create the experiment
experiment = manager.GetExperiment("SLAC_E143")
if not experiment :
    experiment = NucDBExperiment("SLAC_E143","SLAC_E143")
# get/create the measurement
R = experiment.GetMeasurement("R") # get measurement if it already exists
if not R :
    R = NucDBMeasurement("R","R")
    experiment.AddMeasurement(R)
R.ClearDataPoints() # in case datapoints already exist
R.fColor=2 # default color used for plotting
# configure and run the extractor 
RExtractor = SLACE143ExtractorR()
RExtractor.SetMeasurement(R)
RExtractor.fCurrentDataPoint.fName = "R"
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
Epsilon = NucDBBinnedVariable("epsilon","#epsilon")
RExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
RExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
RExtractor.fCurrentDataPoint.fBinnedVariables.Add(Epsilon)
RExtractor.SetInputFile("experiments/SLAC-E143/R.dat") # relative to the NucDB directory
RExtractor.linestoskip=15 # data starts on line 15
RExtractor.Initialize()
RExtractor.ExtractAllValues()
R.BuildGraph()
# prints a summary of this experiments data
experiment.Print()
# Save the changes to this experiment 
manager.SaveExperiment(experiment)