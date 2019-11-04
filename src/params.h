// ************************************************************
// Header file that contains all params and consts
// Madhavun Candadai Vasu
// Jun 30, 2016 - created
//
// ************************************************************
# pragma once
# define SYM

// Simulation Parameters
const double integStepSize = 0.1;           // integration step size
struct taxisEvalParams{
    const double evaluationDuration = 500;      // time to simulate network for before evaluating performance
    const int numTrials = 1;                    // number of trials to average fitness over
};
struct categEvalParams{
    const int numTrials = 24;                     // number of trials per category
    const double maxDistance = 45;               // maxDistance between agent and object when oY = 0
};

// Agent Params
const int cnsSize = 5;                      // size of central nervous system - number of interneuronsss
const double inhibitoryThreshold = 0.8;     // value in genotype beyond which neuron is considered inhibitory
const int numDistSensors = 7;
const int numChemSensors = 1;
const int numInterNeurons = cnsSize;
const int genotypeSize = cnsSize*3 + cnsSize*cnsSize + numDistSensors*cnsSize + numChemSensors*cnsSize + cnsSize + cnsSize + 1 + 1;
// 3perNeuron+interToInter+distSensorToInter+chemSensorToInter+interToLM+interToRM+rateCodeGain+windowSize
//const int windowSize = 15/integStepSize;

// Environment parms.
const double DIAMETER = 30;
const double ENVWIDTH = 400;
const double OBJVEL = 3.;
const double ENVHEIGHT = 265;
const double SENSORDISTANCE = 250;

// Evolution Params
const int popSize  = 100;
const int maxGens = 3000;
const double mutationVariance = 0.5;
const double crossoverProb = 0.5;

// IzhiNN
const int SPIKEV = 30;
