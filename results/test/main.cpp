// ************************************************************
// Main for testing evolved categorizing Agent
// Madhavun Candadai Vasu
// Jun 27, 2016 - created
//
// ************************************************************
#include "main.h"
#include <string>
#include <sstream>

string folderInd = "E2/4";
//************
// Fitness
//************
void convertGenotypeToPhenotype(TVector<double> g, Agent& a){
    //cout << "Converting genotype to phenotype" << endl;
    TVector<double> nsParams;
    nsParams.SetBounds(1,genotypeSize);

	int ne=0, ni=0;
    int index;
    // First: scale individual neuron params
    for(int n=1; n <=cnsSize; n++){
        index = (n-1)*3;
        //scale params
        if(MapSearchParameter(g[index+3],0,1) > inhibitoryThreshold){
            // ranges set for inhibitory neurons
            nsParams[index+1] = MapSearchParameter(g[index+1],0.02,0.2);
            nsParams[index+2] = MapSearchParameter(g[index+2],0.2,0.3);
            nsParams[index+3] = 1;
		ni++;
        }
        else{
            // ranges set for excitatory neurons
            nsParams[index+1] = MapSearchParameter(g[index+1],-65,-50);;
            nsParams[index+2] = MapSearchParameter(g[index+2],2,8);
            nsParams[index+3] = 0;
		ne++;
        }
    }
	if(writeFlag == 1){
		cout << ne << "\n" << ni << endl;
	}
    //cout << "\tFinished scaling individual neuron params" << endl;

    // Second: scale weights
    for(int i=cnsSize*3+1; i<=genotypeSize; i++){
        nsParams[i] = MapSearchParameter(g[i],0,2);
    }
    //cout << "\tFinished scaling weights" << endl;

	if(writeFlag == 1){
		for(int i=1; i<=nsParams.Size(); i++)
			cout << nsParams[i] << endl;
	}
    a.initNS(nsParams);
}

double evaluateCateg(Agent& a){
    Environment env;
    double fitness = 0., fit1 = 0., fit2 = 0.;

    // vector of starting positions for object x-coordinate
    TVector<double> startingPositions;
    startingPositions.SetBounds(1,cEvalParams.numTrials*2);
    for(int i=1; i<=cEvalParams.numTrials; i++){
        double posOffset = (i-1)*(100./(cEvalParams.numTrials-1))-50.;
        startingPositions[i] = posOffset;                           // for isCircle trials
        startingPositions[i+cEvalParams.numTrials] = posOffset;   // for !isCircle trials
    }
    //cEvalParams.maxDistance = std::abs(startingPositions[1]);

    // externalInputs
    TVector<double> distanceInputs;
    distanceInputs.SetBounds(1,numDistSensors);
    distanceInputs.FillContents(0.);
    TVector<double> chemInputs;
    chemInputs.SetBounds(1,numChemSensors);
    chemInputs.FillContents(0.);

    double agentPos = 0.;
    // object params
    double oY, oX;
    bool isCircle = 1;

    for(int trial=1; trial<=cEvalParams.numTrials*2; trial++){
        //cout << "\tTrial-" << trial << endl;
        if(trial == cEvalParams.numTrials+1) isCircle = !isCircle;

        // reset inputs
        distanceInputs.FillContents(0.);
        // reset agent
        a.reset();

        double t=integStepSize;
        oY = ENVHEIGHT;
        oX = startingPositions[trial];
        agentPos = 0.;
        // present object and step agent
        while(oY > 0.){
            env.getDistanceInputs(distanceInputs, isCircle, agentPos, oX, oY);

            double offset = 0.;
            offset = a.step(integStepSize, distanceInputs, chemInputs);
            agentPos += offset;

            //cout << "\t\toY=" << oY << " oX=" << oX << " agentPos=" << agentPos << endl;
            if(writeFlag == 3){
                cout << t << " " << isCircle << " " << oX << " " << oY << " " << agentPos << " " << offset/integStepSize << " ";
                for(int d=1; d<numDistSensors; d++){
                    cout << distanceInputs[d] << " ";
                }
                for(int n=1; n<=cnsSize; n++){
                    cout << a.getNeuronStateV(n) << " ";
                }
                for(int n=1; n<=cnsSize; n++){
                    cout << a.getNeuronOutput(n) << " ";
                }
                cout << endl;
            }
            // Update Object's position
        	oY = oY - OBJVEL*integStepSize;
            t += integStepSize;
        }

        // Finished presenting object
        double distance = std::abs(agentPos - oX);

        if (distance > cEvalParams.maxDistance) {distance = cEvalParams.maxDistance;}
        distance = distance/cEvalParams.maxDistance;

        // Compute Fitness
        if (isCircle) {if (distance < DIAMETER) {fit1 += 1;}}
        else {if (distance > DIAMETER) {fit2 += 1;}}
    }

    //fitness = (fit1/cEvalParams.numTrials)*(fit2/cEvalParams.numTrials);
    fitness = (fit1+fit2)/(cEvalParams.numTrials*2);
    // Average over trials
    return fitness;///(cEvalParams.numTrials*2);
}

