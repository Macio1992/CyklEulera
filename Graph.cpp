#include "Graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

Graph::Graph(){}

Graph::~Graph(){}

bool Graph::sprawdzCzyKrawedzIstnieje(Edge e) {

	auto it = find(edges.begin(), edges.end(), Edge(e.getA(), e.getB()));

	return (it != edges.end()) ? true : false;

}

int Graph::zwrocIndeksWierzcholka(Edge e) {

	auto it = find(edges.begin(), edges.end(), Edge(e.getA(), e.getB()));

	return distance(edges.begin(), it);

}

void Graph::uzupelnijVector() {

	ifstream plik("graf2.txt");

	if (!plik) {
		cout << "Blad podczas otwierania pliku" << endl;
		exit(0);
	}

	int a, b;
	while (plik >> a >> b) {

		Edge e(a,b);

		edges.push_back(e);

		vector<int>::iterator it = find(vertexes.begin(), vertexes.end(), a);

		if (it == vertexes.end())
			vertexes.push_back(a);

		it = find(vertexes.begin(), vertexes.end(), b);

		if (it == vertexes.end())
			vertexes.push_back(b);

	}

	sort(vertexes.begin(), vertexes.end());

}

void Graph::wypisz() {

	cout << "Krawedzie grafu: " << endl;

	for (int i = 0; i < edges.size(); i++)
		cout << edges[i].toString();

}

bool Graph::sprawdzCzyGrafSpojny(vector<Edge> &ed){

	bool *visited = new bool[vertexes.size()];
	for (int i = 0; i < vertexes.size(); i++) visited[i] = false;
	stack<int>stack;
	int vc = 0;

	stack.push(0);
	visited[0] = true;

	while (!stack.empty()) {

		int v = stack.top();
		stack.pop();
		vc++;
		for (int u = 0; u < ed.size(); u++) {
			
			if (ed[u].getA() == v) {
				if (visited[ed[u].getB()]) u++;
				else {
					visited[ed[u].getB()] = true;
					stack.push(ed[u].getB());
				}
			} else if (ed[u].getB() == v) {
				if (visited[ed[u].getA()]) u++;
				else {
					visited[ed[u].getA()] = true;
					stack.push(ed[u].getA());
				}
			}
		}

	}
	
	delete [] visited;
	return (vc == vertexes.size()) ? true : false;

}

bool Graph::sprawdzCzyWszystkieKrawedzieParzystegoStopnia() {

	int licz = 0, liczpar = 0;

	for (int i = 0; i < vertexes.size(); i++) {
		for (int j = 0; j < edges.size(); j++) {
			if (edges[j].getA() == vertexes[i] || edges[j].getB() == vertexes[i]) licz++;
		}
		if (licz % 2 == 0)liczpar++;
	}
	
	return (liczpar == vertexes.size()) ? true : false;
}

bool Graph::sprawdzCzyIstniejeCyklEulera() {

	bool *visited = new bool[vertexes.size()];
	for (int i = 0; i < vertexes.size(); i++) visited[i] = false;

	for (int i = 0; i < edges.size(); i++)
		if (sprawdzCzyKrawedzIstnieje(edges[i])) edges.erase(edges.begin()+i);
	
	stack<int> stack;
	int n0 = 0, nc = 0, v = 0;
	stack.push(0);
	visited[0] = true;
	
	while (!stack.empty()) {
		v = stack.top();
		stack.pop();
		nc = 0;
		for (int u = 0; u < edges.size(); u++) {
			if (edges[u].getA() == v) {
				if (!visited[edges[u].getB()]) {
					visited[edges[u].getB()] = true;
					stack.push(edges[u].getB());
				}
			}
			else if (edges[u].getB() == v) {
				if (visited[edges[u].getA()]) {
					visited[edges[u].getA()] = true;
					stack.push(edges[u].getA());
				}
			}
		}
	}

	delete[] visited;

	return (n0 == 0) ? true : false;
}

int Graph::zwrocLiczbeSasiadow(int v) {

	int l = 0;

	for (int i = 0; i < edges.size(); i++)
		if (edges[i].getA() == v || edges[i].getB() == v)
			l++;
	
	return l;
}

bool Graph::sprawdzCzyDalejSpojny(Edge e) {

	vector<Edge> tmpEdges;

	for (int i = 0; i < edges.size(); i++) {
		if (!(edges[i] == e)) {
			tmpEdges.push_back(edges[i]);
		}
	}

	return sprawdzCzyGrafSpojny(tmpEdges) ? true : false;

}


