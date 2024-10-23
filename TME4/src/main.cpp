#include "Banque.h"
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <random>
using namespace std;
using namespace pr;
const int NB_THREAD = 10;
const int NB_COMPTES =10;
const int NB_ITERATION =100000;
int threadRandom(int min,int max){
	static thread_local std::mt19937 gen;
	std::uniform_int_distribution<int> d(min,max);
	return d(gen);

}
void banquier(Banque &b){
	for(int k = 0 ;k<NB_ITERATION;k++){
		b.lock();
		b.comptabiliser(1000*NB_COMPTES);
		b.unlock();
	}

}
void incremente(Banque &b ){
	for(int k = 0 ;k<NB_ITERATION;k++){
		b.lock();
		int i = threadRandom(1,NB_COMPTES-1);
		int j = threadRandom(1,NB_COMPTES-1);
		int montant = threadRandom(1,100);
		Compte & crediteur = b.comptes[j];
		b.transfert(i,j,montant);
		crediteur.unlock();
		b.unlock();

	}
}
int main () {
	vector<thread> threads;
	Banque b(NB_COMPTES,1000);
	threads.reserve(NB_THREAD);
	// TODO : creer des threads qui font ce qui est demandé
	for(int i = 0 ; i<NB_THREAD;++i){
		threads.emplace_back(incremente,ref(b));
		cout<<"threand : "<<i<<endl;
	}

	threads.emplace_back(banquier,ref(b));
	for (auto & t : threads) {
		t.join();
	}


	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
