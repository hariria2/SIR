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
	setYRedFctr(0.85);
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
		setX(mode->width);
		setY(mode->height);
		
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
	_MouseX = InvXTrsfrm(WXTransform(x));
}
void Visualization::setMouseY(float y){
	_MouseY = InvYTrsfrm(WYTransform(_Y-y));
}

void Visualization::setPeople(list<Person*> ppl){
	_People = ppl;
	_InitialPopSize = _People.size();
	_TotalPopSize = _InitialPopSize;
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

list<Person*> Visualization::getPeople(){
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
	unsigned long S = _Architect->getS();
	unsigned long E = _Architect->getI();
	unsigned long I = _Architect->getP();
	unsigned long R = _Architect->getR();
	//unsigned long D = _Architect->getD();
	_TotalPopSize = S + E + I + R;
	// Update things
	_CurrentTime = _Architect->getCurrentTime();
	_TT.push_back(_CurrentTime);
	_SS.push_back(S);
	_II.push_back(E + I);
	_RR.push_back(R);
	
	
	double domx = (_Architect->getDomain())->Boundary[0][1];
	double domy = (_Architect->getDomain())->Boundary[1][1];
	
	glPointSize(8.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, .5f);
	
	glBegin(GL_POLYGON);
	glColor3f(0.6, 0.6, 0.8); glVertex3f(XTransform(0), YTransform(0), 0.0);
	glColor3f(0.3, 0.4, 0.8); glVertex3f(XTransform(domx), YTransform(0), 0.0);
	glColor3f(0.3, 0.2, 0.9); glVertex3f(XTransform(domx), YTransform(domy), 0.0);
	glColor3f(0.8, 0.8, 0.5); glVertex3f(XTransform(0), YTransform(domy), 0.0);
	glEnd();
	
	
	DrawPlace();
	DrawPeople();
	DrawLabel();
	
	PlotSIR();
	
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void Visualization::RenderSplash(){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.0f, 0.9f);
	string text = "Welcome to Diseaseville.";
	glColor3f(1, 1, 0);
	DrawText(text.data(), (int) text.size(), _X/2-100, _Y/2, 24);
	text = "Written by Sahand";
	DrawText(text.data(), (int) text.size(), _X/2+100-100, _Y/2-20, 10);
	glfwSwapBuffers(_window);
	glfwPollEvents();
	//sleep(4);
}

void Visualization::DrawPlace(){
	float x1, x2;
	float y1, y2;
	float RR, GG, BB;
	vector<vector<double>> co;
	float x, y;
	for(auto p = _Places.cbegin(); p != _Places.cend(); ++p){
		
		x1 = XTransform((*p)->Perimeter[0][0]);
		y1 = YTransform((*p)->Perimeter[1][0]);
		x2 = XTransform((*p)->Perimeter[0][1]);
		y2 = YTransform((*p)->Perimeter[1][1]);

		co = (*p)->getCoordinates();

		if ((*p)->getType()=="Island"){
			RR = 0.5;
			GG = 0.4;
			BB = 0.2;
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

		for (int i=0; i < co.size(); i++){
			x = XTransform((float)co[i][0]);
			y = YTransform((float)co[i][1]);
			glColor3f(RR, i-GG, BB); glVertex3f(x, y, 0.0);
		}

		//glColor3f(RR, GG, BB); glVertex3f(x1, y1, 0.0);
		//glColor3f(1-RR, 0.8-GG, BB); glVertex3f(x2, y1, 0.0);
		//glColor3f(1-RR, 0.8-GG, BB); glVertex3f(x2, y2, 0.0);
		//glColor3f(RR, GG, BB); glVertex3f(x1, y2, 0.0);
		glEnd();

		co.clear();


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
		double tol = ((*p)->getInHostDynamics()).getTol();
		Ic = (((*p)->getInHostDynamics()).getI());
		Sc = (((*p)->getInHostDynamics()).getT())/tol;
		
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
		else if ((*p)->getState()=='N'){
			RR=1;
			GG=1;
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
	delete matrix;
	
}
void Visualization::DrawLabel(){
	char buffer_t [50];
	double leftedge = _XRedFctr*_X + 5;
	double rightedge = _XRedFctr*_X + 120;
	string mnth = "Month: ";
	string day = "Day: ";
	string sus = "Susceptible: ";
	string exp = "Exposed: ";
	string inf = "Infected: ";
	string rec = "Recovered: ";
	string ded = "Dead: ";
	string tot = "Total: ";
	
	int mntht = _CurrentTime/30;
	sprintf(buffer_t,"%2.2f", _Architect->getMonthlyTime());
	string mnthVal = to_string(mntht);
	string dayVal = buffer_t;
	string susVal = to_string(_Architect->getS());
	string expVal = to_string(_Architect->getI());
	string infVal = to_string(_Architect->getP());
	string recVal = to_string(_Architect->getR());
	string dedVal = to_string(_Architect->getD());
	string totVal = to_string(_TotalPopSize);
	glColor3f(1, 0.7, 0);
	DrawText(mnth.data(), (int) mnth.size(), leftedge, _Y - 20, 18);
	DrawText(day.data(), (int) day.size(), leftedge, _Y - 40, 18);
	DrawText(sus.data(), (int) sus.size(), leftedge, _Y - 60, 18);
	DrawText(exp.data(), (int) exp.size(), leftedge, _Y - 80, 18);
	DrawText(inf.data(), (int) inf.size(), leftedge, _Y - 100, 18);
	DrawText(rec.data(), (int) rec.size(), leftedge, _Y - 120, 18);
	DrawText(ded.data(), (int) ded.size(), leftedge, _Y - 140, 18);
	DrawText(tot.data(), (int) tot.size(), leftedge, _Y - 160, 18);
	
	DrawText(mnthVal.data(), (int) mnthVal.size(), rightedge, _Y - 20, 18);
	DrawText(dayVal.data(), (int) dayVal.size(), rightedge, _Y - 40, 18);
	DrawText(susVal.data(), (int) susVal.size(), rightedge, _Y - 60, 18);
	DrawText(expVal.data(), (int) expVal.size(), rightedge, _Y - 80, 18);
	DrawText(infVal.data(), (int) infVal.size(), rightedge, _Y - 100, 18);
	DrawText(recVal.data(), (int) recVal.size(), rightedge, _Y - 120, 18);
	DrawText(dedVal.data(), (int) dedVal.size(), rightedge, _Y - 140, 18);
	DrawText(totVal.data(), (int) totVal.size(), rightedge, _Y - 160, 18);
	
	DrawBarGraph(leftedge + 45, _Y*0.05, _Architect->getS(), "S");
	DrawBarGraph(leftedge + 75, _Y*0.05, _Architect->getI()+_Architect->getP(), "I");
	DrawBarGraph(leftedge + 105, _Y*0.05, _Architect->getR(), "R");
	
}

void Visualization::DrawBarGraph(double x0, double y0, double val, string c){
	
	float x1 = WXTransform(x0);
	float x2 = WXTransform(x0 + 10);
	float y1 = WYTransform(y0);
	float y2 = WYTransform(y0 + 200*val/_Y);
	
	float RR, GG, BB;
	
	if (c == "S"){
		RR = 0; GG = 0; BB = 1;
	} else if (c == "I"){
		RR = 1; GG = 0; BB = 0;
	} else if (c == "R"){
		RR = 0; GG = 1; BB = 0;
	}
	
	glBegin(GL_POLYGON);
	glColor3f(1.f, 0.9f, 0.9f); glVertex3f(x1, y1, 0.0f);
	glColor3f(1.f, 0.9f, 0.9f); glVertex3f(x2, y1, 0.0f);
	glColor3f(RR, GG, BB);      glVertex3f(x2, y2, 0.0f);
	glColor3f(RR, GG, BB);      glVertex3f(x1, y2, 0.0f);
	glEnd();
	
	glBegin(GL_POLYGON);
	glColor4f(RR, GG, BB,0.2); glVertex3f(x1-0.01, y1, 0.0f);
	glColor4f(RR, GG, BB,0.2); glVertex3f(x2+0.01, y1, 0.0f);
	glColor4f(1.f, 0.9f, 0.9f,0.2); glVertex3f(x2+0.01, y1+0.01, 0.0f);
	glColor4f(1.f, 0.9f, 0.9f,0.2); glVertex3f(x1-0.01, y1+0.01, 0.0f);
	glEnd();
	
	glColor3f(RR, GG, BB);
	DrawText(c.data(), (int) c.size(), WInvXTrsfrm((x1+x2)/2)-5 , WInvYTrsfrm(y1)-20, 24);
}
void Visualization::PlotSIR(){
	double x;
	double y;
	glLineWidth(5);
	
	glBegin(GL_LINE_STRIP);
	for (int ii = 0; ii < _TT.size(); ii++){
		x = TTransform(_TT[ii]);
		y = PTransform(_SS[ii]);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(x,y, 0.0f);
	}
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	for (int ii = 0; ii < _TT.size(); ii++){
		x = TTransform(_TT[ii]);
		y = PTransform(_II[ii]);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(x,y, 0.0f);
	}
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	for (int ii = 0; ii < _TT.size(); ii++){
		x = TTransform(_TT[ii]);
		y = PTransform(_RR[ii]);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(x,y, 0.0f);
	}
	glEnd();
	
}


float Visualization::TTransform(double t){
	int scale = _CurrentTime/30+1;
	int tscale = scale*30;
	double domx = (_Architect->getDomain())->Boundary[0][1];
	return XTransform(t*domx/tscale);
}

float Visualization::XTransform(double x){
	double domx = (_Architect->getDomain())->Boundary[0][1];
	double trans = (1-_XRedFctr);
	return _XRedFctr*(2.*x/domx - 1)-trans;
}
float Visualization::YTransform(double y){
	double domy = (_Architect->getDomain())->Boundary[1][1];
	double trans = (1-_YRedFctr);
	return _YRedFctr*(2.*y/domy - 1)+trans;
}
float Visualization::InvXTrsfrm(double x){
	double domx = (_Architect->getDomain())->Boundary[0][1];
	double trans = (1-_XRedFctr);
	return domx*((x+trans)/_XRedFctr + 1)/2;
}
float Visualization::InvYTrsfrm(double y){
	double domy = (_Architect->getDomain())->Boundary[1][1];
	double trans = (1-_YRedFctr);
	return domy*((y-trans)/_YRedFctr + 1)/2;
}

float Visualization::WXTransform(double x){
	return 2.*x/_X - 1;
}
float Visualization::WYTransform(double y){
	return 2.*y/_Y - 1;
}
float Visualization::WInvXTrsfrm(double x){
	return _X*(x + 1)/2;
}
float Visualization::WInvYTrsfrm(double y){
	return _Y*(y+1)/2;
}

float Visualization::PTransform(double y){
	//double pop = (double) _InitialPopSize;
	if (_TotalPopSize > _InitialPopSize){
		return y*2*(1-_YRedFctr)/_TotalPopSize - 1;
	}else{
		return y*2*(1-_YRedFctr)/_InitialPopSize - 1;
	}
}

void Visualization::AddPerson(Person *p){
	_People.push_back(p);
}

void Visualization::removePerson(Person *p){
	_People.remove(p);
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
	
	//cout << "X: " << _visualization->getMouseX() << ", " << "Y: " << _visualization->getMouseY() << endl;
}
void Visualization::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//_visualization->DrawTestPoint(_visualization->getMouseX(),_visualization->getMouseY());
		//_visualization->testPrint();
		//cout << _visualization->getMouseX() << endl;
		(_visualization->_Architect)->AddPerson(_visualization->getMouseX(),_visualization->getMouseY());
		
	}
}

Visualization* getVisualization(int x, int y, bool fsq) {
	if (_visualization==NULL){
		_visualization = new Visualization(x,y, fsq);
	}
	return _visualization;
}