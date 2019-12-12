#!/app/bin/python3
import sys,os
import math
import time
import numpy as np
import pylab as py
import matplotlib.pyplot as plt
from scipy.odr import *
from subprocess import call
from subprocess import check_output

# Nominal beam position (note that 3H07C is offset by (+1.0, -1.0))
BPM_NOMINAL_POS = {
'3H07A': {'XPOS': 0.0,
          'YPOS': 0.8},
'3H07C': {'XPOS': -1.0,
          'YPOS': 0.3}}

plt.ion()
fig=plt.figure()
# Aargh - used old, wrong value
#AZ=-370.83 
AZ=-320.42
BZ=-224.97
CZ=-129.314
ZVEC=np.array([AZ,BZ,CZ])

#XSLOPE=np.array([-0.9843,-1.116,-0.9645])
#XOFF=np.array([-0.05355,0.08082,-0.893])

#YSLOPE=np.array([0.9687,1.166,0.8703])
#YOFF=np.array([-0.2027,0.3751,0.466])

# Fit fro DEC 2019 BPm vs HARP scan . In fit made the Harp X coordinate the same sign as the X epics
XSLOPE=np.array([0.973,1.096,0.956])
XOFF=np.array([-.076,-0.200,0.937])

YSLOPE=np.array([0.957,1.16,0.855])
YOFF=np.array([0.17,-0.06,-0.94])

# changes to offsets from most recent survey
#DY=np.array([0.25,0.22,0.20])
#DX=np.array([-0.02,-0.09,-0.24])

# survey numbers above were updated..
#DY=np.array([0.21,0.21,0.29])
#DX=np.array([0.06,0.05,-0.09])

# from comparison to harp scans 8/29
#DY=np.array([0.36,0.36,0.31])
#DX=np.array([0.21,0.12,-0.29])

# from survey, Feb. 2019 (note these are relative to 2017 survey offsets)
#DY=np.array([-0.05,-0.23,-0.46])
#DX=np.array([0.16,0.21,0.03])

# from HARP/BPM comparison, Feb. 9 2019 !!!!WRONG!!!
#DY=np.array([0.26,0.21,0.06])
#DX=np.array([0.32,0.38,0.09])

# from HARP/BPM comparison, Feb. 9 2019
#DY=np.array([0.38,-0.13,-0.76])
#DX=np.array([0.32,0.38,0.09])

# For Dec 2019 comparison did a BPM vs HARP scan so do not need offset
DY=np.array([0.,0.,0.])
DX=np.array([0.,0.,0.])


XOFF=XOFF+DX
YOFF=YOFF+DY

EX=np.array([0.1,0.1,0.1])
EY=np.array([0.1,0.1,0.1])

# fit line using both x and y errors
def f(B, x):
    '''Linear function y = m*x + b'''
    # B is a vector of the parameters.
    # x is an array of the current x values.
    # x is in the same format as the x passed to Data or RealData.
    #
    # Return an array in the same format as y passed to Data or RealData.
    return B[0]*x + B[1]

