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
#define Version              "3"
#define isSingleLocation     true

// Storage and Visualization
#define StoreData            true
#define VisualizeData        false

// Global Timing
#define endTime              36000
#define timeStep             1
#define timeStepVis          0.1

// Means
#define ageMean              40
#define susceptibilityMean   3.5
#define initialConditionMean 2
#define betaMean             0.1
#define deltaMean            0.1
#define PMean                0.4
#define CMean                0.5
#define sociabilityMean      0
#define stepSizeMean         50
#define lifeExpectencyMean   80

// Variances
#define ageVar               25
#define susceptibilityVar    0
#define initialConditionVar  0
#define betaVar              0.
#define deltaVar             0.
#define PVar                 0.
#define CVar                 0.
#define sociabilityVar       0
#define stepSizeVar          0.
#define lifeExpectencyVar    5


#endif /* Parameters_h */
