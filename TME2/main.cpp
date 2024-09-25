#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include "HashMap.hpp"
using namespace std;
using namespace std::chrono;

void print(vector<pair<int,string>> vec){
	for(pair<int,string> &x : vec){
		if(x.second=="war" || x.second == "toto"||x.second == "peace")
		cout<< "Mot : " << x.second << "Count : "<< x.first<<endl;
	}
}

int main () {


	ifstream input = ifstream("/home/romain/PSCR-TME/TME2/WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	vector<pair<int,string>> vec;
	HashMap<string,int> hMap(1000);
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		int* k =hMap.get(word);
		if(k!=nullptr){
			*k+=1;
		}
		else{
			++nombre_lu;
			hMap.put(word,1);
		}

		/*bool find = false;

		for(pair<int,string> &k: vec){
			if(k.second==word){
				k.first++;
				find=true;
				break;
			}
		}
		if(!find){
			pair<int,string> p1;
			p1.first = 1;
			p1.second = word;
			vec.push_back(p1);
		}*/


	}
	for (std::forward_list<HashMap<string,int>::Entry> &l : hMap.buckets) {
		for (HashMap<string,int>::Entry &e : l) {
			pair<int,string> p(e.val,e.key);
			vec.push_back(p);
		}
	}
	std::sort(vec.begin(), vec.end(), [] (const pair<int,string> & a, const pair<int,string> & b) { return a.first
	> b.first ;});
	for(int i = 0;i<10;i++){
		cout<<"Mot : "<<vec[i].second<<" Count : "<<vec[i].first<< endl;
	}
	input.close();
	print(vec);
	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    return 0;
}


