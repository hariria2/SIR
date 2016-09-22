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
#define isSingleLocation     false

// Storage and Visualization
#define StoreData            true
#define VisualizeData        false

// Global Timing
#define endTime              72000
#define timeStep             1
#define timeStepVis          0.1

// Means
#define ageMean              30
#define susceptibilityMean   3
#define initialConditionMean 2
#define betaMean             .5
#define deltaMean            1.01
#define PMean                1.2
#define CMean                1
#define sociabilityMean      2
#define stepSizeMean         1
#define lifeExpectencyMean   75

// Variances
#define ageVar               10
#define susceptibilityVar    1
#define initialConditionVar  1
#define betaVar              0.002
#define deltaVar             0.002
#define PVar                 0.1
#define CVar                 0.1
#define sociabilityVar       4
#define stepSizeVar          0.2
#define lifeExpectencyVar    10


#endif /* Parameters_h */
