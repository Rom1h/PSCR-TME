#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include <semaphore.h>
#include <csignal>
#include <fcntl.h>
#include <sys/stat.h> 

using namespace std;
using namespace pr;
#define N 10
#define M  10
Stack<char> *s;
sem_t* semProd;
sem_t* semCons;
int fd;

void handler_sigint(int sig){
 	sem_close(semProd);
    sem_close(semCons);
    sem_unlink("/semProd");
    sem_unlink("/semCons");
    munmap(s, sizeof(Stack<char>));
    shm_unlink("/monshm");
	exit(0);
}
void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		sem_wait(semProd);
		stack->push(c);
		sem_post(semCons);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		sem_wait(semCons);		
		char c = stack->pop();
		cout << c << flush ;
		sem_post(semProd);
	}
}
int main() {
    // Initialisation des sémaphores
    semProd = sem_open("/semProd", O_RDWR | O_CREAT|O_EXCL, 0666, 1);
    if (semProd == SEM_FAILED) {
        perror("Erreur sem_open pour semProd");
        exit(1);
    }

    semCons = sem_open("/semCons", O_RDWR | O_CREAT|O_EXCL, 0666, 0);
    if (semCons == SEM_FAILED) {
        perror("Erreur sem_open pour semCons");
        sem_close(semProd);
        sem_unlink("/semProd");
        exit(1);
    }

    // Création de la mémoire partagée pour la pile
    if ((fd = shm_open("/monshm", O_RDWR | O_CREAT, 0600)) == -1) {
        perror("Erreur shm_open");
        sem_close(semProd);
        sem_close(semCons);
        sem_unlink("/semProd");
        sem_unlink("/semCons");
        exit(1);
    }
    close(fd);

    // Redimensionner le segment pour contenir la pile
    if (ftruncate(fd, sizeof(Stack<char>)) == -1) {
        perror("Erreur ftruncate");
        close(fd);
        sem_close(semProd);
        sem_close(semCons);
        sem_unlink("/semProd");
        sem_unlink("/semCons");
        exit(1);
    }

    // Mappage de la mémoire partagée pour la pile
    if ((s = (Stack<char>*)mmap(NULL, sizeof(Stack<char>), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("Erreur mmap");
        close(fd);
        sem_close(semProd);
        sem_close(semCons);
        sem_unlink("/semProd");
        sem_unlink("/semCons");
        exit(1);
    }

    close(fd); // Le descripteur de fichier n'est plus nécessaire

    // Création des processus producteurs
    for (int i = 0; i < N; i++) {
        pid_t pp = fork();
        if (pp == 0) { // Processus fils
	        producteur(s);
            sem_close(semProd);
            sem_close(semCons);
            exit(0);
        } else if (pp < 0) {
            perror("Erreur fork pour producteur");
        }
    }

    // Création des processus consommateurs
    for (int i = 0; i < M; i++) {
        pid_t pc = fork();
        if (pc == 0) { // Processus fils
			
            consomateur(s);
            sem_close(semProd);
            sem_close(semCons);
            exit(0);
        } else if (pc < 0) {
            perror("Erreur fork pour consommateur");
        }
    }

    // Attente de tous les processus enfants
    for (int i = 0; i < N + M; i++) {
        wait(nullptr);
    }

    // Nettoyage des sémaphores et de la mémoire partagée
	signal(SIGINT,handler_sigint);

    return 0;
}

