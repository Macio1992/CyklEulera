#include <iostream>
#include "Graph.h"

using namespace std;

int main() {

	Graph graph;

	graph.uzupelnijVector();
	graph.wypisz();
	

	graph.wyznaczCykl2();

	system("PAUSE");
	return 0;
}