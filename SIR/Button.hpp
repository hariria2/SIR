//
//  Button.hpp
//  SIR
//
//  Created by Sahand Hariri on 11/9/15.
//  Copyright © 2015 Sahand Hariri. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

#include <stdio.h>
#include <string>
#include "Visualization.h"

using namespace std;

class Visualization;

class Button{
public:
    Button(double x, double y, string label, Visualization* vis);
    
    ~Button();
    
    void setX(double x);
    void setY(double y);
    void setL(double l);
    void setW(double w);
    void setLabel(string label);
    void setClicked(bool clicked);
    void setVis(Visualization* vis);
    
    double getX();
    double getY();
    double getL();
    double getW();
    string getLabel();
    bool getClicked();
    Visualization* getVis();
    
    void drawButton();
private:
    
    double _X;
    double _Y;
    double _L=0.1;
    double _W=0.05;
    string _Label;
    
    bool _Clicked = false;
    
    Visualization* _Vis;
    
    
};

#endif /* Button_hpp */
