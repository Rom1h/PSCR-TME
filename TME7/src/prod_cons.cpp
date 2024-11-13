#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>

using namespace std;
using namespace pr;
#define N 10
#define M  10

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

int main () {
	Stack<char> * s = new Stack<char>();

	
	for(int i = 0 ; i < N;i++){
		pid_t pp = fork();
		if (pp==0) {
			producteur(s);
			return 0;
		}
	}
	for(int i = 0 ; i < M;i++){
		pid_t pc = fork();
		if (pc==0) {
			consomateur(s);
			return 0;
		}
	}
	

	wait(0);
	wait(0);

	delete s;
	return 0;
}

