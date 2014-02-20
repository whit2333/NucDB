from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from NucDBExtractors import *
import os

# For A1
class E93009Extractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.linestoskip=12
        self.iValueRow=6
        self.istatErr=7
        self.isysErr=9
        self.NumberOfLines=0
        self.linesRead=0
        self.iQsq=3
    
    def ParseLine(self):
        """ See input file for column structures
        """
        #print self.currentline
        ixbjorken=0
        values = self.currentline.split()
        if self.linesRead > self.NumberOfLines :
             self.rowcut.currentValue=1
             return
        if not self.is_number(values[ixbjorken]) :
             self.rowcut.currentValue=1
             return
        deltax=0.01
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

# for G1/F1
class E93009Extractor2(E93009Extractor):
    def __init__(self):
        E93009Extractor.__init__(self)
        self.iValueRow=11
        self.istatErr=12
        self.isysErr=14





def ExtractFromClasDat(lines,fname) :
    manager = NucDBManager.GetManager(1)

    experiment = manager.GetExperiment("CLAS-E93009")
    
    if not experiment :
        experiment = NucDBExperiment("CLAS-E93009","CLAS E93-009")
    dir = "experiments/CLAS/"
    datafilename = dir + fname
    datfile = open(datafilename,"r")

    line = datfile.readline()
    line = datfile.readline()
    line = datfile.readline()
    line = datfile.readline()

    # line 4: beam energy
    #print line
    values = line.split()
    beamenergy = float(values[2])
    #print values[2]
    Ebeam = NucDBBinnedVariable("E","E")
    Ebeam.SetBinValueSize(beamenergy,0.00001)
    #get target
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
    invmass = float(values[2]) 
    print invmass 
    W = NucDBBinnedVariable("W","W")
    W.SetBinValueSize(invmass,0.001)

    line = datfile.readline()

    if targ == "p" :
        A1p = experiment.GetMeasurement("A1p")
        if not A1p :
                A1p = NucDBMeasurement("A1p","A_{1}^{p}")
                experiment.AddMeasurement(A1p)
            #A1p.ClearDataPoints()
        A1p.SetColor(4018)
        extractor1 = E93009Extractor()
        extractor1.SetMeasurement(A1p)
        extractor1.SetInputFile(datafilename,11,int(lines)-12)
        #extractor1.linestoskip=0
        #extractor1.NumberOfLines=float(lines)-13
        Xbjorken = NucDBBinnedVariable("x","x")
        Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
        extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
        extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
        extractor1.fCurrentDataPoint.AddBinVariable(Ebeam)
        extractor1.fCurrentDataPoint.AddBinVariable(W)
        extractor1.Initialize()
        extractor1.ExtractAllValues()
        A1p.BuildGraph()

        g1pOverF1p = experiment.GetMeasurement("g1p/F1p")
        if not g1pOverF1p :
                g1pOverF1p = NucDBMeasurement("g1p/F1p","g_{1}^{p}/F_{1}^{p}")
                experiment.AddMeasurement(g1pOverF1p)
            #A1p.ClearDataPoints()
        g1pOverF1p.SetColor(4018)
        extractor2 = E93009Extractor2()
        extractor2.SetMeasurement(g1pOverF1p)
        extractor2.SetInputFile(datafilename,11,int(lines)-12)
        #extractor1.linestoskip=0
        #extractor2.NumberOfLines=float(lines)-13
        Xbjorken = NucDBBinnedVariable("x","x")
        Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
        extractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
        extractor2.fCurrentDataPoint.AddBinVariable(Qsq)
        extractor2.fCurrentDataPoint.AddBinVariable(Ebeam)
        extractor2.fCurrentDataPoint.AddBinVariable(W)
        extractor2.Initialize()
        extractor2.ExtractAllValues()
        g1pOverF1p.BuildGraph()

    if targ == "d" :
        A1d = experiment.GetMeasurement("A1d")
        if not A1d :
                A1d = NucDBMeasurement("A1d","A_{1}^{d}")
                experiment.AddMeasurement(A1d)
        #A1d.ClearDataPoints()
        A1d.SetColor(4018)
        extractor1 = E93009Extractor()
        extractor1.SetMeasurement(A1d)
        extractor1.SetInputFile(datafilename,10,int(lines)-11)
        #extractor1.linestoskip=0
        #extractor1.NumberOfLines=float(lines)-13
        Xbjorken = NucDBBinnedVariable("x","x")
        Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
        extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
        extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
        extractor1.fCurrentDataPoint.AddBinVariable(Ebeam)
        extractor1.fCurrentDataPoint.AddBinVariable(W)
        extractor1.Initialize()
        extractor1.ExtractAllValues()
        A1d.BuildGraph()

        g1dOverF1d = experiment.GetMeasurement("g1d/F1d")
        if not g1dOverF1d :
                g1dOverF1d = NucDBMeasurement("g1d/F1d","g_{1}^{d}/F_{1}^{d}")
                experiment.AddMeasurement(g1dOverF1d)
        #g1dOverF1d.ClearDataPoints()
        g1dOverF1d.SetColor(4018)
        extractor2 = E93009Extractor2()
        extractor2.SetMeasurement(g1dOverF1d)
        extractor2.SetInputFile(datafilename,10,int(lines)-11)
        #extracto2.linestoskip=0
        #extractor2.NumberOfLines=float(lines)-13
        Xbjorken = NucDBBinnedVariable("x","x")
        Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
        extractor2.fCurrentDataPoint.AddBinVariable(Xbjorken)
        extractor2.fCurrentDataPoint.AddBinVariable(Qsq)
        extractor2.fCurrentDataPoint.AddBinVariable(Ebeam)
        extractor2.fCurrentDataPoint.AddBinVariable(W)
        extractor2.Initialize()
        extractor2.ExtractAllValues()
        g1dOverF1d.BuildGraph()


    #experiment.Print()

    #experiment.Print()
    experiment.Print()
    manager.SaveExperiment(experiment)



    #for wd in values :
    #   print wd
    return


if __name__ == "__main__":

    # line count for each data file.
    # wc -l E93009/clasdata* > E93009_summary.txt 
    in_file = open("experiments/CLAS/E93009_summary.txt", "r")

    manager = NucDBManager.GetManager(1)
    experiment = manager.GetExperiment("CLAS-E93009")
    if experiment :
        experiment.GetMeasurements().Clear();
    
    for i in range(1,306) : 
        line = in_file.readline()
        #print line
        values = line.split()
    
        nlines = 0
        datfile = ""
        if values[0] :
                nlines = values[0]
                datfile = str(values[1])
        #print nlines
        print datfile 
        ExtractFromClasDat(nlines,datfile) 
   

    #anExp = experiment.GetMeasurement("A1p")
    #if not anExp :
    #    anExp = NucDBMeasurement("F2n","F_{2}^{n}")
    #    experiment.AddMeasurement(F2n)
    #F2n.ClearDataPoints()
    #F2n.SetColor(3)
    #extractor1 = CERNNA2Extractor()
    #extractor1.SetMeasurement(F2n)
    #extractor1.SetInputFile("experiments/CERN-NA-2/f2neutron.dat")
    #extractor1.linestoskip=0
    #extractor1.NumberOfLines=65
    #Xbjorken = NucDBBinnedVariable("x","x")
    #Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    #extractor1.fCurrentDataPoint.AddBinVariable(Xbjorken)
    #extractor1.fCurrentDataPoint.AddBinVariable(Qsq)
    #extractor1.Initialize()
    #extractor1.ExtractAllValues()
    #F2n.BuildGraph()
    

    #experiment.Print()
    #manager.SaveExperiment(experiment)