while True:
    time.sleep(0.5)
    #read in epics stuff
    ibcm=float(check_output("caget -w5 IBC3H00CRCUR4 | awk \ '{print $2}'", shell=True).strip())

    AXraw=float(check_output("caget -w5 IPM3H07A.XPOS | awk \ '{print $2}'", shell=True).strip())
    BXraw=float(check_output("caget -w5 IPM3H07B.XPOS | awk \ '{print $2}'", shell=True).strip())
    CXraw=float(check_output("caget -w5 IPM3H07C.XPOS | awk \ '{print $2}'", shell=True).strip())

    AYraw=float(check_output("caget -w5 IPM3H07A.YPOS | awk \ '{print $2}'", shell=True).strip())
    BYraw=float(check_output("caget -w5 IPM3H07B.YPOS | awk \ '{print $2}'", shell=True).strip())
    CYraw=float(check_output("caget -w5 IPM3H07C.YPOS | awk \ '{print $2}'", shell=True).strip())

    XRAW=np.array([AXraw,BXraw,CXraw])
    YRAW=np.array([AYraw,BYraw,CYraw])

    if ibcm > 0.5:
        XVEC=(XRAW*XSLOPE+XOFF)
        YVEC=YRAW*YSLOPE+YOFF
    else:
        XVEC=XRAW*0.0
        YVEC=YRAW*0.0

    # Show X Position
    #ax1=fig.add_subplot(211)
    ax1=fig.add_subplot(221)
    # define 1st plot -x vs. z
    ax1.errorbar(ZVEC, XRAW, yerr=EX, fmt='o', color='blue', markersize=4)
    ax1.errorbar(ZVEC, XVEC, yerr=EX, fmt='o', color='red', markersize=4)
    linear = Model(f)
    mydata = RealData(ZVEC, XVEC, sy=EX)
    myodr = ODR(mydata, linear, beta0=[1., 2.])
    myoutput = myodr.run()
    #    myoutput.pprint()
    # get errors from covariance matrix - standard errors from ODR amplify by chi-squared
    xtar=round(myoutput.beta[1],2)

    # Plot the fit
    x_fit = np.linspace(-400, 50, 1000)
    y_fit = f(myoutput.beta, x_fit)
    plt.plot(x_fit, y_fit, color='black')

    #plt.xlabel('Distance from target (cm)',fontsize=15)
    plt.text(-400,2,"X Pos @ tgt: "+str(xtar));
    plt.ylabel('X (mm)',fontsize=15)
    py.ylim([-4,4])

    # Show Y Position
    #ax2=fig.add_subplot(212)
    ax2=fig.add_subplot(223)
    yraw = ax2.errorbar(ZVEC, YRAW, yerr=EX, fmt='o', color='blue', markersize=4)
    ycor = ax2.errorbar(ZVEC, YVEC, yerr=EX, fmt='o', color='red', markersize=4)
    linear = Model(f)
    mydata = RealData(ZVEC, YVEC, sy=EX)
    myodr = ODR(mydata, linear, beta0=[1., 2.])
    myoutput = myodr.run()
    #    myoutput.pprint()
    # get errors from covariance matrix - standard errors from ODR amplify by chi-squared
    ytar=round(myoutput.beta[1],2)

    # Plot the fit
    x_fit = np.linspace(-400, 50, 1000)
    y_fit = f(myoutput.beta, x_fit)
    plt.plot(x_fit, y_fit, color='black')

    plt.text(-400,2,"Y Pos @ tgt: "+str(ytar));
    plt.xlabel('Distance from target (cm)',fontsize=15)
    plt.ylabel('Y (mm)',fontsize=15)
    plt.legend
    plt.legend((yraw, ycor), ('raw', 'corrected'), loc='lower right')
    py.ylim([-4,4])

    # BPM monitoring-like Output
    ax3 = fig.add_subplot(122)
    ax3.errorbar(xtar,ytar, yerr=0, fmt='o', color='red', markersize=4)
    ax3.yaxis.tick_right()
    ax3.yaxis.set_label_position('right')
    ax3.yaxis.grid(color='grey')
    ax3.xaxis.grid(color='grey')
    plt.text(-3,3.2,"Beam position on target:\n"+8*" "+str(xtar)+" , "+str(ytar));
    plt.xlabel('Target X (mm)',fontsize=15)
    plt.ylabel('Target Y (mm)',fontsize=15)
    py.xlim([-4,4])
    py.ylim([-4,4])

   # print('xvec',XVEC)
   # print('yvec',YVEC)
    print('xtar is', xtar, 'ytar', ytar)

    plt.text(-13.5, 4.5,"Nominal raw 3H07A: ({}, {}), 3H07C: ({}, {})".format(
        BPM_NOMINAL_POS['3H07A']['XPOS'],
        BPM_NOMINAL_POS['3H07A']['YPOS'],
        BPM_NOMINAL_POS['3H07C']['XPOS'],
        BPM_NOMINAL_POS['3H07C']['YPOS']),
        size=15)

    #    plt.show()
    fig.canvas.draw()
    fig.canvas.flush_events()
    plt.pause(1)
#    plt.clf()
    #    plt.draw()
    ax1.clear()
    ax2.clear()
    ax3.clear()
