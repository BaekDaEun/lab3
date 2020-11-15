#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHMSIZE 1024
#define SHMKEY (key_t)0111

int main() {
    int pid = fork();
    FILE *fp;
    char *shared_txt = (char*)malloc(SHMSIZE);
    void *shared_memory = (void *)0;
    int shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT|0666);
    
    if(shmid == -1) {
        perror ("shmget failed");
        exit(0);
    }

    shared_memory = shmat(shmid, (void *)0,0);
    if(shared_memory == (void *)-1) {
        perror("shmat failed");
        exit(0);
    }

    shared_txt = (char *)shared_memory;
    
    if(pid ==0) {
        shmid = shmget(SHMKEY, SHMSIZE, 0);
        if(shmid == -1) {
            perror("shmget failed");
            exit(0);
        }
        shared_memory = shmat(shmid,(void *)0, IPC_CREAT|0666);

        if(shared_memory == (void *) -1) {
            perror("shmat failed");
            exit(0);
        }
        shared_txt = (char *)shared_memory;

        fp = fopen("test.txt", "r");
        fscanf(fp,"%[^\n]s", shared_txt);
        printf("Chile process read\n");
        fclose(fp);
    }

    else if(pid > 0) {
        fp = fopen("copy.txt","w");
        fprintf(fp,"%s", shared_txt);
        printf("Parent process copy.\n");
        wait(1);
        fclose(fp);
    }
}


