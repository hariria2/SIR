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
#define Version              "2"
#define isSingleLocation     true

// Storage and Visualization
#define StoreData            true
#define VisualizeData        true

// Global Timing
#define endTime              8
#define timeStep             0.1
#define timeStepVis          0.1

// Means
#define ageMean              30
#define susceptibilityMean   3.1
#define initialConditionMean 3
#define betaMean             0.4
#define deltaMean            0.1
#define PMean                0.4
#define CMean                0.8
#define sociabilityMean      0
#define stepSizeMean         0.35
#define stepSizeMeanVis      0.35
#define lifeExpectencyMean   82


#define ageVar               0
#define susceptibilityVar    0.
#define initialConditionVar  0.
#define betaVar              0.0
#define deltaVar             0.0
#define PVar                 0
#define CVar                 0
#define sociabilityVar       0
#define stepSizeVar          0
#define lifeExpectencyVar    0

#define criticalDistance     0.1
#define criticalDistanceD    1.7

#define birthRate1           0.02  // 0.015
#define birthRate2           0.02  //0.02
#define introtimeDist1       650
#define introtimeDist2       652

#define motionBiasQ          false
#define varyStepSize         false


#endif /* Parameters_h */
