#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <limits>

#define INF numeric_limits<double>::infinity()

using namespace std;

class Node {
	public:
		double x;
		double y;
		
		Node(double xx, double yy, int i) : x(xx), y(yy) {}
		
		double computeWeight(const Node k) const{  // odleglosc euklidesowa
			 return sqrt((x-k.x)*(x-k.x)+(y-k.y)*(y-k.y));
		}
};


// Sciezka  w grafie
class Path {
public:
		double lenght;
		deque<int> route;

		void print() {
			int first, last;
			first = route[0];
			last = route[route.size()-1];
			
			if (lenght == INF) { // nie ma sciezki
				cout << first << " " << last << std::endl << first << " " << last << std::endl;
				return;
			}
			
			cout.precision(4); // do 4 miejsc po przecinku
			cout.setf(ios::fixed); 
			
			cout << first << " " << last << " " << lenght << std::endl; // dlugosc
			cout << first << " " << last;
			
			//@TODO make it iterator and pop_front
			for (int i=0; i < route.size(); ++i)
				cout << " " << route[i];
			
			cout << std::endl;
			
		}
};

///Klasa reprezentująca graf

class Graph {
	private:
		vector<Node> nodes; // no, wiadomo
		vector<map<int, double> > adjacent; // ktory wierzcholek do jakiego z jaka waga

	
	public:
		void addNode(double x, double y) {  
			nodes.push_back(Node(x,y,adjacent.size()));
			adjacent.resize(adjacent.size() + 1);
		}
		
		void addEdge(int f, int s, double w) {  
			adjacent[f][s] = w;
		}
		
		/**
		 * @return dlugosc usuwanej
		 */
		double rmEdge (int f, int s) { 
			double ret = adjacent[f][s];
			adjacent[f].erase(s);
			return ret;
		}
	
		double dist(int f, int s) {		
			if (adjacent[f].count(s))
				return nodes[f].computeWeight(nodes[s]); // obliczamy odleglosc
			return INF;
		}
	
		double weight(int f, int s) {		
			if (adjacent[f].count(s)) // istnieje
				return adjacent[f][s];
			return INF;
		}
		
		
		
		Path findPath(int f, int s, bool W = false)  {	
			
			int size = nodes.size();
			int precursor[size];	//poprzednicy
			double lenght[size];	//odleglosci do punktow
			set<int> Q;		//kopiec
			Path path;		//to zwracac bedziem
			int node;					// bierzacy node
			double l; // zmienna pomocnicza do odleglosci
			
						
			for (int i=0; i<size; ++i)
				lenght[i]=INF; // wszedzie daleko
			lenght[f]=0; // ale nie do poczatku
			
			for (int i = 0; i < size; ++i)
				Q.insert(i);  // wszyscy do kopca
			
			while(!Q.empty()) {
				
				set<int>::iterator seti;
				seti = Q.begin();
				node = *seti;
				
				// niesamowicie durnowate extract_min
				for(; seti != Q.end(); ++seti)
					if(lenght[*seti] < lenght[node])
						node = *seti;
				
				
				Q.erase(node);	
				
				
				if(node == s)
					break;  // juz koniec
				
				// tu nalezaloby tylko sasiadow spr, ale coz...
				for (int i = 0; i < size; ++i) {
					
					if(W) //  tu sie roznicuje, jak liczymy
						l = lenght[node] + weight(node, i);
					else
						l = lenght[node] + dist(node, i);
					
					if(lenght[i] > l) {  
						lenght[i] = l;  
						precursor[i]=node;						
					}
				}
				
			}
			
			path.lenght = lenght[s];
			
			if (path.lenght == INF) { // niet sciezki 
				path.route.push_front(s); 
				path.route.push_front(f); 
				return path;
			}
			
			node = s; // zaczynamy sciezke od konca
			path.route.push_front(node);
			
			while(node != f) {
				node = precursor[node];
				path.route.push_front(node);
			}
	
			return path;
		}
		
		Path findSecondPath(int f, int s, Path shortets, bool W = false)  {  
	
			Graph temp = *this; // coby nie poposuc grafu
			Path path, candidate; // zwracana i pomocnicza sciezka
			double removedWeight;
			
			removedWeight = temp.rmEdge(shortets.route[0], shortets.route[1]); // po kolei usuwamy kr z pierwszej sciezki
			path = temp.findPath(f, s, W);
			
			for(int i = 1; i < shortets.route.size() - 1; ++i) {
				
				temp.addEdge(shortets.route[i-1], shortets.route[i], removedWeight);
				removedWeight=temp.rmEdge(shortets.route[i], shortets.route[i+1]);
				
				candidate = temp.findPath(f, s, W);
				
				if(candidate.lenght < path.lenght)
					path = candidate;
			}
			
			return path;
		}
		
		Path findPathW(int f, int s) {	// najkrotsza po wagach
			return findPath(f,s,true);
		}
		
		Path findSecondPathW(int f, int s, Path first) {  
			return findSecondPath(f,s,first,true);
		}			
};



int main(){	
		char buffer[64];	// bufor na wczytywane linie
		Graph Graph;			
		vector<int> start, end;  //wezly poczatkowe i koncowe sciezek
		Path first, second;		// szukane sciezki
		
		int ret=0;				
		double a=0, b=0, c=0;
		bool token = true;
		
		
		while(true){
			cin.getline(buffer, sizeof(buffer));
			ret = sscanf(buffer, "%lf %lf %lf", &a, &b, &c); // czytamy lilijke
			
			// debug, co wczytano
			std::cerr << "Read: " << a << " " << b << " " << c << std::endl; 
			
			if (ret == EOF) // koniec pliku
				break;
			if (ret == 2 && token) // wezly
				Graph.addNode(a, b);
			if (ret == 3) { // krawedzie
				Graph.addEdge(static_cast<int>(a), static_cast<int>(b), c); 
				token = false;
			}
			if (ret == 2 && !token) { // sciezki do szukania
				start.push_back(static_cast<int>(a)); 
				end.push_back(static_cast<int>(b));
			}
		}
		
		for (int i=0; i<start.size(); ++i) {
			first = Graph.findPath(start[i], end[i]);
			first.print();

			second = Graph.findSecondPath(start[i], end[i], first);
			second.print();
			
			first = Graph.findPathW(start[i], end[i]);
			first.print();
			
			second = Graph.findSecondPathW(start[i], end[i], first);
			second.print();
		}
		
			
return 0;
}
