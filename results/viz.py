import numpy as np;
import matplotlib.pylab as plt
import matplotlib.gridspec as gridspec
import math

# simParams
integStepSize = 0.1;
ENVHEIGHT = 265.;
objRate = 3;
linesPerTrial = math.ceil((ENVHEIGHT/objRate)/integStepSize); # round((275./3)/integStepSize) = 916.7

# plot params
numberOfRuns = 20;
trialsPerRun = 24;
basePath = './4sym/';
lineLen = 33;

def plotIt(ind, rank):

    fig = plt.figure(figsize = [20,12]);
    plt.title('Run # '+str(ind)+' -- Rank # '+str(rank)+'/'+str(numberOfRuns), loc = 'left');

    # axes and subplots
    axBehavior = plt.subplot2grid((4,3), (0,0), colspan = 3, rowspan = 2);
    axRateCode = plt.subplot2grid((4,3), (2,0), colspan = 3);
    axBest = plt.subplot2grid((4,3), (3,0));
    axAvg = plt.subplot2grid((4,3), (3,1));
    axVar = plt.subplot2grid((4,3), (3,2));

    # read fitness file
    fitness = np.loadtxt(basePath+str(ind)+'/fitness.dat');

    # plot best fitness
    axBest.plot(fitness[:,0],fitness[:,1],'k', lw=1);
    axBest.set_ylabel('Best Fitness', fontsize = 14);
    #axBest.set_ylim(0,1);
    axBest.set_xlabel('Generation #', fontsize = 14);
    # plot avg fitness
    axAvg.plot(fitness[:,0],fitness[:,2],'k', lw=1);
    axAvg.set_ylabel('Average Fitness', fontsize = 14);
    #axAvg.set_ylim(0,1);
    axAvg.set_xlabel('Generation #', fontsize = 14);
    #plot fitness variance
    axVar.plot(fitness[:,0],fitness[:,3],'k', lw=1);
    axVar.set_ylabel('Fitness Variance', fontsize = 14);
    #axVar.set_ylim(0,1);
    axVar.set_xlabel('Generation #', fontsize = 14);
    # freeing memory
    fitness = [];

    # read and plot behavior
    bestB = np.loadtxt(basePath+str(ind)+'/bestCateg.dat');

    # plot isCircle
    categ = 0;
    for trial in range(trialsPerRun):
        start = (categ*linesPerTrial*trialsPerRun) + (trial)*linesPerTrial;
        axBehavior.plot(bestB[start:start+linesPerTrial,4]-bestB[start:start+linesPerTrial,2],bestB[start:start+linesPerTrial,3], 'b')
    #plot !isCircle
    categ = 1;
    for trial in range(trialsPerRun):
        start = (categ*linesPerTrial*trialsPerRun) + (trial)*linesPerTrial;
        axBehavior.plot(bestB[start:start+linesPerTrial,4]-bestB[start:start+linesPerTrial,2],bestB[start:start+linesPerTrial,3], 'r')
    axBehavior.set_xlabel('Relative Position of agent', fontsize=14)
    axBehavior.set_ylabel('Y position of object', fontsize=14)
    axBehavior.set_xlim(-100,100)

    # plot rate code difference between LM and RM
    #axRateCode.set_title("RateCode Difference", fontsize = 18)
    axRateCode.plot(bestB[:linesPerTrial,0],bestB[:linesPerTrial,5], 'b', label = "Trial1"); # categ 0
    axRateCode.plot(bestB[-linesPerTrial:,0],bestB[-linesPerTrial:,5], 'r', label = "Trial2"); # categ 1
    axRateCode.set_xlabel('Time', fontsize = 14);
    axRateCode.set_ylabel('Rate Code Difference', fontsize = 14);

    # save figure
    plt.savefig(basePath+'rank'+str(rank)+'_run'+str(ind)+'.png');
    plt.close()


bestFitVals = []
'''getting best fitness values for all runs'''
for i in range(numberOfRuns):
    f = open(basePath+str(i)+'/fitness.dat');
    f.seek(-lineLen,2);
    bestFitVals.append(np.fromstring(f.read(),sep=' ')[1]);
    f.close();

'''sorting'''
ranks = np.argsort(bestFitVals);
print 'Ranking - ', ranks;

for rank,ind in enumerate(ranks):
    print 'Plotting run #',ind;
    plotIt(ind,rank);
