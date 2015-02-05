from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable,NucDBInvariantMassDV,NucDBPhotonEnergyDV
from ROOT import TCanvas,TGraphErrors
from NucDBExtractors import *
import os

class RSSExtractor(NucDBRawDataExtractor):
    def __init__(self):
        NucDBRawDataExtractor.__init__(self)
        self.rowcut.nominalValue=2
        self.iValueRow=19-1
        self.iValueRowErr=20-1

    def ParseLine(self):
        """ See input file for column structures
        """
        ixbjorken=5-1
        iQsq=4-1
        iW=3-1
        values = self.currentline.split()
        self.rowcut.currentValue=int(values[0])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.001)
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        W = self.fCurrentDataPoint.GetBinVariable('W')
        W.SetBinValueSize(float(values[iW])/1000.0,0.001)
        self.fCurrentDataPoint.GetStatError().SetError(float(values[self.iValueRowErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.SetValue(float(values[self.iValueRow]))
        #
        #W = self.fCurrentDataPoint.GetDependentVariable("W")
        #if not W :
        #    W   = NucDBInvariantMassDV()
        #    self.fCurrentDataPoint.AddDependentVariable(W)
        #if W :
        #    W.SetVariable(0,x)
        #    W.SetVariable(1,Qsq)
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
        #x.Print()
        #Qsq.Print()

class RSSExtractorg2p(RSSExtractor):
    def __init__(self):
        RSSExtractor.__init__(self)
        self.rowcut.nominalValue=2
        self.iValueRow=19-1
        self.iValueRowErr=20-1


class RSSExtractorg1p(RSSExtractor):
    def __init__(self):
        RSSExtractor.__init__(self)
        self.rowcut.nominalValue=2
        self.iValueRow=17-1
        self.iValueRowErr=18-1

class RSSExtractorApara(RSSExtractor):
    def __init__(self):
        RSSExtractor.__init__(self)
        self.rowcut.nominalValue=2
        self.iValueRow=9-1
        self.iValueRowErr=10-1

class RSSExtractorAperp(RSSExtractor):
    def __init__(self):
        RSSExtractor.__init__(self)
        self.rowcut.nominalValue=2
        self.iValueRow=11-1
        self.iValueRowErr=12-1


class RSSExtractorA2p(RSSExtractor):
    def __init__(self):
        RSSExtractor.__init__(self)
        self.rowcut.nominalValue=2
        self.iValueRow=23-1
        self.iValueRowErr=24-1


class RSSExtractorA1p(RSSExtractor):
    def __init__(self):
        RSSExtractor.__init__(self)
        self.rowcut.nominalValue=2
        self.iValueRow=21-1
        self.iValueRowErr=22-1


if __name__ == "__main__":
    manager = NucDBManager.GetManager(1)
    
    experiment = manager.GetExperiment("RSS")
    if not experiment :
        experiment = NucDBExperiment("RSS","RSS")
    
    Apara = experiment.GetMeasurement("Aparap")
    if not Apara :
        Apara = NucDBMeasurement("Aparap","A_{\para}^{p}")
        experiment.AddMeasurement(Apara)
    Apara.ClearDataPoints()
    Apara.SetColor(4010)
    
    Aperp = experiment.GetMeasurement("Aperpp")
    if not Aperp :
        Aperp = NucDBMeasurement("Aperpp","A_{\perp}^{p}")
        experiment.AddMeasurement(Aperp)
    Aperp.ClearDataPoints()
    Aperp.SetColor(4010)
    
    g1p = experiment.GetMeasurement("g1p")
    if not g1p :
        g1p = NucDBMeasurement("g1p","g_{1}^{p}")
        experiment.AddMeasurement(g1p)
    g1p.ClearDataPoints()
    g1p.SetColor(4010)
    
    g2p = experiment.GetMeasurement("g2p")
    if not g2p :
        g2p = NucDBMeasurement("g2p","g_{2}^{p}")
        experiment.AddMeasurement(g2p)
    g2p.ClearDataPoints()
    g2p.SetColor(4010)
    
    A1p = experiment.GetMeasurement("A1p")
    if not A1p :
        A1p = NucDBMeasurement("A1p","A_{1}^{p}")
        experiment.AddMeasurement(A1p)
    A1p.ClearDataPoints()
    A1p.SetColor(4010)
    
    A2p = experiment.GetMeasurement("A2p")
    if not A2p :
        A2p = NucDBMeasurement("A2p","A_{2}^{p}")
        experiment.AddMeasurement(A2p)
    A2p.ClearDataPoints()
    A2p.SetColor(4010)
    
    
    AparaExtractor = RSSExtractorApara()
    AparaExtractor.SetMeasurement(Apara)
    AparaExtractor.SetInputFile("experiments/RSS/rss_proton_b2.dat",114)
    #AparaExtractor.linestoskip=115
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    AparaExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    AparaExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    W = NucDBBinnedVariable("W","W")
    AparaExtractor.fCurrentDataPoint.AddBinVariable(W)
    AparaExtractor.Initialize()
    AparaExtractor.ExtractAllValues()
    Apara.BuildGraph()
    
    AperpExtractor = RSSExtractorAperp()
    AperpExtractor.SetMeasurement(Aperp)
    AperpExtractor.SetInputFile("experiments/RSS/rss_proton_b2.dat",114)
    #AperpExtractor.linestoskip=115
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    AperpExtractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    AperpExtractor.fCurrentDataPoint.AddBinVariable(Qsq)
    W = NucDBBinnedVariable("W","W")
    AperpExtractor.fCurrentDataPoint.AddBinVariable(W)
    AperpExtractor.Initialize()
    AperpExtractor.ExtractAllValues()
    Aperp.BuildGraph()
    
    g1Extractor = RSSExtractorg1p()
    g1Extractor.SetMeasurement(g1p)
    g1Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat",114)
    #g1Extractor.linestoskip=115
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    g1Extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g1Extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    W = NucDBBinnedVariable("W","W")
    g1Extractor.fCurrentDataPoint.AddBinVariable(W)
    g1Extractor.Initialize()
    g1Extractor.ExtractAllValues()
    g1p.BuildGraph()
    
    g2Extractor = RSSExtractorg2p()
    g2Extractor.SetMeasurement(g2p)
    g2Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat",114)
    #g2Extractor.linestoskip=115
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    W = NucDBBinnedVariable("W","W")
    g2Extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    g2Extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    g2Extractor.fCurrentDataPoint.AddBinVariable(W)
    g2Extractor.Initialize()
    g2Extractor.ExtractAllValues()
    g2p.BuildGraph()
    g2p.Print("data");
    
    A2Extractor = RSSExtractorA2p()
    A2Extractor.SetMeasurement(A2p)
    A2Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat",114)
    #A2Extractor.linestoskip=115
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    W = NucDBBinnedVariable("W","W")
    A2Extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A2Extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    A2Extractor.fCurrentDataPoint.AddBinVariable(W)
    A2Extractor.Initialize()
    A2Extractor.ExtractAllValues()
    A2p.BuildGraph()

    #A2p.PrintBreakDown("x");
    
    A1Extractor = RSSExtractorA1p()
    A1Extractor.SetMeasurement(A1p)
    A1Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat",114)
    #A1Extractor.linestoskip=115
    Xbjorken = NucDBBinnedVariable("x","x")
    Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
    W = NucDBBinnedVariable("W","W")
    A1Extractor.fCurrentDataPoint.AddBinVariable(Xbjorken)
    A1Extractor.fCurrentDataPoint.AddBinVariable(Qsq)
    A1Extractor.fCurrentDataPoint.AddBinVariable(W)
    A1Extractor.Initialize()
    A1Extractor.ExtractAllValues()
    A1p.BuildGraph()
    
    # from nucl-ex/0608003
    d2p = experiment.GetMeasurement("d2p")
    if not d2p :
        d2p = NucDBMeasurement("d2p","d_{2}^{p}")
        experiment.AddMeasurement(d2p)
    d2p.ClearDataPoints()
    d2p.SetColor(4010)
    
    Qsquared = NucDBBinnedVariable("Qsquared","Q^{2}")
    
    d2p_data = NucDBDataPoint();
    d2p_data.AddBinVariable(Qsquared)
    
    Qsquared.SetBinValueSize(1.3,1.0)
    d2p_data.SetValue(0.0057)
    d2p_data.GetSystError().SetError(0.0007)
    d2p_data.GetStatError().SetError(0.0009)  
    d2p_data.CalculateTotalError();
    
    d2p.AddDataPoint(copy.deepcopy( d2p_data )) 
    
    d2p.BuildGraph("Qsquared")
    
    experiment.Print()
    manager.SaveExperiment(experiment)
    
    
    
