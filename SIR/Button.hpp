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

using namespace std;

class Button{
public:
    Button();
    
    ~Button();
    
    void setX(double x);
    void setY(double y);
    void setL(double l);
    void setW(double w);
    void setLabel(string label);
    void setClicked(bool clicked);
    
    double getX();
    double getY();
    double getL();
    double getW();
    string getLabel();
    bool getClicked();
    
private:
    
    double _X;
    double _Y;
    double _L;
    double _W;
    string _Label;
    
    bool _Clicked = false;
    
    
    
};

#endif /* Button_hpp */
