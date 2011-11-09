#!/usr/bin/python
import numpy as np
import pylab as plt
import matplotlib.mlab as mlab
import sys
import time
import datetime
import MySQLdb
import math

#$x$ & $<Q^2>$ &$A_==parallel$ & $A_{rc}^==parallel$ & $A_==perp$ & $A_{rc}^==perp$ ====

def ProtonRSSDataToDBList(line,exp,ang):
    vals=line.split()
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
# Close MySQL connection
cursor.close ()
conn.close ()
# close file
datafile.close()
# Close MySQL connection
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
