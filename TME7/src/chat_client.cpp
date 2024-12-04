#include "chat_common.h"

#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h> 

#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <iostream>
using namespace std;
void send(struct myshm * send){
    while (true)
    { 
        sem_wait(&send->semEmpty); // Attente d'une place libre

        struct message mess;
        mess.type = 2;
        std::string input;
        std::getline(std::cin, input); // Lire jusqu'à appui sur Entrée

        strncpy(mess.content, input.c_str(), sizeof(mess.content) - 1);
        mess.content[sizeof(mess.content) - 1] = '\0';

        sem_wait(&send->sem); // Entrée en section critique

        int index = send->write % MAX_MESS;
        send->messages[index] = mess;
        send->write = (send->write + 1) % MAX_MESS;

        sem_post(&send->sem); // Sortie de la section critique
        sem_post(&send->semFull); // Signaler qu'un message est disponible
        printf("Message sent: %s\n", mess.content);

    }
    
}
void recept (struct myshm * recept){
    while(true){
        sem_wait(&recept->semFull); // Attente d'un message disponible

        sem_wait(&recept->sem); // Entrée en section critique
        int index = recept->read % MAX_MESS;
        struct message* mess = &recept->messages[index];
        printf("Received message: %s\n", mess->content);

        recept->read = (recept->read + 1) % MAX_MESS;

        sem_post(&recept->sem); // Sortie de la section critique
        sem_post(&recept->semEmpty); // Signaler qu'une place est libres
    }
}
int main (int argc,char ** argv){
    char * nameServeur = argv[1];
    char * nameClient = argv[2];
    struct myshm * myShmRecept = accessMyShm(nameServeur);
    struct myshm * myShmSend = creatMyShm(nameClient);
   
    if (myShmRecept == NULL) {
        fprintf(stderr, "Failed to access segment /test\n");
        exit(EXIT_FAILURE);
    }

    struct message mess;
    mess.type = 1;
    strcpy(mess.content,nameClient);
    
    sem_wait(&myShmRecept->semEmpty);
    sem_wait(&myShmRecept->sem);
    int index = myShmRecept->write % MAX_MESS;  // Gestion circulaire des indices
    myShmRecept->messages[index] = mess;      // Copier le message dans le tableau
    myShmRecept->write = (myShmRecept->write + 1) % MAX_MESS;
    sem_post(&myShmRecept->sem);
    sem_post(&myShmRecept->semFull);
   

    vector<thread> threads;
    threads.reserve(2);
    threads.emplace_back(recept,myShmSend);
    threads.emplace_back(send,myShmRecept);
    for (auto & t : threads) {
		t.join();
	}
    
}