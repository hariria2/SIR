//
//  Button.cpp
//  SIR
//
//  Created by Sahand Hariri on 11/9/15.
//  Copyright © 2015 Sahand Hariri. All rights reserved.
//

#include "Button.hpp"

Button::Button(){
    
}

Button::~Button(){
    
}

void Button::setX(double x){
    _X = x;
}
void Button::setY(double y){
    _Y = y;
}
void Button::setL(double l){
    _L = l;
}
void Button::setW(double w){
    _W = w;
}
void Button::setLabel(string label){
    _Label = label;
}
void Button::setClicked(bool clicked){
    _Clicked = clicked;
}

double Button::getX(){
    return _X;
}
double Button::getY(){
    return _Y;
}
double Button::getL(){
    return _L;
}
double Button::getW(){
    return _W;
}
string Button::getLabel(){
    return _Label;
}
bool Button::getClicked(){
    return _Clicked;
}
