from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
from NucDBExtractors import *
import os

class OLDSLACExtractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow=2
        self.isysErr=3
        self.istatErr=4

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=0
        iQsq=1
        values = self.currentline.split()
        self.rowcut.currentValue=int(0) # does nothign
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.01)
        #x.Print()
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.1)
        #Qsq.Print()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr]))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        #self.fCurrentDataPoint.Print()

manager = NucDBManager.GetManager(1)

experiment = manager.GetExperiment("OLDSLAC")
if not experiment :
    experiment = NucDBExperiment("OLDSLAC","OLDSLAC")

F2p = experiment.GetMeasurement("F2p")
if not F2p :
    F2p = NucDBMeasurement("F2p","F_{2}^{p}")
    experiment.AddMeasurement(F2p)
F2p.ClearDataPoints()
F2p.SetColor(1)

F2pExtractor = OLDSLACExtractor()
F2pExtractor.SetMeasurement(F2p)
F2pExtractor.SetInputFile("experiments/OLDSLAC/f2prot.dat")
F2pExtractor.linestoskip=0
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#F2pExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Xbjorken)
#F2pExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Qsq)
F2pExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
F2pExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
F2pExtractor.Initialize()
F2pExtractor.ExtractAllValues()
F2p.BuildGraph()
print "testing 1,2,3"

F2d = experiment.GetMeasurement("F2d")
if not F2d :
    F2d = NucDBMeasurement("F2d","F_{2}^{d}")
    experiment.AddMeasurement(F2d)
F2d.ClearDataPoints()
F2d.SetColor(1)

F2dExtractor = OLDSLACExtractor()
F2dExtractor.SetMeasurement(F2d)
F2dExtractor.SetInputFile("experiments/OLDSLAC/f2nucl.dat")
F2dExtractor.linestoskip=0
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
#F2dExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Xbjorken)
#F2dExtractor.fCurrentDataPoint.GetBinnedVariables().Add(Qsq)
F2dExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
F2dExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
F2dExtractor.Initialize()
F2dExtractor.ExtractAllValues()
F2d.BuildGraph()

#datapoint = NucDBDataPoint(1.0,0.2)
#Aperp.AddDataPoint(datapoint)
experiment.Print()

manager.SaveExperiment(experiment)



