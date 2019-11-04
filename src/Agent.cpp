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
#define SYM

Agent::Agent(int nsize, int nDistSensors, int nChemSensors,int nInterNeurons){
    cnsSize = nsize;
    numDistSensors = nDistSensors;
    numChemSensors = nChemSensors;
    numInterNeurons = nInterNeurons;
    rateCodeGain = 750;
}

//************
// Initialize
//************
void Agent::initNS(TVector<double> nsParams){
    //cout << "Initializing NS from Agent class" << endl;
    TVector<double> cnsParams,dToi, cToi, iToLM, iToRM;
    cnsParams.SetBounds(1,cnsSize*3+(cnsSize*cnsSize));
    int dToiSize = numDistSensors*cnsSize;
    int cToiSize = numChemSensors*cnsSize;
    int iToLMSize = cnsSize;
    int iToRMSize = cnsSize;

    int index = 1;
    // init cns
    for(int i=1; i<=cnsParams.Size(); i++){
        cnsParams[i] = nsParams[index];
        index++;
    }
    //cout << "\tcalling initCNS" << endl;
    ns.initCNS(cnsParams,cnsSize);

    TVector<bool> isInhibitory;
    isInhibitory.SetBounds(1,cnsSize);
    int idx = 1;
    for(int n=3; n<=cnsSize*3; n+=3){
        if(cnsParams[n] == 1){
            isInhibitory[idx] = 1;
        }
        else{
            isInhibitory[idx] = 0;
        }
        idx++;
    }

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
        if(isInhibitory[i])
            ns.interToLM[i] = -nsParams[index];
        else
            ns.interToLM[i] = nsParams[index];
        index++;
    }
    //cout << "\tinited interToLM weights" << endl;
    //ns.setInterToLM(TVector<double> iToLM);
    ns.interToRM.SetBounds(1,iToRMSize);
    for(int i=1; i<=iToRMSize; i++){
        if(isInhibitory[i])
            ns.interToRM[i] = -nsParams[index];
        else
            ns.interToRM[i] = nsParams[index];
        index++;
    }
    //cout << "\tinited interToRM weights" << endl;
    //ns.setInterToRM(iToRM);

    // if network needs to be symmetric,
    #ifdef SYM
	//cout << "\tMaking sensor-inter weights symmetric..." << endl;
        // make weights from distance sensors to inter symmetric
	int ind = 1;
        for(int i=dToiSize; i>double(dToiSize)/2; i--){
	    //cout << "\t\t" << i << " " << cnsSize-i+1 << endl;
            ns.distSensorToInter[i] = ns.distSensorToInter[ind];
	    ind++;
        }
	//cout << "\tMaking inter-motor weights symmetric..." << endl;
        //make inter to motor weights symmetric
	ind = 1;
        for(int i=iToRMSize; i>double(cnsSize)/2; i--){
	    //cout << "\t\tintertoLMRM" << i << " " << cnsSize-i+1 << endl;
            ns.interToRM[i] = ns.interToLM[ind];
            ns.interToLM[i] = ns.interToRM[ind];
	    ind++;
        }
    #endif

    //cout << "\tDone initing Agent" << endl;
}

//************
// Control
//************
double Agent::step(double stepSize, TVector<double> distanceInputs, TVector<double> chemInputs){
    //cout << "\t\tIn Agent step" << endl;
    double motorNeuronsDiff = ns.step(stepSize, distanceInputs, chemInputs);
    double velocity = (motorNeuronsDiff*rateCodeGain)/cnsSize;

    double offset = stepSize*velocity;
    //cout << "\t\tVelocity = " << velocity << " offset = " << offset << endl;
    return offset;
}

void Agent::reset(){
    ns.reset();
}
