from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBEnergyMeV,NucDBPaper,NucDBCalculation
from NucDBExtractors import *
import os
import getopt

if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)

    paper = manager.GetPaper("LATTICE2001")
    if not paper :
        paper = NucDBPaper("LATTICE2001","LATTICE2001")

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

    aDataPoint = NucDBDataPoint()

    # Q^2 = \mu^2 = 5.0 GeV^2  From table 5
    Qsquared = NucDBBinnedVariable("Qsquared","Q^{2}")
    aDataPoint.AddBinVariable(Qsquared)
    beta     = NucDBBinnedVariable("beta","#beta")
    aDataPoint.AddBinVariable(beta)


    # 1
    Qsquared.SetBinValueSize(5.0,1.0)
    beta.SetBinValueSize(6.0,0.1)
    aDataPoint.SetValue(0.008)
    aDataPoint.GetSystError().SetError(0.004)
    aDataPoint.GetStatError().SetError(0.0) # No stat error. 
    aDataPoint.CalculateTotalError();

    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 
    # ^ makes a deep copy of everythin so you can move on to the next point using the same references above

    #2 
    Qsquared.SetBinValueSize(5.0,1.0)
    beta.SetBinValueSize(6.2,0.1)
    aDataPoint.SetValue(0.017)
    aDataPoint.GetSystError().SetError(0.009)
    aDataPoint.GetStatError().SetError(0.0) # No stat error. 
    aDataPoint.CalculateTotalError();

    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #3 
    Qsquared.SetBinValueSize(5.0,1.0)
    beta.SetBinValueSize(6.4,0.1)
    aDataPoint.SetValue(0.017)
    aDataPoint.GetSystError().SetError(0.007)
    aDataPoint.GetStatError().SetError(0.0) # No stat error. 
    aDataPoint.CalculateTotalError();

    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #4 Table 6. \mu^2 = Q^2 = 10.0 GeV^2 
    Qsquared.SetBinValueSize(10.0,1.0)
    beta.SetBinValueSize(6.0,0.1)
    aDataPoint.SetValue(0.002)
    aDataPoint.GetSystError().SetError(0.004)
    aDataPoint.GetStatError().SetError(0.0) # No stat error. 
    aDataPoint.CalculateTotalError();

    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #5 Table 6. \mu^2 = Q^2 = 10.0 GeV^2 
    Qsquared.SetBinValueSize(10.0,1.0)
    beta.SetBinValueSize(6.2,0.1)
    aDataPoint.SetValue(0.006)
    aDataPoint.GetSystError().SetError(0.007)
    aDataPoint.GetStatError().SetError(0.0) # No stat error. 
    aDataPoint.CalculateTotalError();

    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #6 Table 6. \mu^2 = Q^2 = 10.0 GeV^2 
    Qsquared.SetBinValueSize(10.0,1.0)
    beta.SetBinValueSize(6.4,0.1)
    aDataPoint.SetValue(0.008)
    aDataPoint.GetSystError().SetError(0.006)
    aDataPoint.GetStatError().SetError(0.0) # No stat error. 
    aDataPoint.CalculateTotalError();

    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    #7 Actually from Phys Rev D 72, 054507 (2005)
    Qsquared.SetBinValueSize(5.1,1.0)
    beta.SetBinValueSize(5.20,0.1)
    aDataPoint.SetValue(0.004)
    aDataPoint.GetSystError().SetError(0.005)
    aDataPoint.GetStatError().SetError(0.0) # No stat error. 
    aDataPoint.CalculateTotalError();

    d2p.AddDataPoint(copy.deepcopy( aDataPoint )) 

    ### here is where we set the values of the data points
    d2p.BuildGraph("Qsquared")
    paper.Print()
    manager.SavePaper(paper)

    # d2p from SLAC E155x
    e155 = manager.GetExperiment("SLAC_E155")
    if not e155 :
        e155 = NucDBExperiment("SLAC_E155","SLAC_E155")
   
    d2p_155 = e155.GetMeasurement('d2p')
    if not d2p_155 :
        d2p_155 = NucDBMeasurement('d2p','d_{2}^{p}')
        e155.AddMeasurement(d2p_155)
    d2p_155.ClearDataPoints()
    d2p_155.fColor=2

    baDataPoint = NucDBDataPoint()
    baDataPoint.AddBinVariable(Qsquared)

    Qsquared.SetBinValueSize(5.0,1.0)
    baDataPoint.SetValue(0.0085)
    baDataPoint.GetSystError().SetError(0.0035)
    baDataPoint.GetStatError().SetError(0.0) # No stat error. 
    baDataPoint.CalculateTotalError();

    d2p_155.AddDataPoint(copy.deepcopy( baDataPoint )) 

    d2p_155.BuildGraph("Qsquared")
    e155.Print()
    manager.SaveExperiment(e155)


