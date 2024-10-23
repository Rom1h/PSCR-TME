/*
 * Barrier.h
 *
 *  Created on: Oct 16, 2024
 *      Author: romain
 */
#pragma once
#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace pr{


class Barrier {
	mutable std::mutex m;
	int max;
	int curr;
	std::condition_variable cond;
public:
	Barrier(int n) : max(n), curr(0) {}
	void done(){
		std::unique_lock<std::mutex> l(m);
		curr++;
		if(curr == max){
			cond.notify_all();
		}
	}
	void waitFor(){
		std::unique_lock<std::mutex> l(m);
		while(curr < max){
			cond.wait(l);
		}
	}

	~Barrier() {
		// TODO Auto-generated destructor stub
	}
};

}
