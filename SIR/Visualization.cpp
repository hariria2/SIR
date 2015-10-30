//
//  Visualization.cpp
//  SIR
//
//  Created by Sahand Hariri on 10/14/15.
//  Copyright © 2015 Sahand Hariri. All rights reserved.
//

#include "Visualization.h"


Visualization::Visualization(int x, int y, bool fsq){
    
    setX(x);
    setY(y);
    setXRedFctr(0.88);
    setYRedFctr(0.9);
    _FullScreenQ = fsq;
}

void Visualization::setArchitect(Architect *archie){
    _Architect = archie;
}
void Visualization::setWindow(){
    
    
    if (_FullScreenQ){
        mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        _window =glfwCreateWindow(mode->width, mode->height, "DiseaseVille", glfwGetPrimaryMonitor(), NULL);
        //setX(mode->width);
        //setY(mode->height);
        setX(700);
        setY(700);
        
    } else{
        _window = glfwCreateWindow(_X, _Y, "Awesomeness", NULL, NULL);
    }
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
void Visualization::setXRedFctr(double xredfctr){
    _XRedFctr = xredfctr;
}
void Visualization::setYRedFctr(double yredfctr){
    _YRedFctr = yredfctr;
}
void Visualization::setMouseX(float x){
    _MouseX = x;
}
void Visualization::setMouseY(float y){
    _MouseY = y;
}

void Visualization::setPeople(vector<Person*> ppl){
    _People = ppl;
}
void Visualization::setPlaces(vector<Place*> places){
    for(auto p = places.cbegin(); p != places.cend(); ++p){
        _Places.push_back(*p);
    }
}


Architect* Visualization::getArchitect(){
    return _Architect;
}
float Visualization::getMouseX(){
    return _MouseX;
}
float Visualization::getMouseY(){
    return _MouseY;
}
double Visualization::getXRedFactr(){
    return _XRedFctr;
}
double Visualization::getYRedFactr(){
    return _YRedFctr;
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
    cout << width << endl;
    cout << height << endl;
    glViewport(0, 0, width, height);
    glfwSetKeyCallback(_window, key_callback);
    glfwSetInputMode(_window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    glfwSetCursorPosCallback(_window, cursor_pos_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
}
void Visualization::Render(){
    glPointSize(10.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.3, 0.); glVertex3f(XTransform(0), YTransform(0), 0.0);
    glColor3f(0.0, 0.3, 0.); glVertex3f(XTransform(_X), YTransform(0), 0.0);
    glColor3f(0.0, 0.3, 0.); glVertex3f(XTransform(_X), YTransform(_Y), 0.0);
    glColor3f(0.0, 0.3, 0.); glVertex3f(XTransform(0), YTransform(_Y), 0.0);
    glEnd();
    
    
    DrawPlace();
    DrawPeople();
    DrawLabel();
    
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Visualization::RenderSplash(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.0f, 0.9f);
    string text = "Welcome to DiseaseVille.";
    glColor3f(1, 1, 0);
    DrawText(text.data(), (int) text.size(), _X/2-100, _Y/2, 24);
    text = "Written by Sahand";
    DrawText(text.data(), (int) text.size(), _X/2+100-100, _Y/2-20, 10);
    glfwSwapBuffers(_window);
    glfwPollEvents();
    sleep(4);
}

void Visualization::DrawPlace(){
    float x1, x2;
    float y1, y2;
    float RR, GG, BB;
    for(auto p = _Places.cbegin(); p != _Places.cend(); ++p){
        
        x1 = XTransform((*p)->Perimeter[0][0]);
        y1 = YTransform((*p)->Perimeter[1][0]);
        x2 = XTransform((*p)->Perimeter[0][1]);
        y2 = YTransform((*p)->Perimeter[1][1]);
        
        
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
        glColor3f(RR, GG, BB); glVertex3f(x2, y1, 0.0);
        glColor3f(RR, GG, BB); glVertex3f(x2, y2, 0.0);
        glColor3f(RR, GG, BB); glVertex3f(x1, y2, 0.0);
        glEnd();
    }
}
void Visualization::DrawPeople(){
    glBegin(GL_POINTS);
    float X;
    float Y;
    float Ic = 0.0;
    float Sc = 0.0;
    float RR, GG, BB;
    for(auto p = _People.cbegin(); p != _People.cend(); ++p){
        X = XTransform(((*p)->getCoordinates())[0]);
        Y = YTransform(((*p)->getCoordinates())[1]);
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
        
        glColor3f(RR, GG, BB); glVertex3f(X,Y, 0.);
    }
    glEnd();
}
void Visualization::DrawTestPoint(float x, float y){
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(2*x/_X - 1, 2*y/_Y - 1, 0.);
    glEnd();
};
void Visualization::DrawText(const char *text, int length, int x, int y, int fsize){
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, _X, 0, _Y, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x,y);
    for (int i=0; i<length; i++){
        if (fsize == 10){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, (int)text[i]);
        } else if (fsize == 12){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)text[i]);
        }else if (fsize == 18){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
        }else if (fsize == 24){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
        }else{
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)text[i]);
        }
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
    
}
void Visualization::DrawLabel(){
    char buffer [50];
    double leftedge = _XRedFctr*_X + 5;
    double rightedge = _XRedFctr*_X + 60;
    string tim = "Time: ";
    string sus = "Susceptible: ";
    string exp = "Exposed: ";
    string inf = "Infected: ";
    string rec = "Recovered: ";
    string ded = "Dead: ";
    
    sprintf(buffer,"%2.2f", _Architect->getDailyTime());
    string timVal = buffer;
    string susVal = to_string(_Architect->getS());
    string expVal = to_string(_Architect->getI());
    string infVal = to_string(_Architect->getP());
    string recVal = to_string(_Architect->getR());
    string dedVal = to_string(_Architect->getD());
    
    glColor3f(1, 0.7, 0);
    
    
    DrawText(tim.data(), (int) tim.size(), leftedge, 680, 18);
    DrawText(sus.data(), (int) sus.size(), leftedge, 660, 18);
    DrawText(exp.data(), (int) exp.size(), leftedge, 640, 18);
    DrawText(inf.data(), (int) inf.size(), leftedge, 620, 18);
    DrawText(rec.data(), (int) rec.size(), leftedge, 600, 18);
    DrawText(ded.data(), (int) ded.size(), leftedge, 580, 18);
    
    DrawText(timVal.data(), (int) timVal.size(), rightedge, 680, 18);
    DrawText(susVal.data(), (int) susVal.size(), rightedge, 660, 18);
    DrawText(expVal.data(), (int) expVal.size(), rightedge, 640, 18);
    DrawText(infVal.data(), (int) infVal.size(), rightedge, 620, 18);
    DrawText(recVal.data(), (int) recVal.size(), rightedge, 600, 18);
    DrawText(dedVal.data(), (int) dedVal.size(), rightedge, 580, 18);
    
    DrawBarGraph(_X + 25, 400, _Architect->getS(), "S");
    DrawBarGraph(_X + 45, 400, _Architect->getI(), "I");
    DrawBarGraph(_X + 65, 400, _Architect->getR(), "R");
    
}

