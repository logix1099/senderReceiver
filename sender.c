#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
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
	key_t key; /* key to be passed to shmget() */ 
   int shmflg; /* shmflg to be passed to shmget() */ 
   int shmid; /* return value from shmget() */ 
   size_t shmSize = 2; /* size to be passed to shmget() */
   sem_t *mutex;
   sem_t *full;
	sem_t *empty;
   char msg[100];
   char full_name[] = "full";
	char empty_name[] = "empty";
   char semName[] = "buffer";
   int size = 512;
   key = 5000;
   shmflg = IPC_CREAT| 0666;
	//creates the segment
	   
   
	shmid = shmget (key, shmSize, shmflg);
	if (shmid < 0) {
      perror("shmget"); 
      exit(1); 
   }
   //attaches the segment to have a pointer to it
	ram = (struct input *) shmat(shmid, NULL, shmflg);
   if (ram  == (struct input *) -1) {
      perror("shmat");
      exit(1);
   }
   mutex = sem_open(semName, O_CREAT, 0666, 1);
   full = sem_open(full_name, O_CREAT, 0666,0);
   empty = sem_open(empty_name,O_CREAT, 0666,2);
   while (1)
   {
      printf ("enter message\n");
	   scanf ("%s", &msg);
	   
	   sem_wait(empty);
	   sem_wait(mutex);
	   
	   strcpy(ram->shm_msg[ram->load],msg);
	   
	   if (ram->load == 2)
	   {
   	   ram->load = 0;
	   }
	   else{
	      ram->load++;
	   }
	   sem_post(mutex);
	   sem_post(full);
   }
   sem_close(mutex);
   sem_unlink(semName);
}
