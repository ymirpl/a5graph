#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <limits>

#define _INF numeric_limits<double>::infinity()

using namespace std;

///Klasa reprezentująca wierzchołek grafu

class point{
	public:
		double x;
		double y;
		
		point(double xx, double yy){
			x=xx; y=yy;
		}
		
		double dist(const point k) const{  //odległość pomiędzy punktami na płaszczyźnie
			 return sqrt((x-k.x)*(x-k.x)+(y-k.y)*(y-k.y));
		}
};

///Klasa reprezentująca ścieżkę w grafie

struct path{
		double lenght;
		deque<int> route;
	
		void print(){
			int f, s;
			f=route[0];
			s=route[route.size()-1];
			if(lenght==_INF){cout<<f<<" "<<s<<"\n"<<f<<" "<<s<<"\n"; return;}
			cout<<f<<" "<<s<<" "<<lenght<<"\n";
			cout<<f<<" "<<s;
			for (int i=0; i<route.size(); ++i)cout<<" "<<route[i];
			cout<<"\n";
			return;
		}
};

///Klasa reprezentująca graf

class graph{
	private:
		vector<point> positions;
		vector<map<int, double> > connected;
	public:
		void addpoint(double x, double y){  //wstawianie wierzchołków
			positions.push_back(point(x,y));
			connected.push_back(map<int, double>()); //powiększenie tablicy list krawędzi
		}
		
		void addedge(int f, int s, double d){  //dodawanie krwędzi
			connected[f][s]=d;
		}
		
		double remedge (int f, int s){  //usuwanie krawędzi, zwraca długość(uwaga! - brak kontroli czy krawędź istnieje!)
			double temp=connected[f][s];
			connected[f].erase(s);
			return temp;
		}
	
		double dist(int f, int s){		//długość krawędzi pomiędzy wierzchołkami
			if (connected[f].count(s))return positions[f].dist(positions[s]);
			return _INF;
		}
	
		double wdist(int f, int s){		//długość wagowa krawędzi pomiędzy wierzchołkami
			if (connected[f].count(s))return connected[f][s];
			return _INF;
		}
			
		path dijkstra(int f, int s){	//znajdowanie najkrótszej ścieżki
			path r;		//zwracana ścieżka
			int size=positions.size();
			int precursor[size];	//poprzednicy
			double lenght[size];	//długość ścieżki
			
			set<int> temporary;		//zbiór roboczy
			set<int> points;		//zbior wierzchołków
			
			int u;					//obsługiwany wierzchołek
			
						
			for (int i=0; i<size; ++i)lenght[i]=_INF;
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
			if (r.lenght==_INF){r.route.push_front(s); r.route.push_front(f); return r;}
			r.route.push_front(u=s);
			while(u!=f){
				r.route.push_front(precursor[u]);
				u=precursor[u];
			}
	
			return r;
		}
		
		path findsecond(int f, int s, path first) const{  //znajdowanie drugiej ścieżki nie zawierającej first
			path second, help;
			graph temp=*this;
			double rem;
			
			rem=temp.remedge(first.route[0], first.route[1]);
			second=temp.dijkstra(f, s);
			
			for(int i=1; i<first.route.size()-1; ++i){
				temp.addedge(first.route[i-1], first.route[i], rem);
				rem=temp.remedge(first.route[i], first.route[i+1]);
				help=temp.dijkstra(f, s);
				if(help.lenght<second.lenght)second=help;
			}
			
			return second;
		}
		
		path wdijkstra(int f, int s){	//znajdowanie najkrótszej ścieżki
			path r;		//zwracana ścieżka
			int size=positions.size();
			int precursor[size];	//poprzednicy
			double lenght[size];	//długość ścieżki
			
			set<int> temporary;		//zbiór roboczy
			set<int> points;		//zbior wierzchołków
			
			int u;					//obsługiwany wierzchołek
			
						
			for (int i=0; i<size; ++i)lenght[i]=_INF;
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
			if (r.lenght==_INF){r.route.push_front(s); r.route.push_front(f); return r;}
			r.route.push_front(u=s);
			while(u!=f){
				r.route.push_front(precursor[u]);
				u=precursor[u];
			}
			
			return r;
		}
		
		path wfindsecond(int f, int s, path first) const{  //znajdowanie drugiej ścieżki nie zawierającej first
			path second, help;
			graph temp=*this;
			double rem;
			
			rem=temp.remedge(first.route[0], first.route[1]);
			second=temp.wdijkstra(f, s);
			
			for(int i=1; i<first.route.size()-1; ++i){
				temp.addedge(first.route[i-1], first.route[i], rem);
				rem=temp.remedge(first.route[i], first.route[i+1]);
				help=temp.wdijkstra(f, s);
				if(help.lenght<second.lenght)second=help;
			}
			
			return second;
		}			
};



int main(){	
		char buffer[64];
		vector<int> begin, end;  //początki i końce znajdowanych ścieżek
		path one, two;			//ścieżki
		graph main;				//graf
		int n=0;				
		double x=0, y=0, z=0;
		bool state=true;
		
		
		while(true){
			cin.getline(buffer, sizeof(buffer));
			n=sscanf(buffer, "%lf %lf %lf", &x, &y, &z);
			
			if(n<2)break;
			if(n==2 && state)main.addpoint(x, y);
			if(n==3){main.addedge(static_cast<int>(x), static_cast<int>(y), z); state=false;}
			if(n==2 && !state){begin.push_back(static_cast<int>(x)); end.push_back(static_cast<int>(y));}
		}
		
		for(int i=0; i<begin.size(); ++i){
			one=main.dijkstra(begin[i], end[i]);
			one.print();
			two=main.findsecond(begin[i], end[i], one);
			two.print();
			one=main.wdijkstra(begin[i], end[i]);
			two=main.wfindsecond(begin[i], end[i], one);
			one.print();
			two.print();
		}
		
			
return 0;
}