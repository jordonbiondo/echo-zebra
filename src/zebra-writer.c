#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include "zebra-shared.h"
#include "zebra-writer.h"

/* **************************************************************
 * Function implementations
 * ************************************************************** */

int connected_readers = 0;

/**
 * What to do on a sigusr
 */
void signal_handler(int sig) {
  switch(sig) {

  case SIGALRM: {
    printf("hmmm, maybe a reader died...\n");
    connected_readers--;
    break;
  }

  case SIGUSR1: {
    return;
    break;
  }
    
  case SIGUSR2: {
    connected_readers ++;
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
    return;
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
  printf("%lu | %lu", sizeof(pid_t), sizeof(int));
  key_t mem_key = ftok(zebra_keygen, 0);
  signal(SIGINT, signal_handler);
  signal(SIGUSR1, signal_handler);
  signal(SIGUSR2, signal_handler);
  signal(SIGALRM, signal_handler);

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
  ((int*)sharedPtr)[0] = my_pid;
  //write_int(PID_WRITE_LOC(sharedPtr), my_pid);
  
  // set read count to initial 0
  ((int*)sharedPtr)[1] = 0;
  
  /**
   * Write loop
   */
  while(1) {
    printf("count: %d\n", ((int*)sharedPtr)[2]); // increment read count
    if (((int*)sharedPtr)[2] < connected_readers) {
      printf("pausin!\n");
      alarm(2);
      pause();
      alarm(0);
    } else {
      fgets(MSG_LOC(sharedPtr), zebra_msg_size, stdin);
      ((int*)sharedPtr)[1] += 1;
      ((int*)sharedPtr)[2] = 0;
    }
  }
  clean_and_exit();
  return -1;
}


