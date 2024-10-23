/*
 * Barrier.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: romain
 */

#include "Barrier.h"
#include <mutex>
namespace pr{
// Définition du constructeur ici
Barrier::Barrier(int n) : max(n), curr(0) {}
void Barrier::done(){
	std::unique_lock<std::mutex> l(m);
	curr++;
	if(curr == max){
		cond.notify_all();
	}
}
void Barrier::waitFor(){
	std::unique_lock<std::mutex> l(m);
	while(curr < max){
		cond.wait(l);
	}
}

Barrier::~Barrier() {
	// TODO Auto-generated destructor stub
}
}

