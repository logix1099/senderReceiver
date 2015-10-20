#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

   struct input {
      int load; //load pointer
      int unload; //unload pointer
      char shm_msg[3][100];  //three buffers big enough to hold a string 100 chars long
   } *ram;
int main() {
	int size = 512;
	char semName[] = "buffer";
	char full_name[] = "full";
	char empty_name[] = "empty";
	sem_t *mutex;
	sem_t *full;
	sem_t *empty;
	char msg[size];
   int shmid;
   key_t key;
   key = 5000;
   size_t shmSize = 2;   
   if ((shmid = shmget(key, shmSize, 0666)) < 0) {
        perror("shmget");
        exit(1);
   }
   if ((ram = (struct input *) shmat(shmid, NULL, 0)) == (struct input *) -1) {
        perror("shmat");
        exit(1);
    }

    mutex = sem_open(semName, O_CREAT, 0666, 1);
    full = sem_open(full_name, O_CREAT, 0666,0);
    empty = sem_open(empty_name,O_CREAT, 0666,2);
    while (1)
   {
      sem_wait(full);
	   sem_wait(mutex);
	   strcpy(msg,ram->shm_msg[ram->unload]);
	   if (ram->unload == 2)
	   {
   	   ram->unload = 0;
	   }
	   else{
	      ram->unload++;
	   }

	    sem_post(mutex);
	    sem_post(empty);
	   printf("%s\n", msg);
   }
   sem_close(mutex);
   sem_unlink(semName);
}
