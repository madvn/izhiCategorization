// ************************************************************
// Definition file for Agent's nervous system
// Madhavun Candadai Vasu
// Jun 07, 2016 - created
//
// ************************************************************

#include "NervousSystem.h"

//TODO: setters for peripheral nervous system
// init cns
void NervousSystem::initCNS(TVector<double> cnsParams, int numCns){
    cnsSize = numCns;
    // getting structure information
    TVector<bool> isInhibitory;
    isInhibitory.SetBounds(1,numCns);
    int index = 1, ne=0, ni=0;
    for(int n=3; n<=numCns*3; n+=3){
        if(cnsParams[n] == 1){
            isInhibitory[index] = 1;
            ni++;
        }
        else{
            isInhibitory[index] = 0;
            ne++;
        }
        index++;
    }
    //cout << "ne " << ne << " ni " << ni << endl;
    cns.setNetworkSize(ne,ni);

    // setting neuron params
    for(int n=1; n<=cnsSize; n++){
        index = (n-1)*3;
        if(isInhibitory(n)){
            cns.setParamA(n, cnsParams[index+1]);        // ranges set for inhibitory neurons
            cns.setParamB(n, cnsParams[index+2]);      // ranges set for inhibitory neurons
            cns.setParamC(n, -65);                                         // ranges set for inhibitory neurons
            cns.setParamD(n, 2);                                            // ranges set for inhibitory neurons
        }
        else{
            cns.setParamA(n, .02);                                         // ranges set for excitatory neurons
            cns.setParamB(n, .2);                                            // ranges set for excitatory neurons
            cns.setParamC(n, cnsParams[index+1]);        // ranges set for excitatory neurons
            cns.setParamD(n, cnsParams[index+2]);            // ranges set for excitatory neurons
        }
    }

    // setting neuron weights
    index = numCns*3 + 1;
    for(int from=1; from<=numCns; from++){
        for(int to=1; to<=numCns; to++){
            if(isInhibitory[from])
                cns.setWeights(from,to,-cnsParams[index]);
            else
                cns.setWeights(from,to,cnsParams[index]);
            index++;
        }
    }

    // init states
    cns.refractoryInitNeuronStates();

    // init externalInputs
    externalInputs.SetBounds(1,numCns);
    externalInputs.FillContents(0.);

    // init windows
    for(int i=1; i<=windowSize; i++) LMWindow.push(0.);
	LMWinsum = 0;
    for(int i=1; i<=windowSize; i++) RMWindow.push(0.);
	RMWinsum = 0;
}

double NervousSystem::step(double stepSize, TVector<double> distanceInputs, TVector<double> chemInputs){
    //cout << "\t\t in NervousSystem step" << endl;
    int numDistSensors = distanceInputs.Size();
    int numChemSensors = chemInputs.Size();

    // reset externalInputs
    externalInputs.FillContents(0.);

    // input from distance sensors
    //cout << "\t\t\tSetting externalInputs from dist sensors" << endl;
    int index =1;
    for(int i=1; i<=cnsSize; i++){
        for(int j=1; j<=numDistSensors; j++){
            externalInputs[i] += distSensorToInter[index]*distanceInputs[j];
            index++;
        }
    }

    // input from chem sensors
    //cout << "\t\t\tSetting externalInputs from chemSensors" << endl;
    index =1;
    for(int i=1; i<=cnsSize; i++){
        for(int j=1; j<=numChemSensors; j++){
            externalInputs[i] += chemSensorToInter[index]*chemInputs[j];
            index++;
        }
    }

    // step izhiNN
    cns.eulerStep(stepSize,externalInputs);

    // update windows and return difference in rate
    updateRate();
	//cout << "\t\tRMWINSUM " << RMWinsum << " LMWINSUM " << LMWinsum;
    return RMWinsum - LMWinsum;
}

void NervousSystem::updateRate(){
    double outSum;
    // update LM
    outSum = 0.;
    for(int n=1; n<=cnsSize; n++){
        outSum += cns.getOutput(n)*interToLM[n];
    }
    LMWinsum -= LMWindow.front();
    LMWindow.pop();
    LMWinsum += outSum;
    LMWindow.push(outSum);

    // update RM
    outSum = 0.;
    for(int n=1; n<=cnsSize; n++){
        outSum += cns.getOutput(n)*interToRM[n];
    }
    RMWinsum -= RMWindow.front();
    RMWindow.pop();
    RMWinsum += outSum;
    RMWindow.push(outSum);
}

void NervousSystem::reset(){
    // reset neuron states
    cns.refractoryInitNeuronStates();

    // reset RateCode windows
    int s = LMWindow.size();
    for(int i=1; i<=s; i++){
        LMWindow.pop();
        LMWindow.push(0.);
        RMWindow.pop();
        RMWindow.push(0.);
    }
    LMWinsum = 0.;
    RMWinsum = 0.;
}
