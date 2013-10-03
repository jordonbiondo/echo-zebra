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

int main(void) {
  char* sharedMem;
  char* readIn;
  int size = zebra_size;  
  key_t key = ftok(zebra_keygen, 0);

  //Attach to shared memory
  if((sharedMem = shmat(key, 0, 0)) == (void*) -1){
    perror("Didn't attach\n");
    exit(1);
  }

  while(1){
    readIn = sharedMem;
    //Print out to display
    printf("%s\n", readIn);


    //read in exit
    if(strcmp(readIn, "exit")){

      //detach
      if(shmdt(sharedMem) < 0){
	  perror("detach failed\n");
	}

      exit(1);
    }
   }

  return 0;
}
