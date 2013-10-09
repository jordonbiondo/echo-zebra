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
#include <stdbool.h>

/**
 * Is process alive
 */
#define ALIVEP(pid) (kill(pid, 0) == 0)


/**
 * Main
 */
int main(void) {
  char* sharedMem;
  int size = zebra_size;  
  key_t key = ftok(zebra_keygen, 0);
  int shId;

  //Get shared memory
  if((shId = shmget(key, size, S_IRUSR|S_IWUSR)) < 0) {
    perror("Its not here.\n");
    exit(1);
  }

  // attach to mem
  if((sharedMem = shmat(shId, 0, 0)) == (void*) -1){
    perror("Didn't attach\n");
    exit(1);
  }

  // pid of writer
  int writer_pid = ((int*)sharedMem)[0];
  
  // tell writer you connected
  kill(writer_pid, SIGUSR2); // connect

  // last read message id
  int last_read = last_read = ((int*)sharedMem)[1];

  while(1) {
    // if we haven't read this message id
    if (last_read != ((int*)sharedMem)[1]) {
      sleep(1);
      printf("%s", MSG_LOC(sharedMem));
      last_read = ((int*)sharedMem)[1]; // mark as read
      ((int*)sharedMem)[2] += 1; // increment read count
      printf("count: %d", ((int*)sharedMem)[2]); // increment read count
      int failed = kill(writer_pid, SIGUSR1); // tell writer you read it
      if (failed) {
	printf("Writer is dead :(\n");
	exit(-1);
      }
      
    } else {
      // ensure writer is alive
      if(! ALIVEP(writer_pid)) {
	printf("Writer is dead :(\n");
	exit(-1);
      }
      // shady context switch
      usleep(100); 
    }
  }

  //Detach
  if(shmdt(sharedMem) < 0) {
    perror("detach failed.\n");
    exit(1);
  }

  return 0;
}
