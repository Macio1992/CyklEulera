#include "Edge.h"

#include <iostream>
#include <sstream>

using namespace std;

Edge::Edge(int aa, int bb) { 
	a = aa;
	b = bb;
}

Edge::~Edge(){}

bool Edge::operator==(Edge e) {
	return (a == e.getB() && b == e.getA()) ? true : false;
}

bool Edge::operator==(int v) {
	return (a == v || b == v);
}

int Edge::getA() {
	return a;
}

void Edge::setA(int aa) {
	a = aa;
}

int Edge::getB() {
	return b;
}

void Edge::setB(int bb) {
	b = bb;
}

string Edge::toString() {

	stringstream stm;

	stm << a << " - " << b << endl;

	return stm.str();
}