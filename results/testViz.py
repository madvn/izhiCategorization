import numpy as np;
import matplotlib.pylab as plt
import matplotlib.gridspec as gridspec
import math

# simParams
integStepSize = 0.1;
ENVHEIGHT = 275.;
objRate = 3;
linesPerTrial = math.ceil((ENVHEIGHT/objRate)/integStepSize); # round((275./3)/integStepSize) = 916.7

# plot params
trialsPerRun = 24;
lineLen = 33;
basePath = './'
fig = plt.figure(figsize = [20,12]);
# axes and subplots
axBehavior = plt.subplot2grid((3,1), (0,0), rowspan = 2);
axRateCode = plt.subplot2grid((3,1), (2,0));

# read and plot behavior
bestB = np.loadtxt(basePath+'bestCateg.dat');

# plot isCircle
categ = 0;
for trial in range(trialsPerRun):
    start = (categ*linesPerTrial*trialsPerRun) + (trial)*linesPerTrial;
    axBehavior.plot(bestB[start:start+linesPerTrial,4]-bestB[start:start+linesPerTrial,2],bestB[start:start+linesPerTrial,3], 'b')
    #axBehavior.plot(bestB[start:start+linesPerTrial,4]-bestB[start:start+linesPerTrial,2],bestB[start:start+linesPerTrial,0], 'b')
#plot !isCircle
categ = 1;
for trial in range(trialsPerRun):
    start = (categ*linesPerTrial*trialsPerRun) + (trial)*linesPerTrial;
    axBehavior.plot(bestB[start:start+linesPerTrial,4]-bestB[start:start+linesPerTrial,2],bestB[start:start+linesPerTrial,3], 'r')
    #axBehavior.plot(bestB[start:start+linesPerTrial,4]-bestB[start:start+linesPerTrial,2],bestB[start:start+linesPerTrial,0], 'r')
axBehavior.set_xlabel('Relative Position of agent', fontsize=14)
axBehavior.set_ylabel('Y position of object', fontsize=14)

# plot rate code difference between LM and RM
#axRateCode.set_title("RateCode Difference", fontsize = 18)
axRateCode.plot(bestB[:linesPerTrial,3],bestB[:linesPerTrial,5], 'b', label = "Trial1"); # categ 0
axRateCode.plot(bestB[-linesPerTrial:,3],bestB[-linesPerTrial:,5], 'r', label = "Trial2"); # categ 1
axRateCode.set_xlabel('Time', fontsize = 14);
axRateCode.set_ylabel('Rate Code Difference', fontsize = 14);


plt.show()
