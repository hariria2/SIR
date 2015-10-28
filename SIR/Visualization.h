//
//  Visualization.h
//  SIR
//
//  Created by Sahand Hariri on 10/14/15.
//  Copyright © 2015 Sahand Hariri. All rights reserved.
//

#ifndef Visualization_h
#define Visualization_h

#include <GLFW/glfw3.h>
#include <GLUT/GLUT.h>

#include "Architect.h"

class Architect;

class Visualization{

public:
    Visualization(int x, int y);
    void setArchitect(Architect* archie);
    void setWindow();
    void setX(int x);
    void setY(int y);
    void setPeople(vector<Person*> person);
    void setPlaces(vector<Place*> places);
    void setMouseX(float x);
    void setMouseY(float y);
    
    
    Architect* getArchitect();
    float getMouseX();
    float getMouseY();
    
    
    
    void Init();
    void Render();
    void RenderSplash();
    
    void DrawPeople();
    void DrawPlace();
    void DrawTestPoint(float x, float y);
    void DrawText(const char *text, int length, int x, int y, int fsize);
    void DrawLabel();
    
    GLFWwindow* getWindow();
    
    vector<Person*> getPeople();
    vector<Place*> getPlaces();
    
    void testPrint();
    
private:
    
    Architect* _Architect;
    
    GLFWwindow* _window;
    const GLFWvidmode* mode;
    
    int _X;
    int _Y;
    
    float _MouseX;
    float _MouseY;
    
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
