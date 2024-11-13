#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc , char ** argv){
    char* arg1[argc];
    char* arg2[argc];
    bool isPipe = false;
    int ind1, ind2 = 0;
    for(int i = 1 ; i<argc;i++){
        if(!strcmp(argv[i],"|")){
            isPipe=true;

        }
        else if(isPipe){ 
            arg2[ind2]=argv[i];   
            ind2++;
        }
        else{
            arg1[ind1]=argv[i];    
            ind1++;
        }

    }
    arg1[ind1]=nullptr;
    arg2[ind1]=nullptr;

    int tubeDesc[2];
    pid_t fils;

    if(pipe(tubeDesc)==-1){
        perror ("pipe");
        exit (1);
    }

    if ( (fils = fork ( )) == -1 ){
        perror ("fork");
        exit (2);
    }
    if(fils ==0){//dans fils
        dup2(tubeDesc[1],STDOUT_FILENO);
        close (tubeDesc[1]); close (tubeDesc[0]);
        if (execvp (arg1[0],arg1) == -1) {
            perror ("execl"); exit (3);
        }
    }
    else{//dans pere
        dup2(tubeDesc[0],STDIN_FILENO);
        close (tubeDesc[0]);
        close (tubeDesc[1]);
        if (execvp (arg2[0],arg2) == -1) {
            perror ("execl"); exit (3);
        }

    }
    return EXIT_SUCCESS;


}
