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
    Used to extract experimental results from a raw data text file.

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
    """ ABC for extracting experimental results from data text files.
        Implementation is specific to raw text file structure.
        ::ParseLine needs to be implemented for each measurement.
        The order of things is: 
        ::__init_
        ::SetMeasurement
        ::SetInputFile
        ::Initialize
        ::ExtractAllValues
    """
    def __init__(self):
        self._skip_line_       = 0
        self.linestoskip       = 0
        self.currentline       = 0
        self._line_number      = 0
        self._lines_read       = 0
        self.inputfile         = 0
        self.inputfilename     = 0
        self.fMeasurement      = 0
        self.NumberOfLines     = 0
        self.rowcut            = RowCut()
        self.fCurrentDataPoint = NucDBDataPoint()
        self.fCurrentDataPoint.SetName("A")
        #self.fCurrentDataPoint.fDimension=2

    def ParseLine(self):
        """ Specific to file structure. Should be reimplemented/modified for every extractor.""" 
        values = self.currentline.split()
        #for i in range(9):
            #print str(i) + " = " + str(values[i])

    def SetInputFile(self,filename,nskip=0,nread=0):
        """Set the input file name. 
           Optionally the number of header lines to skip and/or total number to read.
        """
        self.inputfilename = filename
        self.linestoskip   = nskip
        self.NInputLines   = nread
        self.NumberOfLines = self.FileLen(filename)
        if not nread:
            self.NInputLines = self.NumberOfLines - nskip
        self.inputfile     = open(self.inputfilename, "r")

    def FileLen(self,fname):
        with open(fname) as f:
            for i, l in enumerate(f):
                pass
            return i + 1

    def is_number(self,s):
        try:
            float(s)
            return True
        except ValueError:
            return False
         
    def SetMeasurement(self, meas):
        self.fMeasurement = meas

    def Initialize(self):
        "  "
        skip = self.linestoskip 
        for i in range(skip):
            self.currentline = self.inputfile.readline() # keep it one line ahead of searchfile
            self._line_number     = self._line_number + 1
            #print self.currentline
        #print "Initialization Complete"
        #print "NInputLines: " + str(self.NInputLines)
        #print "linestoskip: " + str(self.linestoskip)

    def GetNextLine(self):
        self._skip_line  = 0
        self.currentline = self.inputfile.readline()
        self._line_number     = self._line_number + 1
        self._lines_read      = self._lines_read + 1
        if self.currentline.__len__() < 2 :
            self._skip_line = 1

    def GetNextValue(self):
        if not self.fMeasurement :
            return
        if self._lines_read > self.NInputLines : 
            return
        if self._skip_line : 
            print " Bad line: " + self.currentline
            return
        #print "lines_read: " + str(self._lines_read)
        self.ParseLine()
        if self.rowcut.IsGoodRow():
            self.fCurrentDataPoint.SetName("p"+str(self.fMeasurement.GetNDataPoints()));
            self.fMeasurement.AddDataPoint(copy.deepcopy(self.fCurrentDataPoint))

    def ExtractAllValues(self):
        #for currentline in self.inputfile:
        for i in range(self.NInputLines):
            self.GetNextLine()
            self.GetNextValue()
        print "Extraction Complete"



