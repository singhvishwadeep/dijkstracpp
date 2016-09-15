#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

// class for particular city
class city {
	public:
		string name;
		map <string,int> next;
		// constructor
		city (string n):name(n) {}
		~city () {
			next.clear();
			name.clear();
		}
		void insertin (string n, int dist) {
			next.insert(make_pair(n,dist));
		}
		vector <string> shownext () {
			vector<string> n;
			//cout << "for node " << name << " -> " ;
			for (map<string,int>::iterator it = next.begin(); it != next.end(); it++) {
				n.push_back(it->first);
				//cout << it->first << " ";
			}
			//cout << endl;
			return n;
		}
		void showallnext () {
			cout << "for node " << name << " -> " << endl;
			for (map<string,int>::iterator it = next.begin(); it != next.end(); it++) {
				cout << it->first << " " << it->second << endl;
			}
			return;
		}
		int ndistance (string n) {
			map<string, int>::iterator mit = next.find(n);
			if (mit != next.end()) {
				return mit->second;
			}
			return -1;
		}
};

// unique set of all cities
set<string>cities;
map<string,string> connect;
map<string,city*> wholemap;


void dijkstra (string start, string end) {
	/*
	for (mit = wholemap.begin(); mit != wholemap.end(); mit++) {
		mit->second->showallnext();
	}
	cout << "all cities -> ";
	for (set<string>::iterator it = cities.begin(); it != cities.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	*/
	
	set<string> visited;
	set<string>further;
	map<string,string> previous;
	map<string,int> shortdist;
	
	further.insert(start);
	shortdist[start] = 0;
	previous[start] = "";
	
	
	while (1) {
		if (further.empty()) {
			break;
		}
		/*
		cout << "all further -> ";
		for (set<string>::iterator it = further.begin(); it != further.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
		*/
		set<string>tempfurther;
		while (!further.empty()) {
			string running = *further.begin();
			city *obj = wholemap[running];
			vector<string> mynext = obj->shownext();
			for (vector<string>::iterator iti = mynext.begin(); iti != mynext.end(); iti++) {
				string nextname = *iti;
				//cout << *iti << " recieved " << endl;
				
				string pair = running;
				pair.append(",");
				pair.append(nextname);
				set<string>::iterator itpp = visited.find(pair);
				if (itpp != visited.end()) {
					continue;
				}
				// fetching the distance
				int currdis = obj->ndistance(nextname);
				if (currdis == -1) {
					continue;
				}
				// marked visited
				visited.insert(pair);
				tempfurther.insert(nextname);
				map<string, int>::iterator mit1 = shortdist.find(nextname);
				if (mit1 == shortdist.end()) {
					// visiting first time
					shortdist[nextname] = currdis;
					previous[nextname] = running;
				} else {
					// visited previously
					if (shortdist[nextname] > shortdist[running]+currdis) {
						shortdist[nextname] = currdis;
						previous[nextname] = running;
					}
				}
			}
			further.erase(further.begin());
		}
		/*
		cout << "all tempfurther -> ";
		for (set<string>::iterator it = tempfurther.begin(); it != tempfurther.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
		*/
		further = tempfurther;
	}
	
	vector <string> path;
	map<string,string>::iterator fit;
	path.push_back(end);
		
	fit = previous.find(end);
		
	while (fit != previous.end()) {
		path.push_back(fit->second);
		if (start.compare(fit->second) == 0) {
			break;
		}
		fit = previous.find(fit->second);
	}
	reverse(path.begin(),path.end());
	int sdist = 0;
	for (vector<string>::iterator iti = path.begin(); iti != path.end(); iti++) {
		sdist = sdist + shortdist[*iti];
	}
	cout << "PATH " << start << " -> " << end << " (" << sdist << "): ";
	for (vector<string>::iterator iti = path.begin(); iti != path.end(); iti++) {
		cout << *iti << " -> ";
	}
	cout << "END" << endl;
}

void dijkstraclear () {
	while (!cities.empty()) {
		cities.erase(cities.begin());
	}
	connect.clear();
	for (map<string,city *>::iterator it = wholemap.begin(); it != wholemap.end(); it++) {
		city *p = it->second;
		delete p;
	}
	wholemap.clear();
}

int main (int argc, char *argv[]) {
	if (argc < 4 || argc > 5) {
		cout << argv[0] << "<mapfile> <start> <end> ?<bothways path 0/1>?" << endl;
		exit(1);
	}
	int rev = 0;
	if (argc == 5) {
		if (!strcmp(argv[4],"1")) {
			rev = 1;
		}
	}
	ifstream in(argv[1], ios::in);
	if (!in.is_open()) {
		cout << "File cannot be opened" << endl;
		exit(1);
	}
	string start = argv[2];
	string end = argv[3];
	map<string, city*>::iterator mit;
	stringstream ss;
	string loc1, loc2, str;
	int dist;
	while (!in.eof()) {
		getline(in,str);
		ss.str(str);
		ss >> loc1 >> loc2 >> dist;
		if (ss.fail()) {
			ss.clear();
			continue;
		}
		ss.clear();
		// cout << str << endl;
		// set of cities
		cities.insert(loc1);
		cities.insert(loc2);
		// saving connectivity
		connect[loc1] = loc2;
		
		// making object
		mit = wholemap.find(loc1);
		if (mit != wholemap.end()) {
			city *obj = wholemap[loc1];
			obj->insertin(loc2,dist);
		} else {
			city *obj = new city(loc1);
			wholemap[loc1] = obj;
			obj->insertin(loc2,dist);
		}
		mit = wholemap.find(loc2);
		if (mit == wholemap.end()) {
			city *obj = new city(loc2);
			wholemap[loc2] = obj;
		}
		if (rev) {
			// reverse path adding
			// making object
			mit = wholemap.find(loc2);
			if (mit != wholemap.end()) {
				city *obj = wholemap[loc2];
				obj->insertin(loc1,dist);
			} else {
				city *obj = new city(loc2);
				wholemap[loc2] = obj;
				obj->insertin(loc1,dist);
			}
			mit = wholemap.find(loc1);
			if (mit == wholemap.end()) {
				city *obj = new city(loc1);
				wholemap[loc1] = obj;
			}
		}
	}
	in.close();
	
	dijkstra(start, end);
	dijkstraclear();
	return 0;
}
