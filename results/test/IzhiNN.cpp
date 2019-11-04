// ************************************************************
// Source file for Izhikevich Neural network
// Madhavun Candadai Vasu
// May 12, 2016 - created
//
// Other headers from Randall Beer
// ************************************************************


#include "IzhiNN.h"
#include "random.h"

//Constructor
IzhiNN::IzhiNN(int ne, int ni){
    setNetworkSize(ne,ni);
}
//destructor
IzhiNN::~IzhiNN(){
    setNetworkSize(0,0);
}

//*********
//UTILS
//*********
void IzhiNN::setNetworkSize(int ne, int ni){
    //cout << "\t Setting izhiNN size " << ne << " " << ni << endl;
    nEx = ne; nIn = ni;
    size = nEx+nIn;
    //set and init other vars that depend on size
    a.SetBounds(1,size);
    a.FillContents(0.0);
    b.SetBounds(1,size);
    b.FillContents(0.0);
    c.SetBounds(1,size);
    c.FillContents(0.0);
    d.SetBounds(1,size);
    d.FillContents(0.0);

    v.SetBounds(1,size);
    v.FillContents(0.0);
    u.SetBounds(1,size);
    u.FillContents(0.0);
    vbuf.SetBounds(1,size);
    vbuf.FillContents(0.0);
    ubuf.SetBounds(1,size);
    ubuf.FillContents(0.0);

    inputs.SetBounds(1,size);
    inputs.FillContents(0.0);
    outputs.SetBounds(1,size);
    outputs.FillContents(0.0);
    prevOutputs.SetBounds(1,size);
    prevOutputs.FillContents(0.);

    weights.SetBounds(1,size,1,size);
	weights.FillContents(0.0);
}

//*********
//INIT
//*********
void IzhiNN::randomInitNeuronParams() {
    // randomly init for excitatory neurons
    for(int i=1; i<=nEx; i++){
        double re = UniformRandom(0,1);
        // params
        a[i] = 0.02;
        b[i] = 0.2;
        c[i] = -65+(15*re*re);
        d[i] = 8-(6*re*re);
    }
    // randomly init for inhibitory neurons
    for(int i=nEx+1; i<=size; i++){
        double ri = UniformRandom(0,1);
        // params
        a[i] = 0.02+(0.08*ri);
        b[i] = 0.25-(0.05*ri);
        c[i] = -65;
        d[i] = 2;
    }
}

void IzhiNN::refractoryInitNeuronStates(){
    for(int i=1; i<=size; i++){
        // refractory init states
        v[i] = -65;
        u[i] = b[i]*v[i];
    }
}

void IzhiNN::randomInitNeuronStates(RandomState& rs){
    for(int i=1; i<=size; i++){
        // random init states
        v[i] = rs.UniformRandom(-65,20); // refractory state to almost spiking
        u[i] = b[i]*v[i];
    }
}

void IzhiNN::randomInitNetwork(){
    for(int i=1; i<=size; i++){
        // from excitatory neurons
        for(int j=1; j<=nEx; j++){
            weights[j][i] = UniformRandom(0,1)*0.5;
        }
        // from inhibitory neurons
        for(int j=nEx+1; j<=size; j++){
            weights[j][i] = -UniformRandom(0,1);
        }
    }
}

