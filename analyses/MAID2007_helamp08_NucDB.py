from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBEnergyMeV
from NucDBExtractors import *
import os
import getopt

class MAID2007Extractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iQsq=0
        self.iValueRow=1
        self.iStatErr=2
        self.skipTheRest = False

    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        deltax=0.01
        if self.skipTheRest :
            self.rowcut.currentValue=int(1) # data will not be used
            return
        if len(values) == 0 :
            self.skipTheRest = True
            self.rowcut.currentValue=int(1) # data will not be used
            return
        else: 
            self.rowcut.currentValue=int(0) # data will be used
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[self.iQsq]),0.01)
        Qsq.Print()
        
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.iStatErr]))
        self.fCurrentDataPoint.fSystematicError.SetError(float(0.0))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.Print()


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)
    file=' '
#    file = 'analyses/MAID2007/data/mult1p/Maid2007_E0p_1p.dat'
#    meas = 'E0+(p,1/2)'
    meas = ' ' 
    mtitle = meas
    options, remainder = getopt.getopt(sys.argv[1:], 'f:m:t:',
		       ['file=','meas=','title='])
    for opt, arg in options:
        if opt in ('-f', '--file'):
            file = str(arg)
        elif opt in ('-m','--meas'):
            meas = str(arg)  
        elif opt in ('-t','--title'):
            mtitle = str(arg)  
#        else: 
#	    file = 'analyses/MAID2007/data/mult1p/Maid2007_E0p_1p.dat'
#        elif opt in ('-v', '--verbose'):
#             verbose = True

    experiment = manager.GetExperiment("MAID2007")
    if not experiment :
        experiment = NucDBExperiment("MAID2007","MAID2007")

    #proton Im and Re parts of E0(proton,I=1/2) 
    A12 = experiment.GetMeasurement(str('Ap(1/2)')+meas)
    if not A12 :
        A12 = NucDBMeasurement(str('Ap(1/2)')+meas,str('Ap(1/2)')+meas)
        experiment.AddMeasurement(A12)
    A12.ClearDataPoints()
    A12.fColor=1
    extractor1 = MAID2007Extractor()
    extractor1.iValueRow=1
    extractor1.iStatErr=2
    extractor1.SetMeasurement(A12)
    extractor1.SetInputFile(file)
    extractor1.linestoskip=6
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    #energyunit = NucDBEnergyMeV()
    #energy.SetUnit(energyunit)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    A12.BuildGraph("Qsquared")
    
    A32 = experiment.GetMeasurement(str('Ap(3/2)')+meas)
    if not A32 :
        A32 = NucDBMeasurement(str('Ap(3/2)')+meas,str('Ap(3/2)')+meas)
        experiment.AddMeasurement(A32)
    A32.ClearDataPoints()
    A32.fColor=1
    extractor2 = MAID2007Extractor()
    extractor2.iValueRow=3
    extractor2.iStatErr=4
    extractor2.SetMeasurement(A32)
    extractor2.SetInputFile(file)
    extractor2.linestoskip=6
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    #energyunit = NucDBEnergyMeV()
    #energy.SetUnit(energyunit)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    A32.BuildGraph("Qsquared")
    
    S12 = experiment.GetMeasurement(str('Sp(1/2)')+meas)
    if not S12 :
        S12 = NucDBMeasurement(str('Sp(1/2)')+meas,str('Sp(1/2)')+meas)
        experiment.AddMeasurement(S12)
    S12.ClearDataPoints()
    S12.fColor=1
    extractor3 = MAID2007Extractor()
    extractor3.iValueRow=5
    extractor3.iStatErr=6
    extractor3.SetMeasurement(S12)
    extractor3.SetInputFile(file)
    extractor3.linestoskip=6
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    extractor3.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
    #energyunit = NucDBEnergyMeV()
    #energy.SetUnit(energyunit)
    extractor3.Initialize()
    extractor3.ExtractAllValues()
    S12.BuildGraph("Qsquared")

    experiment.Print()
    manager.SaveExperiment(experiment)



