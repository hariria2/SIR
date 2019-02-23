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
#define Version              "1"
#define isSingleLocation     true

// Storage and Visualization
#define StoreData            true
#define VisualizeData        false

// Global Timing
#define endTime              43300
#define timeStep             1
#define timeStepVis          0.1

// Means
#define ageMean              30
#define susceptibilityMean   3.1
#define initialConditionMean 2
#define betaMean             2
#define deltaMean            0.1
#define PMean                2
#define CMean                1
#define sociabilityMean      0
#define stepSizeMean         3.5
#define stepSizeMeanVis      0.3
#define lifeExpectencyMean   82


#define ageVar               0
#define susceptibilityVar    0
#define initialConditionVar  0
#define betaVar              0
#define deltaVar             0
#define PVar                 0
#define CVar                 0
#define sociabilityVar       0
#define stepSizeVar          0
#define lifeExpectencyVar    0

#define criticalDistance     4
#define criticalDistanceD    4

#define birthRate1           0.02  // 0.015
#define birthRate2           0.02  //0.02
#define introtimeDist1       650
#define introtimeDist2       651

#define motionBiasQ          false
#define varyStepSize         false


#endif /* Parameters_h */
