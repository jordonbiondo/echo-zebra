#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "zebra-shared.h"


/**
 * Main
 */
int main(void) {

  key_t mem_key = ftok(zebra_keygen, 0);

  int shmId = shmget(mem_key , zebra_size, IPC_CREAT|S_IRUSR|S_IWUSR);
  if (shmId < 0) {
    printf("Failed get\n");
    exit(-1);
  }
  
  printf("made: %d\n", shmId);
  printf("my pid: %d\n", getpid());
  
  char* sharedPtr = shmat(shmId, (void*)0, 0);
  
  if (sharedPtr < 0) {
    printf("Failed attach\n");
    exit(-1);
  }
  
  pid_t my_pid = getpid();
  
  memcpy(sharedPtr, int_bytes(&my_pid), 4);
  
  sleep(10);
  
  shmctl(shmId, IPC_RMID, 0);

  return 0;
}
