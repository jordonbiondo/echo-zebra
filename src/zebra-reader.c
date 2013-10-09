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
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>

/**
 * Main
 */
int main(void) {
  char* sharedMem;
  char* readIn;
  int size = zebra_size;  
  key_t key = ftok(zebra_keygen, 0);
  int shId;

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
  pid_t writer_pid = bytes_to_int(PID_WRITE_LOC(sharedMem));
  printf("%d\n", writer_pid);
  printf("%d\n", bytes_to_int(READ_COUNT_LOC(sharedMem)));
  
  printf("reader: %s\n", MSG_LOC(sharedMem));
  sleep(1);
  int fail = kill(writer_pid, SIGUSR1);
  if (fail) {
    printf("fail: %d\n", fail);
  }
  /* while(1){ */
  /*   //Check if new data written, first byte = 0 */
  /*   if(sharedMem[4] == 0) */
  /*     { */
  /* 	readIn = sharedMem; */
  /* 	//Print out to display */
  /* 	printf("%s\n", readIn); */

  /* 	//Increment Count */
  /* 	sharedMem[4]++; */


  /* 	printf("SharedMem: %s\n", sharedMem); */

  /* 	//read in exit */
  /* 	if(strcmp(readIn, "exit")){ */
	  
  /* 	  //detach */
  /* 	  if(shmdt(sharedMem) < 0){ */
  /* 	    perror("detach failed\n"); */
  /* 	  } */
	  
  /* 	  exit(1); */
  /* 	} */
  /*     } */
  /*  } */
  
  //Detach
  if(shmdt(sharedMem) < 0){
    perror("detach failed.\n");
    exit(1);
  }

  printf("DETACHED!!\n");

  /* if(shmctl(shId, IPC_RMID, 0) < 0){ */
    /* perror("deallocate failed.\n"); */
    /* exit(1); */
  /* } */

  printf("OUTTA HERE!\n");

  return 0;
}
