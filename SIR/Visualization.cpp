//
//  Visualization.cpp
//  SIR
//
//  Created by Sahand Hariri on 10/14/15.
//  Copyright © 2015 Sahand Hariri. All rights reserved.
//

#include "Visualization.h"

using namespace std;


Visualization::Visualization(int x, int y){
    setX(x);
    setY(y);
}

void Visualization::setWindow(){
    _window = glfwCreateWindow(_X, _Y, "My Awesome Real-Time OpenGL Animation", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
}
void Visualization::setX(int x){
    _X = x;
}
void Visualization::setY(int y){
    _Y = y;
}

void Visualization::setPeople(vector<Person*> ppl){
    _People = ppl;
}
void Visualization::setPlaces(vector<Place*> places){
    for(auto p = places.cbegin(); p != places.cend(); ++p){
        _Places.push_back(*p);
    }
}


GLFWwindow* Visualization::getWindow(){
    return _window;
}

vector<Person*> Visualization::getPeople(){
    return _People;
}
vector<Place*> Visualization::getPlaces(){
    return _Places;
}


void Visualization::Init(){
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    setWindow();
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
    
    
    int width, height;
    glEnable(GL_POINT_SMOOTH);
    glfwGetFramebufferSize(_window, &width, &height);
    glViewport(0, 0, width, height);
    
    glfwSetKeyCallback(_window, key_callback);
    glfwSetInputMode(_window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    glfwSetCursorPosCallback(_window, cursor_pos_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
}
void Visualization::Render(){
    glPointSize(10.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.3f, 0.0f, 0.9f);
    
    DrawPlace();
    DrawPeople();
    //DrawTestPoint(400, 400);
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Visualization::DrawPlace(){
    
    float x1, x2, x3, x4;
    float y1, y2, y3, y4;
    float RR, GG, BB;
    for(auto p = _Places.cbegin(); p != _Places.cend(); ++p){
        x1 = (*p)->Perimeter[0][0]*2./700.-1;
        y1 = (*p)->Perimeter[1][0]*2./700.-1;
        x2 = (*p)->Perimeter[0][1]*2./700.-1;
        y2 = (*p)->Perimeter[1][0]*2./700.-1;
        x3 = (*p)->Perimeter[0][1]*2./700.-1;
        y3 = (*p)->Perimeter[1][1]*2./700.-1;
        x4 = (*p)->Perimeter[0][0]*2./700.-1;
        y4 = (*p)->Perimeter[1][1]*2./700.-1;
        
        
        if ((*p)->getType()=="Home"){
            RR = 0.5;
            GG = 0.6;
            BB = 0.7;
        } else if ((*p)->getType()=="School"){
            RR = 0.7;
            GG = 0.6;
            BB = 0.5;
        } else if ((*p)->getType()=="Work"){
            RR = 0.6;
            GG = 0.5;
            BB = 0.7;
        } else if ((*p)->getType()=="Cemetery"){
            RR = 0.7;
            GG = 0.5;
            BB = 0.6;
        } else {
            RR = 1.;
            GG = 1.;
            BB = 1.;
        }
        
        glBegin(GL_POLYGON);
        glColor3f(RR, GG, BB); glVertex3f(x1, y1, 0.0);
        glColor3f(RR, GG, BB); glVertex3f(x2, y2, 0.0);
        glColor3f(RR, GG, BB); glVertex3f(x3, y3, 0.0);
        glColor3f(RR, GG, BB); glVertex3f(x4, y4, 0.0);
        glEnd();
    }
}
void Visualization::DrawPeople(){
    glBegin(GL_POINTS);
    double X;
    double Y;
    float Ic = 0.0;
    float Sc = 0.0;
    float RR, GG, BB;
    for(auto p = _People.cbegin(); p != _People.cend(); ++p){
        X = ((*p)->getCoordinates())[0];
        Y = ((*p)->getCoordinates())[1];
        Ic = ((*p)->getInHostDynamics()).getI();
        Sc = ((*p)->getInHostDynamics()).getT();
        if (Ic >=1){
            Ic = 1;
        }
        if (Sc >=1){
            Sc = 1;
        }
        if ((*p)->getState()=='D'){
            RR=0;
            GG=0;
            BB=0;
        }
        else {
            RR = Ic;
            GG = 1-max(Ic,Sc);
            BB = Sc;
        }
        glColor3f(RR, GG, BB); glVertex3f(2.*X/700 - 1, 2.*Y/700. - 1, 0.);
    }
    glEnd();
}
void Visualization::DrawTestPoint(float x, float y){
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(2*x/700 - 1, 2*y/900. - 1, 0.);
    glEnd();
};

void Visualization::error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
void Visualization::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
void Visualization::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    cout << "X: " << xpos << ", " << "Y: " << ypos << endl;
}
void Visualization::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Visualization* vis = getVisualization(700, 700);
        vis->DrawTestPoint(400,400);
    }
}

Visualization* getVisualization(int x, int y) {
    if (_visualization==NULL){
        _visualization = new Visualization(x,y);
    }
    return _visualization;
}