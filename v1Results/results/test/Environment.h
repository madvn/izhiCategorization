// ************************************************************
// Header file for Environment functions
// Madhavun Candadai Vasu
// Jun 22, 2016 - created
//
// ************************************************************
# pragma once

#include <iostream>
#include <cmath>
#include "VectorMatrix.h"
#include "random.h"

// Constants for distance sensors
const double COTAN0 = 3.73205080756888; 	// cotan(pi/12)
const double COTAN1 = 5.67128181961771;	    // cotan(pi/18)
const double COTAN2 = 11.43005230276134;    // cotan(pi/36)
const double DIAMETER = 30.0;               // diameter of cirle and agent, length of horizontal line
const double ENVHEIGHT = 275.0;             // starting oY value
const double SENSORDISTANCE = 220.0;        // max distance of sensors
const double ENVWIDTH = 400.0;
const double MAXDISTANCE = 45.0;            // max distance between aX and oX when oY=0, as far as fitness is concerned
const double OBJVEL = 3.;                  // rate at which object falls
class Environment{

    public:
        Environment(){};
        ~Environment(){};

        void getDistanceInputs(TVector<double>& distanceInputs, bool isCircle, double aX, double oX, double oY);
        double getConcentration(double gradientPeak, int agentPos);
};
