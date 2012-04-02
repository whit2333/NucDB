from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBEnergyMeV
from NucDBExtractors import *
import os
import getopt

class MAID2007Extractor(NucDBRawDataExtractor) :
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iEnergy=0
        self.iValueRow=1
        self.iStatErr=2

    def ParseLine(self):
        """ See input file for column structures
        """
        values = self.currentline.split()
        deltax=0.01
        if values[self.iValueRow]=="-" :
            self.rowcut.currentValue=int(1) # data will not be used
            return
        else: 
            self.rowcut.currentValue=int(0) # data will be used
        
	energy = self.fCurrentDataPoint.GetBinVariable('energy')
        if energy : 
            energy.SetBinValueSize(float(values[self.iEnergy]),0.1)
            energy.Print()
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
    ImE0p = experiment.GetMeasurement(str('Im_')+meas)
    if not ImE0p :
        ImE0p = NucDBMeasurement(str('Im_')+meas,str('Im(')+meas+str(')'))
        experiment.AddMeasurement(ImE0p)
    ImE0p.ClearDataPoints()
    ImE0p.fColor=8
    extractor1 = MAID2007Extractor()
    extractor1.iValueRow=3
    extractor1.iStatErr=4
    extractor1.SetMeasurement(ImE0p)
#    extractor1.SetInputFile("analyses/MAID2007/data/mult1p/Maid2007_E0p_1p.dat")
    extractor1.SetInputFile(file)
    extractor1.linestoskip=4
    energy = NucDBBinnedVariable("energy","E_{#gamma}")
    extractor1.fCurrentDataPoint.fBinnedVariables.Add(energy)
    energyunit = NucDBEnergyMeV()
    energy.SetUnit(energyunit)
    extractor1.Initialize()
    extractor1.ExtractAllValues()
    ImE0p.BuildGraph("energy")
    
        #proton Im and Re parts of E0(proton,I=1/2) 
    ReE0p = experiment.GetMeasurement(str('Re_')+meas)
    if not ReE0p :
	    #        ReE0p = NucDBMeasurement("Re_E0+(p,1/2)","Re(E0+(proton,I=1/2))")
        ReE0p = NucDBMeasurement(str('Re_')+meas,str('Re(')+meas+str(')'))
        experiment.AddMeasurement(ReE0p)
    ReE0p.ClearDataPoints()
    ReE0p.fColor=1
    extractor2 = MAID2007Extractor()
    extractor2.iValueRow=1
    extractor2.iStatErr=2
    extractor2.SetMeasurement(ReE0p)
#    extractor2.SetInputFile("analyses/MAID2007/data/mult1p/Maid2007_E0p_1p.dat")
    extractor2.SetInputFile(file)
    extractor2.linestoskip=4
    energy = NucDBBinnedVariable("energy","E_{#gamma}")
    extractor2.fCurrentDataPoint.fBinnedVariables.Add(energy)
    #energyunit = NucDBEnergyMeV()
    #energy.SetUnit(energyunit)
    extractor2.Initialize()
    extractor2.ExtractAllValues()
    ReE0p.BuildGraph("energy")

    experiment.Print()
    manager.SaveExperiment(experiment)