double evaluateTaxis(Agent& a){
    return 1.;
    /*Environment env;
    double fitness = 0,fit1 =0, fit2 = 0;
    double initialDist = 5, agentPos;
    // create env to test agent in
    double gradientPeak = 0;

    // externalInputs
    TVector<double> distanceInputs;
    distanceInputs.SetBounds(1,numDistSensors);
    distanceInputs.FillContents(0.);
    TVector<double> chemInputs;
    chemInputs.SetBounds(1,numChemSensors);
    chemInputs.FillContents(0.);

    for(int trial=1; trial<= tEvalParams.numTrials; trial++){
        // Trial 1
        //cout << "Starting Trial 1" << endl;
        agentPos = gradientPeak+initialDist;
        double lap, la = env.getConcentration(gradientPeak, agentPos);
        double offset = 0;
        // simulate agent in landscape
        //cout << "\t starting to simulate agent" << endl;
        for(double t=integStepSize; t<= tEvalParams.evaluationDuration; t+=integStepSize){
            lap = la;
            la = env.getConcentration(gradientPeak,agentPos);
            if(offset ==0){
                chemInputs[1] = 0;
            }
            else{
                chemInputs[1] = (la-lap)/offset;
            }
            //cout << "\t Trial 1 t=" << t << " la-lap=" << la-lap << " agentPos" << agentPos;
            // update position based on agent stepping
            offset = a.step(integStepSize, distanceInputs, chemInputs);
            agentPos += offset;
            //cout << " offset " << offset << endl;
            //cout << "\t back in main" << endl;
            // update fitness
            fit1 += std::abs(agentPos - gradientPeak);
            if(writeFlag == 2){
                cout << t << " " << gradientPeak << " " << agentPos << " " << offset/integStepSize << " ";
                for(int n=1; n<=cnsSize; n++){
                    cout << a.getNeuronStateV(n) << " ";
                }
                cout << endl;
            }
        }
        fit1 = 1 - (fit1/(initialDist*tEvalParams.evaluationDuration/integStepSize));
        //cout << "fit1 " << fit1;
        if(fit1 > 0){} else{fit1 = 0;}

        // reset agent
        a.reset();

        // Trial 2
        agentPos = gradientPeak-initialDist;
        la = env.getConcentration(gradientPeak,agentPos);
        offset = 0;
        // simulate agent in landscape
        for(double t=integStepSize; t<= tEvalParams.evaluationDuration; t+=integStepSize){
            lap = la;
            la = env.getConcentration(gradientPeak,agentPos);
            if(offset ==0){
                chemInputs[1] = 0;
            }
            else{
                chemInputs[1] = (la-lap)/offset;
            }
            //cout << "\t Trial 2 t=" << t << " la-lap=" << la-lap << " agentPos" << agentPos << endl;
            // update position based on agent stepping
            offset = a.step(integStepSize, distanceInputs, chemInputs);
            agentPos += offset;
            // update fitness
            fit2 += std::abs(agentPos - gradientPeak);
            if(writeFlag == 2){
                cout << t << " " << gradientPeak << " " << agentPos << " " << offset/integStepSize << " ";
                for(int n=1; n<=cnsSize; n++){
                    cout << a.getNeuronStateV(n) << " ";
                }
                cout << endl;
            }
        }
        fit2 = 1 - (fit2/(initialDist*tEvalParams.evaluationDuration/integStepSize));
        //cout << "fit2 " << fit2 << endl;
        if(fit2>0){}else{fit2=0;}

        fitness += (fit1*fit2);

        // reset agent
        a.reset();
    }
    return fitness/tEvalParams.numTrials;*/
}

double evaluateFitness(TVector<double> &g, RandomState &rs){
    //cout << "evaluateFitness" << endl;
    // init
    double fitness = 0, fitTaxis = 0., fitCateg = 0.;

    Agent a(cnsSize,numDistSensors, numChemSensors, numInterNeurons, windowSize);
    convertGenotypeToPhenotype(g,a);
    //cout << "converted Genotype To Phenotype" << endl;
    a.reset();
    fitTaxis = evaluateTaxis(a);
    a.reset();
    fitCateg = evaluateCateg(a);

    // Combine and return fitness
    fitness = fitTaxis * fitCateg;
    return fitness;
}

//************
// Test main
//************

int main(){
    //cout << windowSize << endl;

   ifstream paramsFile;
   paramsFile.open("../"+folderInd+"/paramsFile.dat");

   // config search
   long randomseed;
   paramsFile >> randomseed;
   paramsFile.close();
   cout << "randomseed " << randomseed << endl;

   RandomState rs;
   rs.SetRandomSeed(randomseed);

   ifstream genotypeFile;
   genotypeFile.open("../"+folderInd+"/best.gen.dat");
   string s;
   getline(genotypeFile,s);
   istringstream ss(s);
   TVector<double> genotype;
   genotype.SetBounds(1,genotypeSize);
   //cout << "ss " << ss << endl;

   int index = 1;
   while(ss && index <= genotypeSize){
       string s;
       getline(ss, s, ' ');
       //cout << s << "---" << index << endl;
       genotype[index] = stod(s);
       index++;
   }
   cout << "Done reading genotype! " << genotype.Size() << endl;

    ofstream bestBehaviorFile;
   	bestBehaviorFile.open("./bestCateg.dat");
   	cout.rdbuf(bestBehaviorFile.rdbuf());
    writeFlag = 3;

   double fitness;
   cEvalParams.numTrials = 50;
   fitness = evaluateFitness(genotype, rs);

   //cout.rdbuf();
   //cout << "Fitness " << fitness << endl;

}
