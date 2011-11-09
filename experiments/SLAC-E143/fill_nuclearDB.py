#!/usr/bin/python
import numpy as np
import pylab as plt
import matplotlib.mlab as mlab
import sys
import time
import datetime
import MySQLdb
import math


def ProtonE143DataToDBList(line,exp,ang):
    vals=line.split()
    results=[]
    results.append(experiment) #experiment
    results.append(float(ang)) #angle
    results.append(float(vals[0])) #x
    results.append(float(0))       #xlow
    results.append(float(0))       #xhigh
    results.append(float(vals[1])) #Q2
    results.append(float(0))       #Q2_low
    results.append(float(0))       #Q2_high
    results.append(float(vals[2])) #A
    results.append(float(0))      #syst_error_low
    results.append(float(0))      #syst_error_high
    results.append(float(vals[3])-float(vals[4]))     #stat_error_low
    results.append(float(0)       #stat_error_high
    results.append("'p'")     #nucleon
    return(results)

#$x$ & $<Q^2>$ &$A_==parallel$ & $A_{rc}^==parallel$ & $A_==perp$ & $A_{rc}^==perp$ ====
def DeuteronE143sDataToDBList(line,exp,ang):
    vals=line.split()
    results=[]
    results.append(float(vals[5])) #x
    results.append(float(0))       #xlow
    results.append(float(0))       #xhigh
    results.append(float(vals[6])) #Q2
    results.append(float(0))       #Q2_low
    results.append(float(0))       #Q2_high
    results.append(float(vals[7])) #A
    results.append(float(vals[7])-float(vals[8]))       #syst_error_low
    results.append(float(vals[7])+float(vals[8]))       #syst_error_high
    results.append(float(vals[7])-float(vals[9]))       #stat_error_low
    results.append(float(vals[7])+float(vals[9]))       #stat_error_high
    results.append("'d'")     #nucleon
    return(results)

### Loads slac E155 data into nuclearDB
# RAW ASYMMETRIES
### A parallel for proton and deuteron
datafile1 = "apar_pd_2.txt"
dataStartLine1=9
datafile2 = "apar_pd_5.txt"
dataStartLine2=9
datafile3 = "apar_pd_10.txt"
dataStartLine3=9
datafile4 = "aperp_pd_2.txt"
dataStartLine4=27
datafile5 = "aperp_pd_5.txt"
dataStartLine5=27
datafile6 = "aperp_pd_10.txt"
dataStartLine6=27

table="Aparallel"
experiment="'SLAC-E155'"
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
    res=ProtonE155DataToDBList(line,experiment,2.75)
    sqlcmd="REPLACE INTO " + table + " SET "
    for n, v in enumerate(res):
        sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
    sqlcmd = sqlcmd.strip(", ")
    sqlcmd = sqlcmd + " ; "
    print sqlcmd
    cursor.execute (sqlcmd)
# Close MySQL connection
cursor.close ()
conn.close ()
# close file
datafile.close()
##############################################################
#open file
datafile = open(datafile2, "r")
#go to first line with data
for i in range(dataStartLine2-1):
    nextline = datafile.readline() # keep it one line ahead of searchfile
# Open MySQL Database connection
conn = MySQLdb.connect (host = "quarks.temple.edu",
                        user = "bjorken",
                        passwd = "drell",
                        db = "nuclearDB")
cursor = conn.cursor ()
for line in datafile:
    res=ProtonE155DataToDBList(line,experiment,5.5)
    sqlcmd="REPLACE INTO " + table + " SET "
    for n, v in enumerate(res):
        sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
    sqlcmd = sqlcmd.strip(", ")
    sqlcmd = sqlcmd + " ; "
    print sqlcmd
    cursor.execute (sqlcmd)
# Close MySQL connection
cursor.close ()
conn.close ()
# close file
datafile.close()
##############################################################
#open file
datafile = open(datafile3, "r")
#go to first line with data
for i in range(dataStartLine3-1):
    nextline = datafile.readline() # keep it one line ahead of searchfile
# Open MySQL Database connection
conn = MySQLdb.connect (host = "quarks.temple.edu",
                        user = "bjorken",
                        passwd = "drell",
                        db = "nuclearDB")
cursor = conn.cursor ()
for line in datafile:
    res=ProtonE155DataToDBList(line,experiment,10.5)
    sqlcmd="REPLACE INTO " + table + " SET "
    for n, v in enumerate(res):
        sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
    sqlcmd = sqlcmd.strip(", ")
    sqlcmd = sqlcmd + " ; "
    print sqlcmd
    cursor.execute (sqlcmd)
# Close MySQL connection
cursor.close ()
conn.close ()
# close file
datafile.close()
##############################################################################
##############################################################################
table="Aperp"
experiment="'SLAC-E155'"
columnNames=["experiment","angle","x","x_low","x_high","Q2","Q2_low","Q2_high","A","syst_error_low","syst_error_high","stat_error_low","stat_error_high","nucleon"]

#open file
datafile = open(datafile4, "r")
#go to first line with data
for i in range(dataStartLine4-1):
    nextline = datafile.readline() # keep it one line ahead of searchfile
# Open MySQL Database connection
conn = MySQLdb.connect (host = "quarks.temple.edu",
                        user = "bjorken",
                        passwd = "drell",
                        db = "nuclearDB")
cursor = conn.cursor ()
for line in datafile:
    res=ProtonE155DataToDBList(line,experiment,2.75)
    sqlcmd="REPLACE INTO " + table + " SET "
    for n, v in enumerate(res):
        sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
    sqlcmd = sqlcmd.strip(", ")
    sqlcmd = sqlcmd + " ; "
    print sqlcmd
    cursor.execute (sqlcmd)
# Close MySQL connection
cursor.close ()
conn.close ()
# close file
datafile.close()
##############################################################
#open file
datafile = open(datafile5, "r")
#go to first line with data
for i in range(dataStartLine5-1):
    nextline = datafile.readline() # keep it one line ahead of searchfile
# Open MySQL Database connection
conn = MySQLdb.connect (host = "quarks.temple.edu",
                        user = "bjorken",
                        passwd = "drell",
                        db = "nuclearDB")
cursor = conn.cursor ()
for line in datafile:
    res=ProtonE155DataToDBList(line,experiment,5.5)
    sqlcmd="REPLACE INTO " + table + " SET "
    for n, v in enumerate(res):
        sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
    sqlcmd = sqlcmd.strip(", ")
    sqlcmd = sqlcmd + " ; "
    print sqlcmd
    cursor.execute (sqlcmd)
# Close MySQL connection
cursor.close ()
conn.close ()
# close file
datafile.close()
##############################################################
#open file
datafile = open(datafile6, "r")
#go to first line with data
for i in range(dataStartLine6-1):
    nextline = datafile.readline() # keep it one line ahead of searchfile
# Open MySQL Database connection
conn = MySQLdb.connect (host = "quarks.temple.edu",
                        user = "bjorken",
                        passwd = "drell",
                        db = "nuclearDB")
cursor = conn.cursor ()
for line in datafile:
    res=ProtonE155DataToDBList(line,experiment,10.5)
    sqlcmd="REPLACE INTO " + table + " SET "
    for n, v in enumerate(res):
        sqlcmd = sqlcmd + columnNames[n] + "=" + str(v) +", "
    sqlcmd = sqlcmd.strip(", ")
    sqlcmd = sqlcmd + " ; "
    print sqlcmd
    cursor.execute (sqlcmd)
# Close MySQL connection
cursor.close ()
conn.close ()
# close file
datafile.close()
## Make plot
#box = dict(facecolor='yellow', pad=5, alpha=0.2)
#fig = plt.figure()
#fig.subplots_adjust(left=0.2, wspace=0.6)
#labelx = -0.3  # axes coords

#ax1 = fig.add_subplot(221)
#ax1.set_title('valence q ')
#ax1.plot(values[0],values[2],'g',
         #values[0],values[4],'b')

#ax2 = fig.add_subplot(222)
#ax2.set_title('Delta q')
#ax2.plot(values[0],values[3],'g',
         #values[0],values[5],'b')

#ax3 = fig.add_subplot(223)
#ax3.set_title('Delta q')
#ax3.plot(values[0],values[6],'g',
         #values[0],values[7],'b')

#ax4 = fig.add_subplot(224)
#ax4.set_title('Gluon')
#ax4.plot(values[0],values[8],'g',
         #values[0],values[9],'b')

#plt.show()
