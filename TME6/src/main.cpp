/*
 * main.cpp
 *
 *  Created on: 23 oct. 2024
 *      Author: 21412125
 */
#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <ctime>
#include "rsleep.h"
int pv = 3;
void sighandler(int signal){
	--pv;
	std::cout<< "pv restant : " <<pv <<" pid "<<getpid()<< std::endl;
	if(pv == 0){
		std::cout<< "T'es mort."<<getpid()<< std::endl;
		exit(1);
	}
	return ;

}
void handlerDef(int signal){
	std::cout<<"coup paré "<<getpid()<<std::endl;
	return ;

}
void attaque(pid_t adversaire){
	std::cout<<"Je suis " << getpid() <<" et je combat "<<adversaire<<std::endl;
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = &sighandler;
	sigaction(SIGINT,&act,nullptr);
  
	if(kill(adversaire,SIGINT)==-1){
		std::cout<<"L'adversaire est mort."<<adversaire<<std::endl;
		exit(0);
	}

	srand(time(nullptr)+getpid());
	int sleep_d=700000+rand()%300000;
	usleep(sleep_d);

}
void defense (){
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	//act.sa_handler = SIG_IGN;
	act.sa_handler = &handlerDef;
	sigaction(SIGINT,&act,nullptr);
	
	sigset_t setPos;
	sigemptyset(&setPos);
	sigaddset(&setPos,SIGINT);
	sigprocmask(SIG_BLOCK,&setPos,nullptr);
	
	srand(time(nullptr)+getpid());
	int sleep_d=300000+rand()%700000;
	usleep(sleep_d);
	
	sigprocmask(SIG_UNBLOCK,&setPos,nullptr);

	


}

void combat(pid_t adversaire){
	while(pv>0){
		defense();
		attaque(adversaire);
	}
}
int main(){
	    srand(time(nullptr));  
	pid_t pid = fork();
	if(pid == 0){
		combat(getppid());
	}
	else{
		combat(pid);
	}
}


