#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
int main () {
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;
	std::vector<pid_t> child;
	int nbChild = 0;
	for (int i=1, j=N; i<=N && j==N ; i++ ) {
		if(fork() != 0) break;

		nbChild = 0;
		child.clear();
		std :: cout << getpid()<<std::endl;
		child.push_back(getpid());
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0){
				nbChild = 0;
				child.clear();
				std :: cout << getpid()<<std::endl;
				child.push_back(getpid());
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
				exit(0);
			}
			else{
				nbChild++;
			}
		}


	}

	nbChild ++;

	for (int i = 0 ; i < nbChild ; i++){
		wait(nullptr);
	}
	return 0;
}