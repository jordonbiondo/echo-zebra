#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "zebra-shared.h"
#include "zebra-writer.h"

/* **************************************************************
 * Function implementations
 * ************************************************************** */

/**
 * What to do on a sigusr
 */
void signal_handler(int sig) {
  switch(sig) {

  case SIGUSR1: case SIGUSR2: {
    printf("sigusr: %d\n", sig);
    // do sig user stuff
    break;
  }
    
  case SIGINT: {
    printf("sigint\n");
    clean_and_exit();
    break;
  }
  default: {
    //defualt
    printf("eff\n");
  }
  }
}


/**
 * Cleanup shared mem and exit
 */
void clean_and_exit() {
  shmctl(shmId, IPC_RMID, 0);
  exit(0);
}

/* **************************************************************
 * Main
 * ************************************************************** */

/**
 * Main
 */
int main(void) {
  
  key_t mem_key = ftok(zebra_keygen, 0);
  signal(SIGINT, signal_handler);
  signal(SIGUSR1, signal_handler);
  signal(SIGUSR2, signal_handler);
  shmId = shmget(mem_key , zebra_size, IPC_CREAT|S_IRUSR|S_IWUSR);
  if (shmId < 0) {
    printf("Failed get\n");
    exit(-1);
  }
  
  printf("made: %d\n", shmId);
  printf("my pid: %d\n", getpid());
  
  sharedPtr = shmat(shmId, (void*)0, 0);
  
  if (sharedPtr < 0) {
    printf("Failed attach\n");
    exit(-1);
  }
  
  pid_t my_pid = getpid();
  
  // write my pid to mem
  memcpy(sharedPtr, int_bytes(&my_pid), sizeof(int));
  // write 0 to read count
  int zero = 1;
  printf("zed: %d\n", zero);
  memcpy(sharedPtr+4, int_bytes(&zero), sizeof(int));
  
  fgets(sharedPtr+8, zebra_size - 8, stdin);
  pause();

  
  
  clean_and_exit();
  return -1;
}


