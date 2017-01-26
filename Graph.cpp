#include "Graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cstdlib>

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

	ifstream plik("c4.txt");

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

bool Graph::sprawdzCzyGrafSpojny(vector<Edge> &ed, int x){

	bool *visited = new bool[vertexes.size()];
	for (int i = 0; i < vertexes.size(); i++) visited[i] = false;
	stack<int>stack;
	int vc = 0;

	//na stos wrzucamy pierwszy wierzcholek
	stack.push(0);
	//oznaczamy go jako odwiedzony
	visited[0] = true;

	//dopoki stos jest niepusty
	while (!stack.empty()) {

		//do v przypisujemy top
		int v = stack.top();
		//zdejmujemy top
		stack.pop();
		//zwiekszamy vc
		vc++;
		//dla kazdego sasiada v sprawdzamy czy jego sasiedzi sa odwiedzeni. Jesli nie to odwiedzamy ich
		//i wrzucamy na stos
		for (int u = 0; u < ed.size(); u++) {
			
			if (ed[u].getA() == v) {
				//krawedz jest przechowywana jakos dwie liczby: a i b i ten warunek sprawdza dla dwoch
				//mozliwosci: a-b lub b-a
				if (!visited[ed[u].getB()]){
					visited[ed[u].getB()] = true;
					stack.push(ed[u].getB());
				}
			} else if (ed[u].getB() == v) {
				if (!visited[ed[u].getA()]){
					visited[ed[u].getA()] = true;
					stack.push(ed[u].getA());
				}
			}
		}

	}
	
	delete [] visited;
	//jesli vc jest rowne wielkosci vectora wierzcholkow. Czyli sprawdzilismy kazdy wierzcholek co oznacza
	//ze istnieja sciezki z kazdego wierzcholka do innego kazdego czyli graf jest spojny.
	//x jest po to ze pozniej sprawdzamy mosty. I jesli jakis wierzcholek jest juz zerowego stopnia 
	//to nie beirzemy go pod uwage przy sprawdzaniu tego warunku. x to ilosc wierzcholkow zerowego stopnia
	return (vc == vertexes.size()-x) ? true : false;

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

	//for (int i = 0; i < edges.size(); i++)
		//if (sprawdzCzyKrawedzIstnieje(edges[i])) edges.erase(edges.begin()+i);
	
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
				if (!visited[edges[u].getA()]) {
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

bool Graph::sprawdzCzyDalejSpojny(Edge e, int x) {

	//tworzy pomocniczy vector, zapisuje do niego wszystkie krawedzie z edges, potem usuwa przekazana do
	//sprawdzenia krawedz i sprawdza czy ten pomocniczy vector z usunieta krawedzia jest dalej spojny
	//x po to jak w fcji sprawdzspojnosc() , jest to ilosc wierzcholkow zerowego stopnia, ktorychnie bierzemy
	//pod uwage przy sprawdzaniu spojnosci pozostalego grafu
	vector<Edge> tmpEdges;

	for (int i = 0; i < edges.size(); i++) 
		tmpEdges.push_back(edges[i]);

	for(int i = 0; i < tmpEdges.size(); i++)
		if (tmpEdges[i].getA() == e.getA() && tmpEdges[i].getB() == e.getB()) {
			tmpEdges.erase(tmpEdges.begin() + i);
			i = tmpEdges.size();
		}

	return sprawdzCzyGrafSpojny(tmpEdges, x) ? true : false;

}


void Graph::wyznaczCyklEulera() {
	/*
	if (sprawdzCzyGrafSpojny(edges, 0) && sprawdzCzyWszystkieKrawedzieParzystegoStopnia() && sprawdzCzyIstniejeCyklEulera()) {
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
	*/
}

void Graph::wyznaczCykl2() {
	int v = 0, l = 0, l2 = 0;

	if (sprawdzCzyGrafSpojny(edges, 0) && sprawdzCzyWszystkieKrawedzieParzystegoStopnia() && sprawdzCzyIstniejeCyklEulera()) {
		
		cout << endl << "Cykl Eulera: " << endl << endl << v;

		while (!edges.empty()) {
			//sprawdza ile jest wierzcholkow zerowego stopnia
			l = 0; l2 = 0;
			for (int i = 0; i < vertexes.size(); i++) {
				l = 0;
				for (int j = 0; j < edges.size(); j++) {
					if (edges[j].getA() == vertexes[i] || edges[j].getB() == vertexes[i]) l++;
				}
				if (l == 0) l2++;
			}

			//algorytm Fleuryego: jeœli jest jedna krawedz wychodzaca z wierzcholka
			//to usuwamy te krawedz i do v przypisujemy wierzcholek do ktorego
			//prowadzila ta krawedz, z neigo dalej bedziemy usuwac krawedzie
			if (zwrocLiczbeSasiadow(v) == 1) {

				auto it = find(edges.begin(), edges.end(), v);

				if (it->getA() == v) v = it->getB();
				else v = it->getA();

				edges.erase(edges.begin() + distance(edges.begin(), it));
			}
			//jesli jest wiecej krawedzi, to szukamy takiej ktora nie rozspojni grafu - mostu.
			//jesli znajdziemy, usuwamy i idziemy do wierzcholka do ktorego prowadzila ta krawedz 
			//(nasze nowe v)
			else {
				for (int u = 0; u < edges.size(); u++) {

					if (edges[u].getA() == v) {
						if (sprawdzCzyDalejSpojny(edges[u], l2)) {
							v = edges[u].getB();
							edges.erase(edges.begin() + u);
							u = edges.size();
						}
					}
					else if (edges[u].getB() == v) {
						if (sprawdzCzyDalejSpojny(edges[u], l2)) {
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
		if (!sprawdzCzyGrafSpojny(edges, 0)) {
			cout << "Graf nie jest spojny" << endl;
		}
		else if (!sprawdzCzyWszystkieKrawedzieParzystegoStopnia()) {
			cout << "Nie wszystkie wierzcholki sa parzystego stopnia" << endl;
		}
		else if (!sprawdzCzyIstniejeCyklEulera()) {
			cout << "Nie istnieje cykl w danym grafie" << endl;
		}
	}

}