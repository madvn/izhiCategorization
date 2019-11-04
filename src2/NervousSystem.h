// ************************************************************
// Header file for Agent's nervous system
// Madhavun Candadai Vasu
// Jun 07, 2016 - created
//
// TODO: Make atrributes private
// ************************************************************

#pragma once

#include "IzhiNN.h"
#include <queue>

class NervousSystem{
    private:
        IzhiNN cns;                                     //central nervous sytem
        int cnsSize, windowSize;
        queue<double> LMWindow,RMWindow;       // window of activity for each motor
        double LMWinsum, RMWinsum;                      // sum of activity in queue
        TVector<double> externalInputs;                 // inputs to cns

    public:
        NervousSystem(){};
        ~NervousSystem(){};

        // TODO make these private and use getters - might slow down the execution though
        TVector<double> distSensorToInter, chemSensorToInter, interToLM, interToRM;  // weights for peripheral nervous system
        // setters for peripehral weights
        //void setDistSensorToInter(TVector<double> dToi);
        //void setChemSensorToInter(TVector<double> cToi);
        //void setInterToLM(TVector<double> iToLM);
        //void setInterToRM(TVector<double> iToRM);
        void setWindowSize(int winSize){windowSize = winSize;};

        // init cns
        void initCNS(TVector<double> cnsParams, int cnsSize);
        double step(double stepSize, TVector<double> distanceInputs, TVector<double> chemInputs);
        void updateRate();
        void reset();
	double getStateV(int n){return cns.getStateV(n);};
};
