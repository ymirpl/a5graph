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
		
		Node(double xx, double yy) : x(xx), y(yy) {}
		
		double distance(const Node k) const{  // odleglosc euklidesowa
			 return sqrt((x-k.x)*(x-k.x)+(y-k.y)*(y-k.y));
		}
};


// Sciezka  w grafie
class path {
public:
		double lenght;
		deque<int> route;

		void print() {
			int first, last;
			first = route[0];
			last = route[route.size()-1];
			
			if (lenght == INF) { // nie ma sciezki
				cout << first << "  " << last << std::endl << first << " " << last << std::endl;
				return;
			}
			
			cout << first << " " << last << " " << lenght << std::endl; // dlugosc
			cout << first << " " << last;
			
			for (int i=0; i < route.size(); ++i)
				cout << " " << route[i];
			
			cout << std::endl;
			
		}
};

///Klasa reprezentująca graf

class graph{
	private:
		vector<Node> positions;
		vector<map<int, double> > connected;
	public:
		void addNode(double x, double y){  //wstawianie wierzchołków
			positions.push_back(Node(x,y));
			connected.push_back(map<int, double>()); //powiększenie tablicy list krawędzi
		}
		
		void addEdge(int f, int s, double d){  //dodawanie krwędzi
			connected[f][s]=d;
		}
		
		double remedge (int f, int s){  //usuwanie krawędzi, zwraca długość(uwaga! - brak kontroli czy krawędź istnieje!)
			double temp=connected[f][s];
			connected[f].erase(s);
			return temp;
		}
	
		double dist(int f, int s){		//długość krawędzi pomiędzy wierzchołkami
			if (connected[f].count(s))return positions[f].distance(positions[s]);
			return INF;
		}
	
		double wdist(int f, int s){		//długość wagowa krawędzi pomiędzy wierzchołkami
			if (connected[f].count(s))return connected[f][s];
			return INF;
		}
			
		path findPath(int f, int s){	//znajdowanie najkrótszej ścieżki
			path r;		//zwracana ścieżka
			int size=positions.size();
			int precursor[size];	//poprzednicy
			double lenght[size];	//długość ścieżki
			
			set<int> temporary;		//zbiór roboczy
			set<int> points;		//zbior wierzchołków
			
			int u;					//obsługiwany wierzchołek
			
						
			for (int i=0; i<size; ++i)lenght[i]=INF;
			lenght[f]=0;
			
			for (int i=0; i<size; ++i)points.insert(i);  //wypełnianie zbioru wierzchołków
			
			while(!points.empty()){
				u=*points.begin();
				for(set<int>::iterator i=points.begin(); i!=points.end(); ++i)if(lenght[*i]<lenght[u])u = *i;
				temporary.insert(u);
				points.erase(u);		//przełożenie najbliższego elementu zbioru wierzchołków do zbioru roboczego
				
				
				if(u==s)break;
				
				for (int i=0; i<size; ++i){
					if(lenght[i]>(lenght[u]+dist(u,i))){  
						lenght[i]=lenght[u]+dist(u,i);  //ew. poprawienie min odległości
						precursor[i]=u;						//ustawienie nowego poprzednika
					}
				}
				if(u==s)break;
			}
			
			r.lenght = lenght[s];
			if (r.lenght==INF){r.route.push_front(s); r.route.push_front(f); return r;}
			r.route.push_front(u=s);
			while(u!=f){
				r.route.push_front(precursor[u]);
				u=precursor[u];
			}
	
			return r;
		}
		
		path findSecondPath(int f, int s, path first) const{  //znajdowanie drugiej ścieżki nie zawierającej first
			path second, help;
			graph temp=*this;
			double rem;
			
			rem=temp.remedge(first.route[0], first.route[1]);
			second=temp.findPath(f, s);
			
			for(int i=1; i<first.route.size()-1; ++i){
				temp.addEdge(first.route[i-1], first.route[i], rem);
				rem=temp.remedge(first.route[i], first.route[i+1]);
				help=temp.findPath(f, s);
				if(help.lenght<second.lenght)second=help;
			}
			
			return second;
		}
		
		path findPathW(int f, int s){	//znajdowanie najkrótszej ścieżki
			path r;		//zwracana ścieżka
			int size=positions.size();
			int precursor[size];	//poprzednicy
			double lenght[size];	//długość ścieżki
			
			set<int> temporary;		//zbiór roboczy
			set<int> points;		//zbior wierzchołków
			
			int u;					//obsługiwany wierzchołek
			
						
			for (int i=0; i<size; ++i)lenght[i]=INF;
			lenght[f]=0;
			
			for (int i=0; i<size; ++i)points.insert(i);  //wypełnianie zbioru wierzchołków
			
			while(!points.empty()){
				u=*points.begin();
				for(set<int>::iterator i=points.begin(); i!=points.end(); ++i)if(lenght[*i]<lenght[u])u = *i;
				temporary.insert(u);
				points.erase(u);		//przełożenie najbliższego elementu zbioru wierzchołków do zbioru roboczego
				
				
				if(u==s)break;
				
				for (int i=0; i<size; ++i){
					if(lenght[i]>(lenght[u]+wdist(u,i))){  
						lenght[i]=lenght[u]+wdist(u,i);  //ew. poprawienie min odległości
						precursor[i]=u;						//ustawienie nowego poprzednika
					}
				}
			}
			
			r.lenght = lenght[s];
			if (r.lenght==INF){r.route.push_front(s); r.route.push_front(f); return r;}
			r.route.push_front(u=s);
			while(u!=f){
				r.route.push_front(precursor[u]);
				u=precursor[u];
			}
			
			return r;
		}
		
		path findSecondPathW(int f, int s, path first) const{  //znajdowanie drugiej ścieżki nie zawierającej first
			path second, help;
			graph temp=*this;
			double rem;
			
			rem=temp.remedge(first.route[0], first.route[1]);
			second=temp.findPathW(f, s);
			
			for(int i=1; i<first.route.size()-1; ++i){
				temp.addEdge(first.route[i-1], first.route[i], rem);
				rem=temp.remedge(first.route[i], first.route[i+1]);
				help=temp.findPathW(f, s);
				if(help.lenght<second.lenght)second=help;
			}
			
			return second;
		}			
};



int main(){	
		char buffer[64];	// bufor na wczytywane linie
		graph Graph;			
		vector<int> start, end;  //wezly poczatkowe i koncowe sciezek
		path first, second;		// szukane sciezki
		
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
