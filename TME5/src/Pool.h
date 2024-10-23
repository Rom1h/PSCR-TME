#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <atomic>
#include <functional>

using namespace std;
namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
	std::atomic<bool> isRunning;
public:
	// Fonction pour gérer le travail des threads (Question 12)
	void poolWorker(Queue<Job>& queue, std::atomic<bool>& isRunning) {
	    while (isRunning) {
	        Job* j = queue.pop();
	        if (j) {
	            j->run(); // Exécute le job
	            delete j; // Libère la mémoire après exécution
	        }
	    }
	}
	Pool(int qsize) : queue(qsize){}
	void start(int nbThreads) {
		isRunning =true;
	        for (int i = 0; i < nbThreads; i++) {
	            // Utilisation de lambda pour capturer `this` et lancer poolWorker

	            threads.emplace_back(std::bind(&Pool::poolWorker, this, std::ref(queue), std::ref(isRunning)));
	    }
	}
	void submit(Job * job){
		queue.push(job);
	}

	void stop(){
		isRunning = false;
		queue.setBlocking(false);
		for(auto & t : threads){
			t.join();
		}
		threads.clear();
	}
	 ~Pool() {
	        stop(); // Appeler stop pour garantir que tout s'arrête avant la destruction
	    }
};

}
