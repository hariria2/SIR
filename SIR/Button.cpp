//
//  Button.cpp
//  SIR
//
//  Created by Sahand Hariri on 11/9/15.
//  Copyright © 2015 Sahand Hariri. All rights reserved.
//

#include "Button.hpp"
Button::Button(double x,double y, string label, Visualization* vis){
    setX(x);
    setY(y);
    setLabel(label);
    setVis(vis);
    drawButton();
    
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
void Button::setVis(Visualization* vis){
    _Vis = vis;
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
Visualization* Button::getVis(){
    return _Vis;
}
void Button::drawButton(){
    double RR, GG, BB;
    if (_Clicked){
        RR = 0.8;
        GG = 0.;
        BB = 0.;
    } else{
        RR = 0.0;
        GG = 0.0;
        BB = 0.6;
    }
    
    glBegin(GL_QUADS);
    glColor3f(RR,GG,BB); glVertex2d(_X, _Y);
    glColor3f(RR,GG,BB); glVertex2d(_X+_L, _Y);
    glColor3f(RR,GG,BB); glVertex2d(_X+_L, _Y+_W);
    glColor3f(RR,GG,BB); glVertex2d(_X, _Y+_W);
    glEnd();
    glColor3f(0.8,0.9,0.9);
    double tx = _Vis->WInvXTrsfrm(_X+_L/4.);
    double ty = _Vis->WInvYTrsfrm(_Y+_W/4.);
    _Vis->DrawText(_Label.data(),(int) _Label.size(), tx, ty,18);
}