// ************************************************************
// Header file for Agent
// Madhavun Candadai Vasu
// Jun 06, 2016 - created
//
// ************************************************************

#include "NervousSystem.h"

class Agent{
    private:
        int cnsSize, numDistSensors, numChemSensors, numInterNeurons;
        NervousSystem ns; // nervous system
        //TVector<double> chemSensors;        // vector that holds the sensory inpu
    public:
        Agent(){};
        Agent(int nsize, int nDistSensors, int nChemSensors,int nInterNeurons, int windowSize);
        ~Agent(){};
        //void senseChemInputs(TVector<double> chemInputs);
        void initNS(TVector<double> nsParams);
        double step(double stepSize, TVector<double> distanceInputs, TVector<double> chemInputs); // one step of the agent's NS and body (same time scale?) and return offset in position
        void reset();
        double getNeuronStateV(int n){return ns.getStateV(n);};
        double getNeuronOutput(int n){return ns.getOutput(n);};
};
