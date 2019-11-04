// ************************************************************
// Header file for Izhikevich Neural network
// Madhavun Candadai Vasu
// May 12, 2016 - created
//
// Other headers from Randall Beer
// ************************************************************

# pragma once

#include "VectorMatrix.h"
#include "random.h"
#include <iostream>
#include <math.h>

class IzhiNN{
    private:
        const int SPIKEV = 30;
        int nEx,nIn,size;                   // number of excitatory, inhibitory, size
        TVector<double> a,b,c,d;            // parameters of Izhikevich neuron
        TVector<double> v,u,vbuf,ubuf;      // states and temp buffers
        TVector<double> inputs;             // external input
        TVector<int> outputs,prevOutputs;   // outputs are 0 or 1
        TMatrix<double> weights;            // weight matrix

    public:
        //constructor
        IzhiNN(int ne = 0, int ni =0);
        //destructor
        ~IzhiNN();

        //getters and setters - size
        void setNetworkSize(int ne, int ni); // defined in cpp file
        int getNumExcitatory() {return nEx;};
        int getNumInhibitory() {return nIn;};
        //getters and setters - Params
        void setParamA(int neuronIndex, double paramVal){a[neuronIndex] = paramVal;};
        double getParamA(int neuronIndex){return a[neuronIndex];};
        void setParamB(int neuronIndex, double paramVal){b[neuronIndex] = paramVal;};
        double getParamB(int neuronIndex){return b[neuronIndex];};
        void setParamC(int neuronIndex, double paramVal){c[neuronIndex] = paramVal;};
        double getParamC(int neuronIndex){return c[neuronIndex];};
        void setParamD(int neuronIndex, double paramVal){d[neuronIndex] = paramVal;};
        double getParamD(int neuronIndex){return d[neuronIndex];};
        //getters and setters - states
        void setStateV(int neuronIndex, double value){v[neuronIndex] = value;};
        double getStateV(int neuronIndex){return v[neuronIndex];};
        void setStateU(int neuronIndex, double value){u[neuronIndex] = value;};
        double getStateU(int neuronIndex){return u[neuronIndex];};
        //getters and setters - inputs/outputs
        void setInput(int neuronIndex, double inputVal){inputs[neuronIndex] = inputVal;};
        double getInput(int neuronIndex){return inputs[neuronIndex];};
            // cannot set output directly; only via v
            //void setOutput(int neuronIndex, double outputVal){outputs[neuronIndex] = outputVal;};
        double getOutput(int neuronIndex){return outputs[neuronIndex];};
        //getters and setters - weights
        void setWeights(int from, int to, double val){weights[from][to] = val;};
        double getWeights(int from,int to){return weights[from][to];};

        // network dynamics
        void eulerStep(double stepSize, TVector<double> externalInputs);
        void randomInitNeuronParams();
        void randomInitNeuronStates(RandomState& rs);
        void refractoryInitNeuronStates();
        void randomInitNetwork();

};
