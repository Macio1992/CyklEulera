#ifndef Edge_H
#define Edge_H

#include <iostream>

using namespace std;

class Edge
{
public:
	Edge(int, int);
	~Edge();
	bool operator==(Edge);
	bool operator==(int);
	int getA();
	void setA(int);
	int getB();
	void setB(int);
	string toString();

private:
	int a;
	int b;
};


#endif // !Edge_H
