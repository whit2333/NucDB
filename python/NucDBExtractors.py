from ROOT import gROOT,gSystem
gSystem.Load( 'libTreeViewer' )
gSystem.Load( 'libNucDB' )

#import numpy as np
import sys
import time
import datetime
#import MySQLdb
import math
import os
from ROOT import NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDataPoint,NucDBBinnedVariable,NucDBDiscreteVariable
import copy

""" @package NucDBRawDataExtractor
    Used to extract experimental results from a raw data text file

    Implementation is specific to raw text file structure.
"""

class RowCut:
    """ Simple cut on a variable for a row in columnwise textfile
    """

    def __init__(self):
        self.columnNumber=0
        self.nominalValue=0
        self.nominalRange=0
        self.currentValue=0
    
    def IsGoodRow(self):
        if self.currentValue == self.nominalValue :
           return 1
        else :
           return 0

class NucDBRawDataExtractor:
    """ ABC used to extract experimental results from a raw data text file
        Implementation is specific to raw text file structure.
    """
    def __init__(self):
        self.linestoskip=0
        self.currentline=0
        self.inputfile=0
        self.inputfilename=0
        self.fMeasurement=0
        self.rowcut = RowCut()
        self.fCurrentDataPoint = NucDBDataPoint()
        self.fCurrentDataPoint.SetName("A")
        #self.fCurrentDataPoint.fDimension=2

    def ParseLine(self):
        """ Specific to file structure. Should be reimplemented/modified for every measurment
        """
        values = self.currentline.split()
        #for i in range(9):
            #print str(i) + " = " + str(values[i])

    def SetInputFile(self,filename):
        self.inputfilename = filename
        self.inputfile = open(self.inputfilename, "r")

    def SetMeasurement(self, meas):
        self.fMeasurement = meas

    def Initialize(self):
        skip = self.linestoskip -1
        for i in range(skip):
            self.currentline = self.inputfile.readline() # keep it one line ahead of searchfile
            #print self.currentline
        print "done"

    def GetNextLine(self):
        self.currentline = self.inputfile.readline()

    def ExtractAllValues(self):
        for self.currentline in self.inputfile:
            self.GetNextValue()

    def GetNextValue(self):
        if not self.fMeasurement :
            return
        self.ParseLine()
        if self.rowcut.IsGoodRow():
            self.fCurrentDataPoint.SetName("p"+str(self.fMeasurement.GetNDataPoints()));
            self.fMeasurement.AddDataPoint(copy.deepcopy(self.fCurrentDataPoint))
