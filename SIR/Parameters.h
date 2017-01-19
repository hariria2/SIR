//
//  Parameters.h
//  SIR
//
//  Created by Sahand Hariri on 7/17/16.
//  Copyright © 2016 Sahand Hariri. All rights reserved.
//

#ifndef Parameters_h
#define Parameters_h

// Simulation Version
#define Version              "23"
#define isSingleLocation     false

// Storage and Visualization
#define StoreData            true

// Global Timing
#define endTime              43300
#define timeStep             1

// Means
#define ageMean              30
#define susceptibilityMean   3.1
#define initialConditionMean 2
#define betaMean             3
#define deltaMean            0.1
#define PMean                2
#define CMean                1
#define sociabilityMean      0
#define stepSizeMean         3.5
#define lifeExpectencyMean   82


#define ageVar               10
#define susceptibilityVar    1
#define initialConditionVar  .5
#define betaVar              0.02
#define deltaVar             0.02
#define PVar                 0.1
#define CVar                 0.1
#define sociabilityVar       2
#define stepSizeVar          0.05
#define lifeExpectencyVar    10

#define criticalDistance     1.5
#define criticalDistanceD    1.5

#define birthRate1           0.015
#define birthRate2           0.020
#define introtimeDist1       650
#define introtimeDist2       651

#define motionBiasQ          true
#define varyStepSize         true


#endif /* Parameters_h */
