#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "zebra-shared.h"

int main(void) {
  key_t mem_key = ftok(zebra_keygen, 'W');
  
  int shmId = shmget(mem_key , zebra_size, IPC_CREAT|S_IWUSR);
  printf("made: %d\n", shmId);
  char* sharedPtr = shmat(shmId, 0, 0);

  shmdt(sharedPtr);
  
  shmctl(shmId, IPC_RMID, 0);
	     
  return 0;
}