void Graph::wyznaczCyklEulera() {

	if (sprawdzCzyGrafSpojny(edges) && sprawdzCzyWszystkieKrawedzieParzystegoStopnia() && sprawdzCzyIstniejeCyklEulera()) {
		int v = 0;

		cout << endl << "Cykl Eulera: " << endl << endl << v;

		while (!edges.empty()) {
			if (zwrocLiczbeSasiadow(v) == 1) {
				auto it = find(edges.begin(), edges.end(), v);

				if (it->getA() == v) v = it->getB();
				else v = it->getA();

				edges.erase(edges.begin() + distance(edges.begin(), it));
			}
			else {
				for (int u = 0; u < edges.size(); u++) {
					if (edges[u].getA() == v) {
						if (sprawdzCzyDalejSpojny(edges[u])) {
							v = edges[u].getB();
							edges.erase(edges.begin() + u);
							u = edges.size();
						}
					}
					else if (edges[u].getB() == v) {
						if (sprawdzCzyDalejSpojny(edges[u])) {
							v = edges[u].getA();
							edges.erase(edges.begin() + u);
							u = edges.size();
						}
					}
				}
			}
			cout << " -> " << v;
		}
		cout << endl << endl;
	}
	else {
		cout << "Nieprawidlowy graf" << endl;
		if (!sprawdzCzyGrafSpojny(edges)) {
			cout << "Graf nie jest spojny" << endl;
		}
		else if (!sprawdzCzyWszystkieKrawedzieParzystegoStopnia()) {
			cout << "Nie wszystkie wierzcholki sa parzystego stopnia" << endl;
		}
		else if(!sprawdzCzyIstniejeCyklEulera()){
			cout << "Nie istnieje cykl w danym grafie" << endl;
		}
	}

}

void Graph::wyznaczCykl2() {
	int v = 0;
	if (zwrocLiczbeSasiadow(v) == 1) {
		auto it = find(edges.begin(), edges.end(), v);

		if (it->getA() == v) v = it->getB();
		else v = it->getA();

		edges.erase(edges.begin() + distance(edges.begin(), it));
	}
	else {
		
		for (int u = 0; u < edges.size(); u++) {
			if (edges[u].getA() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getB();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
			else if (edges[u].getB() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getA();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
		}
	}
	
	wypisz();
	cout << "nowe v: " << v << endl;

	if (zwrocLiczbeSasiadow(v) == 1) {
		auto it = find(edges.begin(), edges.end(), v);

		if (it->getA() == v) v = it->getB();
		else v = it->getA();

		edges.erase(edges.begin() + distance(edges.begin(), it));
	}
	else {

		for (int u = 0; u < edges.size(); u++) {
			if (edges[u].getA() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getB();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
			else if (edges[u].getB() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getA();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
		}
	}

	wypisz();
	cout << "nowe v: " << v << endl;

	if (zwrocLiczbeSasiadow(v) == 1) {
		auto it = find(edges.begin(), edges.end(), v);

		if (it->getA() == v) v = it->getB();
		else v = it->getA();

		edges.erase(edges.begin() + distance(edges.begin(), it));
	}
	else {

		for (int u = 0; u < edges.size(); u++) {
			if (edges[u].getA() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getB();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
			else if (edges[u].getB() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getA();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
		}
	}

	wypisz();
	cout << "nowe v: " << v << endl;

	if (zwrocLiczbeSasiadow(v) == 1) {
		auto it = find(edges.begin(), edges.end(), v);

		if (it->getA() == v) v = it->getB();
		else v = it->getA();

		edges.erase(edges.begin() + distance(edges.begin(), it));
	}
	else {

		for (int u = 0; u < edges.size(); u++) {
			if (edges[u].getA() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getB();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
			else if (edges[u].getB() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getA();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
			}
		}
	}

	wypisz();
	cout << "nowe v: " << v << endl;

	if (zwrocLiczbeSasiadow(v) == 1) {
		auto it = find(edges.begin(), edges.end(), v);

		if (it->getA() == v) v = it->getB();
		else v = it->getA();

		edges.erase(edges.begin() + distance(edges.begin(), it));
	}
	else {

		for (int u = 0; u < edges.size(); u++) {
			if (edges[u].getA() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getB();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
				else {
					cout << "a rozspojnia" << endl;
				}
			}
			else if (edges[u].getB() == v) {
				if (sprawdzCzyDalejSpojny(edges[u])) {
					v = edges[u].getA();
					edges.erase(edges.begin() + u);
					u = edges.size();
				}
				else {
					cout << "b rozspojnia" << endl;
				}
			}
		}
	}

	wypisz();
	cout << "nowe v: " << v << endl;

}