//*********
//CONTROL
//*********
void IzhiNN::eulerStep(double stepSize, TVector<double> externalInputs){
    //cout << "\t\t\tIn euler step for IzhiNN " << size << endl;
    // check for neurons that may spike
    for(int i=1; i<= size; i++){
        ////cout << "\t\t\t\t| i = " << i << "|v[i] = " << v[i];
        if (v[i] > SPIKEV){
            // spike
            outputs[i] = 1;
            v[i] = c[i];
            u[i] += d[i];
        }
        else{
            // no spike
            outputs[i] = 0;
        }
         ////cout << "| o[i] = " << outputs[i] << "||";
    }
    ////cout << endl;

    // update buffers
    for (int i = 1; i <= size; i++) {
        ////cout << "\t\t\t\tUpdating buffer for neuron " << i << endl;
        if(!outputs[i]){
            double input = externalInputs[i];
            //input from other neurons
            for (int j = 1; j <= size; j++)
                input += weights[j][i] * prevOutputs[j];

            double vi = v[i], ui = u[i];
            vbuf[i] = vi+0.5*stepSize*(0.04*vi*vi + 5*vi + 140 - ui + input);   // half step
            vbuf[i] = vbuf[i]+0.5*stepSize*(0.04*vbuf[i]*vbuf[i] + 5*vbuf[i] + 140 - ui + input);   // second half step for numerical stability
            ubuf[i] = ui + stepSize*(a[i]*(b[i]*vi - ui));
        }
    }

    // update actual vars from buffers
    for(int i=1;i <= size; i++){
        if(!outputs[i]){
            v[i] = vbuf[i];
            u[i] = ubuf[i];
        }
    }

    for(int i=1; i<=size; i++){
        prevOutputs[i] = outputs[i];
    }
}

//*********
//INPUT/OUTPUT
//*********
/*
#include <iomanip>

ostream& operator<<(ostream& os, IzhiNN& inn){
	// Set the precision
	os << setprecision(32);
    int size = inn.getNumInhibitory()+inn.getNumExcitatory();
    // Write size
    os << inn.getNumExcitatory() << endl << endl;
    os << inn.getNumInhibitory() << endl << endl;
    // Write params
    for (int i = 1; i <= size; i++)
		os << inn.getParamA(i) << " ";
	os << endl << endl;
    for (int i = 1; i <= size; i++)
		os << inn.getParamB(i) << " ";
	os << endl << endl;
    for (int i = 1; i <= size; i++)
		os << inn.getParamC(i) << " ";
	os << endl << endl;
    for (int i = 1; i <= size; i++)
		os << inn.getParamD(i) << " ";
	os << endl << endl;
    // Write states
    for (int i = 1; i <= size; i++)
        os << inn.getStateV(i) << " ";
    os << endl << endl;
    for (int i = 1; i <= size; i++)
        os << inn.getStateU(i) << " ";
    os << endl << endl;
    //Write weights
    for (int i = 1; i <= size; i++) {
		for (int j = 1; j <= size; j++)
			os << inn.getWeights(i,j) << " ";
		os << endl << endl;
	}
    //Write outputs
    for(int i=1; i<=size; i++){
        os << inn.getOutput(i) << " ";
    os << endl;
    }
    // Return the ostream
    return os;
}

istream& operator>>(istream& is, IzhiNN& inn){
    //Read size
    int ne,ni,size;
    is >> ne;
    is >> ni;
    inn.setNetworkSize(ne,ni);
    size = ne+ni;
    // Read params
    for (int i = 1; i <= size; i++){
        double paramAi;
        is >> paramAi;
        inn.setParamA(i,paramAi);
    }
    for (int i = 1; i <= size; i++){
        double paramBi;
        is >> paramBi;
        inn.setParamB(i,paramBi);
    }
    for (int i = 1; i <= size; i++){
        double paramCi;
        is >> paramCi;
        inn.setParamC(i,paramCi);
    }
    for (int i = 1; i <= size; i++){
        double paramDi;
        is >> paramDi;
        inn.setParamD(i,paramDi);
    }
	// Read states
    for (int i = 1; i <= size; i++){
        double stateVi;
        is >> stateVi;
        inn.setStateV(i,stateVi);
    }
    for (int i = 1; i <= size; i++){
        double stateUi;
        is >> stateUi;
        inn.setStateU(i,stateUi);
    }
    // Read weights
    for (int i=1; i<=size; i++){
        for(int j=1; j<=size; j++){
            double weightij;
            is >> weightij;
            inn.setWeights(i,j,weightij);
        }
    }
}*/
