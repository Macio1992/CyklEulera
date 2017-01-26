#ifndef Graph_H
#define Graph_H

#include "Edge.h"
#include <vector>
#include <stack>

using namespace std;

class Graph
{
public:
	Graph();
	~Graph();
	void uzupelnijVector();
	bool sprawdzCzyKrawedzIstnieje(Edge);
	void wypisz();
	int zwrocIndeksWierzcholka(Edge);
	bool sprawdzCzyGrafSpojny(vector<Edge> &);
	bool sprawdzCzyWszystkieKrawedzieParzystegoStopnia();
	bool sprawdzCzyIstniejeCyklEulera();
	void wyznaczCyklEulera();
	bool sprawdzCzyDalejSpojny(Edge);
	int zwrocLiczbeSasiadow(int);
	void wyznaczCykl2();

private:
	vector<Edge> edges;
	vector<int> vertexes;

};

#endif // !Graph_H
