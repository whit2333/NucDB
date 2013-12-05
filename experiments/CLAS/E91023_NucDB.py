from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

# For A1
class E91023Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.iValueRow = 3
        self.iW        = 0
        self.istatErr  = 4
        self.isysErr   = 6
        self.fQsqMin   = 0.0
        self.fQsqMax   = 1.0
    
    def ParseLine(self):
        """ See input file for column structures
        """
        print self.currentline
        values = self.currentline.split()
        deltaW=0.001
        W = self.fCurrentDataPoint.GetBinVariable('W')
        W.SetBinValueSize(float(values[self.iW]),deltaW)
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinMinimum(self.fQsqMin)
        Qsq.SetBinMaximum(self.fQsqMax)
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.istatErr].lstrip('+')))
        self.fCurrentDataPoint.GetSystError().SetError(float(values[self.isysErr].lstrip('+')))
        self.fCurrentDataPoint.CalculateTotalError()
        # x(W,Q2)
        x = self.fCurrentDataPoint.GetDependentVariable("W")
        if not x :
            x   = NucDBInvariantMassDV()
            self.fCurrentDataPoint.AddDependentVariable(x)
        if x :
            x.SetVariable(0,W)
            x.SetVariable(1,Qsq)
        # nu(x,Q2)
        nu = self.fCurrentDataPoint.GetDependentVariable("nu")
        if not nu :
            nu   = NucDBPhotonEnergyDV()
            self.fCurrentDataPoint.AddDependentVariable(nu)
        if nu :
            nu.SetVariable(0,x)
            nu.SetVariable(1,Qsq)
        self.fCurrentDataPoint.CalculateDependentVariables()
        self.fCurrentDataPoint.Print()
        #self.linesRead+=1

# ---------------------------------------------------------------
def ExtractFromClasDat(lines,fname) :
    manager    = NucDBManager.GetManager(1)
    experiment = manager.GetExperiment("CLAS-E91023")
    if not experiment :
        experiment = NucDBExperiment("CLAS-E91023","CLAS E91-023")
    dir          = "experiments/CLAS/"
    datafilename = dir + fname
    datfile      = open(datafilename,"r")

    
    line = datfile.readline()
    line = datfile.readline()
    line = datfile.readline()


    # line 4: beam energy
    line = datfile.readline()
    #print line
    values = line.split()
    beamenergy = float(values[2])
    #print values[2]
    Ebeam = NucDBBinnedVariable("E","E")
    Ebeam.SetBinValueSize(beamenergy,0.00001)

    # line 5: Q2 range 
    line = datfile.readline()
    #print line
    values = line.split()
    Q2min = float(values[2])
    Q2max = float(values[4]) 

    # line 6: reaction -> get target
    line = datfile.readline()
    values = line.split()
    targ = ""
    if values[3] == "P" :
        targ = "p"
    if values[3] == "DEUT" :
        targ = "d"
    print targ
    line = datfile.readline()
    values = line.split()

    line = datfile.readline()

    if targ == "p" :
        meas = experiment.GetMeasurement("AparapOverD")
        if not meas :
                meas = NucDBMeasurement("AparaOverD","A_{para}^{p}/D")
                experiment.AddMeasurement(meas)
            #A1p.ClearDataPoints()
        meas.SetColor(4001)
        extractor1 = E91023Extractor()
        extractor1.SetMeasurement(meas)
        extractor1.SetInputFile(datafilename,9,int(lines)-10)
        extractor1.fQsqMin = Q2min
        extractor1.fQsqMax = Q2max
        W = NucDBBinnedVariable("W","W")
        Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
        extractor1.fCurrentDataPoint.AddBinVariable(W)
        extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
        extractor1.fCurrentDataPoint.AddBinVariable(Ebeam)
        extractor1.Initialize()
        extractor1.ExtractAllValues()
        meas.BuildGraph("W")

    if targ == "d" :
        meas = experiment.GetMeasurement("AparadOverD")
        if not meas :
                meas = NucDBMeasurement("AparadOverD","A_{para}^{d}/D")
                experiment.AddMeasurement(meas)
            #A1p.ClearDataPoints()
        meas.SetColor(4001)
        extractor1 = E91023Extractor()
        extractor1.SetMeasurement(meas)
        extractor1.SetInputFile(datafilename,9,int(lines)-10)
        extractor1.fQsqMin = Q2min
        extractor1.fQsqMax = Q2max
        W = NucDBBinnedVariable("W","W")
        Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
        extractor1.fCurrentDataPoint.AddBinVariable(W)
        extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
        extractor1.fCurrentDataPoint.AddBinVariable(Ebeam)
        extractor1.Initialize()
        extractor1.ExtractAllValues()
        meas.BuildGraph("W")
    return


if __name__ == "__main__":
    """ Tables 1 and 2 have A1+ETA*A2 = Apara/D. 
        Talbes 3, 4, 5, 6, and 7 have g1 for various Q2 bins.
        Table 8 has g1 for all Q2. 
    """
    # line count for each data file.
    # wc -l E91023/clasdata* > E91023_summary.txt 
    in_file = open("experiments/CLAS/E91023_summary.txt", "r")

    manager = NucDBManager.GetManager(1)
    experiment = manager.GetExperiment("CLAS-E91023")
    if experiment :
        experiment.GetMeasurements().Clear();
    if not experiment :
        experiment = NucDBExperiment("CLAS-E91023","CLAS E91-023")
    
    for i in range(1,2) : 
        line = in_file.readline()
        values = line.split()
        nlines = 0
        datfile = ""
        if values[0] :
                nlines = values[0]
                datfile = str(values[1])
        print nlines
        print datfile 
        ExtractFromClasDat(nlines,datfile) 

    # todo: extract g1 tables

    experiment.Print()
    manager.SaveExperiment(experiment)




