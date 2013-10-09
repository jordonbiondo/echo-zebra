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
  printf("SIGNAL!");
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

  pid_t my_pid = getpid();
  printf("my mem: %d\n", shmId);
  printf("my pid: %d\n", my_pid);
  
  sharedPtr = shmat(shmId, (void*)0, 0);
  
  if (sharedPtr < 0) {
    printf("Failed attach :(\n");
    clean_and_exit();
    exit(-1); // just in case
  }
  
  
  
  // write my pid to mem
  write_int(PID_WRITE_LOC(sharedPtr), my_pid);
  
  // set read count to initial 0
  reset_read_count();
  
  fgets(MSG_LOC(sharedPtr), zebra_msg_size, stdin);
  pause();

  
  
  clean_and_exit();
  return -1;
}


