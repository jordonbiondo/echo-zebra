/*
 * Reader
 *
 *Jordon Biondo
 *Doug MacDonald
 *
 * Reads from shared memory
 * and outputs to display
 */

#include "zebra-shared.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>

//Prototypes
void CleanUp();
void handle_signal(int);

//Globals
char* sharedMem;
int shId;

int main(void) {
  char* readIn;
  int size = zebra_size;  
  key_t key = ftok(zebra_keygen, 0);

  signal(SIGINT, handle_signal);

  //Get shared memory
  if((shId = shmget(key, size, S_IRUSR|S_IWUSR)) < 0){
    perror("Its not here.\n");
    exit(1);
  }
  printf("Shared Mem Id: %d\n", shId); 

  //Attach to shared memory
  if((sharedMem = shmat(shId, 0, 0)) == (void*) -1){
    perror("Didn't attach\n");
    exit(1);
  }
  printf("ATTACHED!\n");

  
  while(1){
    //Check if new data written, first byte = 0
    if(sharedMem[4] == 0)
      {
	readIn = sharedMem;
	//Print out to display
	printf("%s\n", readIn);

	//Increment Count
	sharedMem[4]++;


	printf("SharedMem: %s\n", sharedMem);

	//read in exit
	if(strcmp(readIn, "exit")){
	  
	  //detach
	  if(shmdt(sharedMem) < 0){
	    perror("detach failed\n");
	  }
	  
	  exit(1);
	}
      }
   }
  
  CleanUp();

  return 0;
}

//Detach, Deallocate, and Exit
void CleanUp()
{
  //Detach
  if(shmdt(sharedMem) < 0){
    perror("detach failed\n");
    exit(1);
  }
  //Deallocate
  if(shmctl(shId, IPC_RMID, 0) < 0){
    perror("deallocate failed\n");
    exit(1);
  }
  //exit
  exit(0);
}


//On Signal Call CleanUp
void handle_signal(int sig){
  CleanUp();
}
