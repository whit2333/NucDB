from ROOT import gROOT,gSystem
gSystem.Load( 'libNucDB' )
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDiscreteVariable
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
        iQsq=5-1
        values = self.currentline.split()
        self.rowcut.currentValue=int(values[0])
        x = self.fCurrentDataPoint.GetBinVariable('x')
        x.SetBinValueSize(float(values[ixbjorken]),0.05)
        Qsq = self.fCurrentDataPoint.GetBinVariable("Qsquared")
        Qsq.SetBinValueSize(float(values[iQsq]),0.5)
        self.fCurrentDataPoint.fStatisticalError.SetError(float(values[self.iValueRowErr]))
        self.fCurrentDataPoint.CalculateTotalError()
        self.fCurrentDataPoint.fValue=float(values[self.iValueRow])
        self.fCurrentDataPoint.Print()
        x.Print()
        Qsq.Print()

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


manager = NucDBManager.GetManager()

experiment = manager.GetExperiment("RSS")
if not experiment :
    experiment = NucDBExperiment("RSS","RSS")

Apara = experiment.GetMeasurement("Aparap")
if not Apara :
    Apara = NucDBMeasurement("Aparap","A_{\para}^{p}")
    experiment.AddMeasurement(Apara)
Apara.ClearDataPoints()
Apara.fColor=4

Aperp = experiment.GetMeasurement("Aperpp")
if not Aperp :
    Aperp = NucDBMeasurement("Aperpp","A_{\perp}^{p}")
    experiment.AddMeasurement(Aperp)
Aperp.ClearDataPoints()
Aperp.fColor=4

g1p = experiment.GetMeasurement("g1p")
if not g1p :
    g1p = NucDBMeasurement("g1p","g_{1}^{p}")
    experiment.AddMeasurement(g1p)
g1p.ClearDataPoints()
g1p.fColor=4

g2p = experiment.GetMeasurement("g2p")
if not g2p :
    g2p = NucDBMeasurement("g2p","g_{2}^{p}")
    experiment.AddMeasurement(g2p)
g2p.ClearDataPoints()
g2p.fColor=4

A1p = experiment.GetMeasurement("A1p")
if not A1p :
    A1p = NucDBMeasurement("A1p","A_{1}^{p}")
    experiment.AddMeasurement(A1p)
A1p.ClearDataPoints()
A1p.fColor=4

A2p = experiment.GetMeasurement("A2p")
if not A2p :
    A2p = NucDBMeasurement("A2p","A_{2}^{p}")
    experiment.AddMeasurement(A2p)
A2p.ClearDataPoints()
A2p.fColor=4


AparaExtractor = RSSExtractorApara()
AparaExtractor.SetMeasurement(Apara)
AparaExtractor.SetInputFile("experiments/RSS/rss_proton_b2.dat")
AparaExtractor.linestoskip=115
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
AparaExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
AparaExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
AparaExtractor.Initialize()
AparaExtractor.ExtractAllValues()
Apara.BuildGraph()

AperpExtractor = RSSExtractorAperp()
AperpExtractor.SetMeasurement(Aperp)
AperpExtractor.SetInputFile("experiments/RSS/rss_proton_b2.dat")
AperpExtractor.linestoskip=115
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
AperpExtractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
AperpExtractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
AperpExtractor.Initialize()
AperpExtractor.ExtractAllValues()
Aperp.BuildGraph()

g1Extractor = RSSExtractorg1p()
g1Extractor.SetMeasurement(g1p)
g1Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat")
g1Extractor.linestoskip=115
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g1Extractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g1Extractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g1Extractor.Initialize()
g1Extractor.ExtractAllValues()
g1p.BuildGraph()

g2Extractor = RSSExtractorg2p()
g2Extractor.SetMeasurement(g2p)
g2Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat")
g2Extractor.linestoskip=115
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
g2Extractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
g2Extractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
g2Extractor.Initialize()
g2Extractor.ExtractAllValues()
g2p.BuildGraph()

A2Extractor = RSSExtractorA2p()
A2Extractor.SetMeasurement(A2p)
A2Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat")
A2Extractor.linestoskip=115
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
A2Extractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
A2Extractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
A2Extractor.Initialize()
A2Extractor.ExtractAllValues()
A2p.BuildGraph()

A1Extractor = RSSExtractorA1p()
A1Extractor.SetMeasurement(A1p)
A1Extractor.SetInputFile("experiments/RSS/rss_proton_b2.dat")
A1Extractor.linestoskip=115
Xbjorken = NucDBBinnedVariable("x","x")
Qsq = NucDBBinnedVariable("Qsquared","Q^{2}")
A1Extractor.fCurrentDataPoint.fBinnedVariables.Add(Xbjorken)
A1Extractor.fCurrentDataPoint.fBinnedVariables.Add(Qsq)
A1Extractor.Initialize()
A1Extractor.ExtractAllValues()
A1p.BuildGraph()


experiment.Print()
manager.SaveExperiment(experiment)



