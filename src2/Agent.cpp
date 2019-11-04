// ************************************************************
// Agent class definition
// Madhavun Candadai Vasu
// Jun 06, 2016 - created
//
// TODO: in initNS use setters to set private attributes
// ************************************************************

//************
// Constructor
//************
#include "Agent.h"

Agent::Agent(int nsize, int nDistSensors, int nChemSensors,int nInterNeurons, int windowSize){
    cnsSize = nsize;
	rateCodingGain = 1;
    numDistSensors = nDistSensors;
    numChemSensors = nChemSensors;
    numInterNeurons = nInterNeurons;
    ns.setWindowSize(windowSize);
}

//************
// Initialize
//************
void Agent::initNS(TVector<double> nsParams){
    //cout << "Initializing NS from Agent class" << endl;
    TVector<double> cnsParams,dToi, cToi, iToLM, iToRM;
    cnsParams.SetBounds(1,cnsSize*3+(cnsSize*cnsSize));
    int dToiSize = numDistSensors*numInterNeurons;
    int cToiSize = numChemSensors*numInterNeurons;
    int iToLMSize = numInterNeurons;
    int iToRMSize = numInterNeurons;

    int index = 1;
    // init cns
    for(int i=1; i<=cnsParams.Size(); i++){
        cnsParams[i] = nsParams[index];
        index++;
    }
    //cout << "\tcalling initCNS" << endl;
    ns.initCNS(cnsParams,cnsSize);

    // setters for peripehral weights
    ns.distSensorToInter.SetBounds(1,dToiSize);
    for(int i=1; i<=dToiSize; i++){
        ns.distSensorToInter[i] = nsParams[index];
        index++;
    }
    //cout << "\tinitied distSensorToInter weights" << endl;
    //ns.setDistSensorToInter(TVector<double> dToi);
    ns.chemSensorToInter.SetBounds(1,cToiSize);
    for(int i=1; i<=cToiSize; i++){
        ns.chemSensorToInter[i] = nsParams[index];
        index++;
    }
    //cout << "\tinited chemSensorToInter weights" << endl;
    //ns.setChemSensorToInter(TVector<double> cToi);
    ns.interToLM.SetBounds(1,iToLMSize);
    for(int i=1; i<=iToLMSize; i++){
        ns.interToLM[i] = nsParams[index];
        index++;
    }
    //cout << "\tinited interToLM weights" << endl;
    //ns.setInterToLM(TVector<double> iToLM);
    ns.interToRM.SetBounds(1,iToRMSize);
    for(int i=1; i<=iToRMSize; i++){
        ns.interToRM[i] = nsParams[index];
        index++;
    }
    //cout << "\tinited interToRM weights" << endl;
    //ns.setInterToRM(TVector<double> iToRM);
	rateCodingGain = nsParams[index];
}

//************
// Control
//************
double Agent::step(double stepSize, TVector<double> distanceInputs, TVector<double> chemInputs){
    //cout << "\t\tIn Agent step" << endl;
    double motorNeuronsDiff = ns.step(stepSize, distanceInputs, chemInputs);
    double velocity = motorNeuronsDiff;

    double offset = ((stepSize*velocity)/cnsSize)*rateCodingGain;
    //cout << "\t\tVelocity = " << velocity << " offset = " << offset << endl;
    return offset;
}

void Agent::reset(){
    ns.reset();
}
