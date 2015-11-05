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
    Visualization(int x, int y, bool fsq);
    void setArchitect(Architect* archie);
    void setWindow();
    void setX(int x);
    void setY(int y);
    void setXRedFctr(double xredfctr);
    void setYRedFctr(double yredfctr);
    void setPeople(vector<Person*> person);
    void setPlaces(vector<Place*> places);
    void setMouseX(float x);
    void setMouseY(float y);
    
    
    Architect* getArchitect();
    float getMouseX();
    float getMouseY();
    double getXRedFactr();
    double getYRedFactr();
    GLFWwindow* getWindow();
    
    vector<Person*> getPeople();
    vector<Place*> getPlaces();
    
    
    
    void Init();
    void Render();
    void RenderSplash();
    
    void DrawPeople();
    void DrawPlace();
    void DrawTestPoint(float x, float y);
    void DrawText(const char *text, int length, int x, int y, int fsize);
    void DrawLabel();
    void DrawBarGraph(double x0, double y0, double val, string c);
    
    //void DrawPlot();
    
    float TTransform(double t);
    float XTransform(double x);
    float YTransform(double y);
    float InvXTrsfrm(double x);
    float InvYTrsfrm(double y);
    
    float WXTransform(double x);
    float WYTransform(double y);
    float WInvXTrsfrm(double x);
    float WInvYTrsfrm(double y);
    
    float PTransform(double y);
    void AddPerson(Person* p);

    void testPrint();
    
    void PlotSIR();
    
private:
    
    Architect* _Architect;
    
    GLFWwindow* _window;
    const GLFWvidmode* mode;
    
    bool _FullScreenQ = true;
    
    double _CurrentTime;
    int _X;
    int _Y;
    double _XRedFctr;
    double _YRedFctr;
    
    float _MouseX;
    float _MouseY;
    
    vector<Person*> _People;
    vector<Place*>  _Places;
    vector<double> _TT;
    vector<double> _SS;
    vector<double> _II;
    vector<double> _RR;
    
    
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};

static Visualization* _visualization = NULL;
Visualization* getVisualization(int x, int y, bool fsq);

#endif /* Visualization_h */
