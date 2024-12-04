#ifndef H_CHAT_COMMON
#define H_CHAT_COMMON

#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <fcntl.h>           /* Pour les constantes O_* */
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <iostream>
#include <ostream>

#include <string.h>

#define MAX_MESS 50
#define MAX_USERS 10
#define TAILLE_MESS 10

struct message {
  long type;
  char content[TAILLE_MESS];
};

struct myshm {
  char* name;
  int read; /* nombre de messages retransmis par le serveur */
  int write; /* nombre de messages non encore retransmis par le serveur */
  int nb; /* nombre total de messages emis */
  sem_t sem;
  sem_t semEmpty;
  sem_t semFull;
  struct message messages[MAX_MESS];
};

myshm * creatMyShm(char *  name){
  if (shm_unlink(name) == -1) {
        if (errno == ENOENT) {
            printf("Segment %s n'existe pas.\n", name);
        } else {
            perror("shm_unlink failed");
        }
      
    }
  int fd = shm_open(name,O_RDWR|O_EXCL|O_CREAT,0666);   
  ftruncate(fd,sizeof(struct myshm));

  void * shm = mmap(NULL,sizeof(struct myshm),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  close(fd);
  
  struct myshm* myShm = (struct myshm* ) shm;
  myShm -> name = name;
  sem_init(&myShm->sem,1,1);
  sem_init(&myShm->semFull,1,0);
  sem_init(&myShm->semEmpty,1,MAX_MESS);
   printf("Semaphore states after init: semFull=%d, semEmpty=%d\n", 0, MAX_MESS);
  
  return myShm;

}
void destroyMyShm(char* name, struct myshm* shared) {
    sem_destroy(&shared->sem);
    sem_destroy(&shared->semFull);
    sem_destroy(&shared->semEmpty);

    munmap(shared, sizeof(struct myshm));  // Détacher la mémoire
    shm_unlink(name);  // Supprimer le segment partagé
}
struct myshm* accessMyShm(char* name) {
    int fd = shm_open(name, O_RDWR, 0666);  // Ouvrir un segment partagé existant
    if (fd == -1) {
        perror("shm_open failed ici");
        return NULL;
    }

    // Mapper le segment en mémoire
    void* shm = mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);  // On peut fermer le descripteur après mmap

    if (shm == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }

    return (struct myshm*) shm;  // Retourne un pointeur vers la mémoire partagée
}
char *getName(char *name);

#endif
