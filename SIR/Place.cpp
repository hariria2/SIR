/*
 * Place.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: Sahand
 */

#include "Place.h"

using namespace std;


Place::Place(int id, string name, string type, double perimeter[2][2], Domain location, int pop):
    _Location(location), _TotalPopulation(pop)
{
	setID(id);
	setName(name);
	setType(type);
	setPerimeter(perimeter);
    _InfectionRadius = 40;
}// end constructor

// setters
void Place::setID(int id){
	_ID = id;
}
void Place::setName(string name){
	_Name = name;
}
void Place::setType(string type){
	_Type = type;
}
void Place::setPerimeter(double perimeter[2][2]){
	for (int ii=0; ii<2; ii++){
		for (int jj=0; jj<2; jj++){
			Perimeter[ii][jj] = perimeter[ii][jj];
		}
	}
}
void Place::setInfectionRadius(int r){
    _InfectionRadius = r;
}
void Place::setTotalPopulation(int pop){
    _TotalPopulation = pop;
}

void Place::setCoordinates(vector<vector<double>> coo){
	for (auto s=coo.begin(); s<coo.end(); s++){
		_Coordinates.push_back(*s);
	}
}

void Place::setSides(){
	vector<double> side;
	vector<double> co1;
	vector<double> co2;
	long size = _Coordinates.size();

	for (int i=0; i<size-1; i++){
		side.clear();
		co1 = _Coordinates[i];
		co2 = _Coordinates[i+1];
		for (auto t=co1.begin(); t<co1.end(); t++){
			side.push_back(*t);
		}
		for (auto t=co2.begin(); t<co2.end(); t++){
			side.push_back(*t);
		}
		_Sides.push_back(side);
	}

	side.clear();
	side.push_back(_Coordinates.back()[0]);
	side.push_back(_Coordinates.back()[1]);
	side.push_back(_Coordinates.front()[0]);
	side.push_back(_Coordinates.front()[1]);

	_Sides.push_back(side);
}

void Place::setDistanceMatrix(){
    double x1, x2, y1, y2;
    int id1, id2;
    //string key1, key2;
    double val;
    for (auto p1=_Occupants.cbegin(); p1 != _Occupants.cend(); ++p1) {
        auto p2 = p1; ++p2;
        for (; p2 != _Occupants.cend();++p2){
            id1 = (*p1)->getID();
            id2 = (*p1)->getID();
            x1 = ((*p1)->getCoordinates())[0];
            y1 = ((*p1)->getCoordinates())[1];
            x2 = ((*p2)->getCoordinates())[0];
            y2 = ((*p2)->getCoordinates())[1];
            //key1 = to_string(id1)+to_string(id2);
            //key2 = to_string(id1)+to_string(id2);
            val = Distance(x1,y1,x2,y2);
            _DistanceMatrix[id1][id2] = val;
            _DistanceMatrix[id2][id1] = val;
        }
    }
}

void Place::addPerson(Person* p){
	(_Occupants).push_back(p);
}

void Place::removePerson(Person* p){
	_Occupants.remove(p);
}

// getters
int Place::getID(){
	return _ID;
}
int Place::getTotalPopulation(){
    return _TotalPopulation;
}
string Place::getName(){
	return _Name;
}
string Place::getType(){
	return _Type;
}
string Place::getLocation(){
	return _Location.getName();
}

int Place::getInfectionRadius(){
    return _InfectionRadius;
}
list<Person*>* Place::getOccupants(){
    return &_Occupants;
}
vector<vector<double>> Place::getCoordinates(){
	return _Coordinates;
}

vector<vector<double>> Place::getSides(){
	return _Sides;
}

double Place::Distance(double x1, double y1, double x2, double y2){
    return sqrt(pow((x2-x1),2) + pow((y2 - y1),2));
}


int Place::areIntersecting(
										float v1x1, float v1y1, float v1x2, float v1y2,
										float v2x1, float v2y1, float v2x2, float v2y2
										) {
	float d1, d2;
	float a1, a2, b1, b2, c1, c2;

	// Convert vector 1 to a line (line 1) of infinite length.
	// We want the line in linear equation standard form: A*x + B*y + C = 0
	// See: http://en.wikipedia.org/wiki/Linear_equation
	a1 = v1y2 - v1y1;
	b1 = v1x1 - v1x2;
	c1 = (v1x2 * v1y1) - (v1x1 * v1y2);

	// Every point (x,y), that solves the equation above, is on the line,
	// every point that does not solve it, is not. The equation will have a
	// positive result if it is on one side of the line and a negative one
	// if is on the other side of it. We insert (x1,y1) and (x2,y2) of vector
	// 2 into the equation above.
	d1 = (a1 * v2x1) + (b1 * v2y1) + c1;
	d2 = (a1 * v2x2) + (b1 * v2y2) + c1;

	// If d1 and d2 both have the same sign, they are both on the same side
	// of our line 1 and in that case no intersection is possible. Careful,
	// 0 is a special case, that's why we don't test ">=" and "<=",
	// but "<" and ">".
	if (d1 > 0 && d2 > 0) return 0;
	if (d1 < 0 && d2 < 0) return 0;

	// The fact that vector 2 intersected the infinite line 1 above doesn't
	// mean it also intersects the vector 1. Vector 1 is only a subset of that
	// infinite line 1, so it may have intersected that line before the vector
	// started or after it ended. To know for sure, we have to repeat the
	// the same test the other way round. We start by calculating the
	// infinite line 2 in linear equation standard form.
	a2 = v2y2 - v2y1;
	b2 = v2x1 - v2x2;
	c2 = (v2x2 * v2y1) - (v2x1 * v2y2);

	// Calculate d1 and d2 again, this time using points of vector 1.
	d1 = (a2 * v1x1) + (b2 * v1y1) + c2;
	d2 = (a2 * v1x2) + (b2 * v1y2) + c2;

	// Again, if both have the same sign (and neither one is 0),
	// no intersection is possible.
	if (d1 > 0 && d2 > 0) return 0;
	if (d1 < 0 && d2 < 0) return 0;

	// If we get here, only two possibilities are left. Either the two
	// vectors intersect in exactly one point or they are collinear, which
	// means they intersect in any number of points from zero to infinite.
	if ((a1 * b2) - (a2 * b1) == 0.0f) return 2;

	// If they are not collinear, they must intersect in exactly one point.
	return 1;
}

int Place::ContainsQ(double x, double y){
	int intersections = 0;
	int intQ;

	for (int side = 0; side < _Sides.size(); side++) {
		intQ = areIntersecting(0, 0, x, y, _Sides[side][0], _Sides[side][1], _Sides[side][2], _Sides[side][3]);
		if (intQ){
			intersections++;
		}
	}
	if ((intersections & 1) == 1) {
		return 1;
	} else {
		return 0;
	}
}

Place::~Place(){
}
