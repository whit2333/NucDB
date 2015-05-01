from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBEnergyMeV,NucDBPaper,NucDBCalculation,NucDBReference
from NucDBExtractors import *
import os
import getopt

if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    paper = manager.GetPaper("Gockeler2000ja")
    if not paper :
        paper = NucDBPaper("Gockeler2000ja","A Lattice calculation of the nucleon's spin dependent structure function g(2) revisited")
    paper.SetAuthor("Gockeler, et.al.")

    ref = NucDBReference("Gockeler2000ja","A Lattice calculation of the nucleon's spin dependent structure function g(2) revisited")
    ref.SetURL("http://inspirehep.net/record/537676")
    ref.SetDOI("http://dx.doi.org/10.1103/PhysRevD.63.074506")


    # d2p - From Phys.Rev.D 63 074506, page 8

    d2Calc = paper.GetCalculation('d2')
    if not d2Calc :
        d2Calc = NucDBCalculation('d2','d_{2}')
        paper.AddCalculation(d2Calc)

    d2p = d2Calc.GetMeasurement('d2p')
    if not d2p : 
	    d2p = NucDBMeasurement('d2p','d_{2}^{p}')
	    d2Calc.AddMeasurement(d2p)
    
    d2p.ClearDataPoints()
    d2p.fColor=4
    d2p.AddRef(ref)
    d2p.SetExperimentName( paper.GetAuthor() )


    aDataPoint = NucDBDataPoint()

    # Q^2 = \mu^2 = 5.0 GeV^2  From table 5
    Qsquared = NucDBBinnedVariable("Qsquared","Q^{2}")
    aDataPoint.AddBinVariable(Qsquared)
    beta     = NucDBBinnedVariable("beta","#beta")
    aDataPoint.AddBinVariable(beta)

# best lattice spacing for beta=6.4

    # 1
    Qsquared.SetBinValueSize(5.0,1.0)
    beta.SetBinValueSize(6.0,0.1)
    aDataPoint.SetValue(0.008)
    aDataPoint.GetStatError().SetError(0.004)
    aDataPoint.CalculateTotalError();
    #d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 
    # ^ makes a deep copy of everythin so you can move on to the next point using the same references above

    #2 
    Qsquared.SetBinValueSize(5.0,1.0)
    beta.SetBinValueSize(6.2,0.1)
    aDataPoint.SetValue(0.017)
    aDataPoint.GetStatError().SetError(0.009)
    aDataPoint.CalculateTotalError();
    #d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #3 
    Qsquared.SetBinValueSize(5.0,1.0)
    beta.SetBinValueSize(6.4,0.1)
    aDataPoint.SetValue(0.017)
    aDataPoint.GetStatError().SetError(0.007)
    aDataPoint.CalculateTotalError();
    #d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #4 Table 6. \mu^2 = Q^2 = 10.0 GeV^2 
    Qsquared.SetBinValueSize(10.0,1.0)
    beta.SetBinValueSize(6.0,0.1)
    aDataPoint.SetValue(0.002)
    aDataPoint.GetStatError().SetError(0.004)
    aDataPoint.CalculateTotalError();
    #d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #5 Table 6. \mu^2 = Q^2 = 10.0 GeV^2 
    Qsquared.SetBinValueSize(10.0,1.0)
    beta.SetBinValueSize(6.2,0.1)
    aDataPoint.SetValue(0.006)
    aDataPoint.GetStatError().SetError(0.007)
    aDataPoint.CalculateTotalError();
    #d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #6 Table 6. \mu^2 = Q^2 = 10.0 GeV^2 
    Qsquared.SetBinValueSize(10.0,1.0)
    beta.SetBinValueSize(6.4,0.1)
    aDataPoint.SetValue(0.008)
    aDataPoint.GetStatError().SetError(0.006)
    aDataPoint.CalculateTotalError();
    #d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #7 Actually from Phys Rev D 72, 054507 (2005)
    Qsquared.SetBinValueSize(5.1,1.0)
    beta.SetBinValueSize(5.20,0.1)
    aDataPoint.SetValue(0.004)
    aDataPoint.GetStatError().SetError(0.005)
    aDataPoint.CalculateTotalError();
    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    ### here is where we set the values of the data points
    d2p.BuildGraph("Qsquared")
    paper.Print()
    manager.SavePaper(paper)

    # d2p from SLAC E155x
    #
    #  ...  We obtained values of d p 2 =0.0025 +-0.0016 +-0.0010 and d d 2 =0.0054 +-0.0023 +-0.0005 at an average Q 2 of 5 GeV 2 .
    #  We combined these results with those from SLAC experiments on the neutron (E142 [7] and E154 [15]) and proton
    #  and deuteron (E143 [6] and E155 [14]) to obtained average values d p 2 =
    e155 = manager.GetExperiment("SLAC E155x")
    if not e155 :
        e155 = NucDBExperiment("SLAC E155x","SLAC E155x")
   
    d2p_155 = e155.GetMeasurement('d2p')
    if not d2p_155 :
        d2p_155 = NucDBMeasurement('d2p','d_{2}^{p}')
        e155.AddMeasurement(d2p_155)
    d2p_155.ClearDataPoints()
    d2p_155.fColor=2
    d2p_155.SetExperimentName("SLAC");

    baDataPoint = NucDBDataPoint()
    baDataPoint.AddBinVariable(Qsquared)

    Qsquared.SetBinValueSize(5.0,1.0)
    baDataPoint.SetValue(0.0025)
    baDataPoint.GetStatError().SetError(0.0016)
    baDataPoint.GetSystError().SetError(0.0010)
    baDataPoint.CalculateTotalError();

    d2p_155.AddDataPoint(copy.deepcopy( baDataPoint )) 

    e155.Print()
    manager.SaveExperiment(e155)


