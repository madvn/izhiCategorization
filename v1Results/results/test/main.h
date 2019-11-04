#include <iostream>
#include <fstream>
#include <iomanip>
#include "Agent.h"
#include "TSearch.h"
#include "Environment.h"

// Simulation Parameters
const double integStepSize = 0.1;           // integration step size
struct taxisEvalParams{
    double evaluationDuration = 500;      // time to simulate network for before evaluating performance
    int numTrials = 1;                    // number of trials to average fitness over
};
struct categEvalParams{
    int numTrials = 24;                     // number of trials per category
    double maxDistance = 22.5;               // maxDistance between agent and object when oY = 0
};
taxisEvalParams tEvalParams;
categEvalParams cEvalParams;

// Agent Params
const int cnsSize = 5;                      // size of central nervous system - number of interneuronsss
const int genotypeSize = cnsSize*3 + (5*5) + (7*5) + 5 + 5 + 5;  // 3perNeuron+interToInter+distSensorToInter+chemSensorToInter+interToLM+interToRM = 120
const double inhibitoryThreshold = 0.8;     // value in genotype beyond which neuron is considered inhibitory
const int numDistSensors = 7;
const int numChemSensors = 1;
const int numInterNeurons = 5;
const int windowSize = 10/integStepSize;

// Evolution Params
const int popSize  = 100;
const int maxGens = 2000;
const double mutationVariance = 0.5;
const double crossoverProb = 0.5;

//Flags
int writeFlag = 0;
