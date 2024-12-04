#include "chat_common.h"

#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h> 
#include <string>
#include <vector>
using namespace std;
void sendMessClient(struct message* mess, vector<myshm*>& users) {
    for (myshm* s : users) {
        if (s == NULL) {
            fprintf(stderr, "Invalid pointer in users vector.\n");
            continue;
        }
        sem_wait(&s->semEmpty);
        sem_wait(&s->sem);

        int index = s->write % MAX_MESS;
        s->messages[index] = *mess;
        s->write = (s->write + 1) % MAX_MESS;

        sem_post(&s->sem);
        sem_post(&s->semFull);

        printf("Message sent to %s\n", s->name);
    }
}

void messManager(struct message* mess, struct myshm* shm, vector<myshm*>& users) {
    if (mess->type == 1) {
        struct myshm* clientShm = accessMyShm(mess->content);
        if (clientShm == NULL) {
            fprintf(stderr, "Failed to access segment: %s\n", mess->content);
            return;
        }
        clientShm->name = strdup(mess->content);
        users.push_back(clientShm);
    } else if (mess->type == 2) {
        sendMessClient(mess, users);
    } else if (mess->type == 3) {
        for (auto i = users.begin(); i != users.end();) {
            if (!strcmp(mess->content, (*i)->name)) {
                free((*i)->name);   // Libérer la mémoire allouée pour le nom
                users.erase(i);    // Supprimer l'élément et mettre à jour l'itérateur
            } else {
                ++i;
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <segment_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* name = argv[1];
    struct myshm* myShm = creatMyShm(name);
    if (myShm == NULL) {
        fprintf(stderr, "Failed to create segment: %s\n", name);
        return EXIT_FAILURE;
    }

    vector<myshm*> client;

    while (true) {
        sem_wait(&myShm->semFull);
        sem_wait(&myShm->sem);

        int indice = myShm->read % MAX_MESS;
        struct message* mess = &myShm->messages[indice];
        myShm->read++;
        messManager(mess, myShm, client);

        sem_post(&myShm->sem);
        sem_post(&myShm->semEmpty);
    }

    return 0;
}