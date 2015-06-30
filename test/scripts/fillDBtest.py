from ROOT import gROOT,gSystem
gSystem.Load( 'libFoam' )
gSystem.Load( 'libTreeViewer' )
gSystem.Load( 'libInSANE' )
gSystem.Load( 'libNucDB' )

import numpy as np
import pylab as plt
import matplotlib.mlab as mlab
import sys
import time
import datetime
import MySQLdb
import math
import os
from ROOT import TCanvas,NucDBManager,NucDBExperiment,NucDBMeasurement,NucDBDataPoint

import NucDBRawDataExtractor


def ProtonRSSDataToDBList(line,exp,ang):
    """
    Returns a properly filled 
    """    vals=line.split()
    results=[]
    results.append(experiment) #experiment
    results.append(float(ang)) #angle
    results.append(float(vals[4])) #x
    results.append(float(0))       #xlow
    results.append(float(0))       #xhigh
    results.append(float(vals[3])) #Q2
    results.append(float(0))       #Q2_low
    results.append(float(0))       #Q2_high
    results.append(float(vals[10])) #A
    results.append(float(vals[11])/2.0)      #syst_error_low
    results.append(float(vals[11])/2.0)      #syst_error_high
    results.append(float(vals[3])-float(vals[4]))     #stat_error_low
    results.append(float(0)  )     #stat_error_high
    results.append("'p'")     #nucleon
    return(results)



manager = NucDBManager.GetManager()

manager.ListExperiments()
manager.ListMeasurements()

experiment = manager.GetExperiment("RSS")
if not experiment :
    experiment = NucDBExperiment("RSS","RSS")

Aperp = experiment.GetMeasurement("Aperpp")
if not Aperp :
    Aperp = NucDBMeasurement("Aperpp","A_{\perp}^{p}")
Apara = experiment.GetMeasurement("Aparap")
if not Apara :
    Apara = NucDBMeasurement("Aparap","A_{\para}^{p}")

experiment.fMeasurements.Add(Aperp)
experiment.fMeasurements.Add(Apara)

extractor = NucDBRawDataExtractor()
extractor.SetMeasurement(Apara)


# RAW ASYMMETRIES
datafile1 = "rss_proton_b2.dat"
dataStartLine1=115

table="Aperp"
experiment="'RSS'"
columnNames=["experiment","angle","x","x_low","x_high","Q2","Q2_low","Q2_high","A","syst_error_low","syst_error_high","stat_error_low","stat_error_high","nucleon"]

#open file
datafile = open(datafile1, "r")
#go to first line with data
for i in range(dataStartLine1-1):
    nextline = datafile.readline() # keep it one line ahead of searchfile
# Open MySQL Database connection
conn = MySQLdb.connect (host = "quarks.temple.edu",
                        user = "bjorken",
                        passwd = "drell",
                        db = "nuclearDB")
cursor = conn.cursor ()
for line in datafile: 
    if line.split()[0] == "0" :
        res=ProtonRSSDataToDBList(line,experiment,0.0)
        sqlcmd="REPLACE INTO " + table + " SET "
        for n, v in enumerate(res):
            sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
        sqlcmd = sqlcmd.strip(", ")
        sqlcmd = sqlcmd + " ; "
        print sqlcmd
        cursor.execute (sqlcmd)



datapoint = NucDBDataPoint(1.0,0.2)
Aperp.AddDataPoint(datapoint)
experiment.Print()

manager.SaveExperiment(experiment)





#$x$ & $<Q^2>$ &$A_==parallel$ & $A_{rc}^==parallel$ & $A_==perp$ & $A_{rc}^==perp$ ====


# RAW ASYMMETRIES
#datafile1 = "../experiments/RSS/rss_proton_b2.dat"
#dataStartLine1=115
#table="Aperp"
#experiment="'RSS'"
#columnNames=["experiment","angle","x","x_low","x_high","Q2","Q2_low","Q2_high","A","syst_error_low","syst_error_high","stat_error_low","stat_error_high","nucleon"]

##open file
#datafile = open(datafile1, "r")
##go to first line with data
#for i in range(dataStartLine1-1):
    #nextline = datafile.readline() # keep it one line ahead of searchfile
## Open MySQL Database connection
#conn = MySQLdb.connect (host = "quarks.temple.edu",
                        #user = "bjorken",
                        #passwd = "drell",
                        #db = "nuclearDB")
#cursor = conn.cursor ()
#for line in datafile: 
    #if line.split()[0] == "0" :
        #res=ProtonRSSDataToDBList(line,experiment,0.0)
        #sqlcmd="REPLACE INTO " + table + " SET "
        #for n, v in enumerate(res):
            #sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
        #sqlcmd = sqlcmd.strip(", ")
        #sqlcmd = sqlcmd + " ; "
        #print sqlcmd
        #cursor.execute (sqlcmd)
## Close MySQL connection
#cursor.close ()
#conn.close ()
## close file
#datafile.close()