void Visualization::DrawBarGraph(double x0, double y0, double val, string c){
    
    float x1 = XTransform(x0);
    float x2 = XTransform(x0 + 7);
    float y1 = YTransform(y0);
    float y2 = YTransform(y0 + 15*val/70);
    
    float RR, GG, BB;
    
    if (c == "S"){
        RR = 0; GG = 0; BB = 1;
    } else if (c == "I"){
        RR = 1; GG = 0; BB = 0;
    } else if (c == "R"){
        RR = 0; GG = 1; BB = 0;
    }
    
    glBegin(GL_POLYGON);
    glColor3f(1.f, 0.8f, 0.8f); glVertex3f(x1, y1, 0.0f);
    glColor3f(1.f, 0.8f, 0.8f); glVertex3f(x2, y1, 0.0f);
    glColor3f(RR, GG, BB);      glVertex3f(x2, y2, 0.0f);
    glColor3f(RR, GG, BB);      glVertex3f(x1, y2, 0.0f);
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(1.f, 0.8f, 0.8f); glVertex3f(x1-0.01, y1, 0.0f);
    glColor3f(1.f, 0.8f, 0.8f); glVertex3f(x2+0.01, y1, 0.0f);
    glColor3f(1.f, 0.8f, 0.8f); glVertex3f(x2+0.01, y1+0.01, 0.0f);
    glColor3f(1.f, 0.8f, 0.8f); glVertex3f(x1-0.01, y1+0.01, 0.0f);
    glEnd();
    
    glColor3f(RR, GG, BB);
    DrawText(c.data(), (int) c.size(), InvXTrsfrm((x1+x2)/2) , InvYTrsfrm(y1+y1), 18);
    
    }




float Visualization::XTransform(double x){
    double trans = (1-_XRedFctr);
    return _XRedFctr*(2.*x/_X - 1)-trans;
}
float Visualization::YTransform(double y){
    double trans = (1-_YRedFctr);
    return _YRedFctr*(2.*y/_Y - 1)+trans;
}
float Visualization::InvXTrsfrm(double x){
    double trans = (1-_XRedFctr);
    return _X*((x+trans)/_XRedFctr + 1)/2;
}
float Visualization::InvYTrsfrm(double y){
    double trans = (1-_YRedFctr);
    return _Y*((y+trans)/_YRedFctr - 1)/2;
}



void Visualization::testPrint(){
    cout << "=====>THIS IS A TEST<=====" << endl;
}



//==========> Call Backs ===============//

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
    _visualization->setMouseX(xpos);
    _visualization->setMouseY(ypos);
    
    cout << "X: " << xpos << ", " << "Y: " << ypos << endl;
}
void Visualization::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        _visualization->DrawTestPoint(_visualization->getMouseX(),_visualization->getMouseY());
        _visualization->testPrint();
        cout << _visualization->getMouseX() << endl;
        
    }
}

Visualization* getVisualization(int x, int y, bool fsq) {
    if (_visualization==NULL){
        _visualization = new Visualization(x,y, fsq);
    }
    return _visualization;
}