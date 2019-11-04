import numpy as np;
import matplotlib.pylab as plt
import matplotlib.gridspec as gridspec

# simParams
integStepSize = 0.1;
ENVHEIGHT = 265.;
objRate = 3;
linesPerTrial = round((ENVHEIGHT/objRate)/integStepSize); # round((275./3)/integStepSize) = 916.7
numberOfNeurons = 5;

# plot params
numberOfRuns = 20;
trialsPerRun = 24;
basePath = './2sym/';
lineLen = 33;

for i in range(numberOfRuns):
    fig = plt.figure(figsize = [20,12]);
    print "Reading file.."
    bestB = np.loadtxt(basePath+str(i)+'/bestCateg.dat');

    for n in range(numberOfNeurons):
        print "\tPlotting neuron #",n
        ax = plt.subplot2grid((numberOfNeurons,1), (n,0));
        ax.plot(bestB[:linesPerTrial,0],bestB[:linesPerTrial,-(n+1)])
        ax.plot(bestB[-linesPerTrial:,0],bestB[-linesPerTrial:,-(n+1)],'r')

    plt.show()
