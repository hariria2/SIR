//
//  Visualization.h
//  SIR
//
//  Created by Sahand Hariri on 10/14/15.
//  Copyright © 2015 Sahand Hariri. All rights reserved.
//

#ifndef Visualization_h
#define Visualization_h

#include <stdio.h>

#include <stdio.h>
#include <iostream>
#include <vector>
//#include <algorithm>

#include <GLFW/glfw3.h>
#include "Person.h"
#include "Place.h"

using namespace std;

class Visualization{
public:
    Visualization(int x, int y);
    
    void setWindow();
    void setX(int x);
    void setY(int y);
    void setPeople(vector<Person*> person);
    void setPlaces(vector<Place*> places);
    
    
    
    void Init();
    void Render();
    
    void DrawPeople();
    void DrawPlace();
    void DrawTestPoint(float x, float y);
    
    GLFWwindow* getWindow();
    
    vector<Person*> getPeople();
    vector<Place*> getPlaces();
    
private:
    
    GLFWwindow* _window;
    
    int _X;
    int _Y;
    
    vector<Person*> _People;
    vector<Place*> _Places;
    
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};

static Visualization* _visualization = NULL;
Visualization* getVisualization(int x, int y);

#endif /* Visualization_h */
