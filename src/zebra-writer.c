#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
 * Globals
 * ************************************************************** */

int connected_readers = 0;

/* **************************************************************
 * Function implementations
 * ************************************************************** */

/**
 * What to do on a sigusr
 */
void signal_handler(int sig) {
  switch(sig) {

    // when reader times out...
  case SIGALRM: {
    if (DEBUG) printf("hmmm, maybe a reader died...\n");
    connected_readers--;
    if (connected_readers == 0 && DEBUG) {
      printf("AAALLLL BYYYY MYSEEEEEEEEELF\n");
    }
    break;
  }
    // reader sent an OK
  case SIGUSR1: {
    break;
  }
    
    // new connection
  case SIGUSR2: {
    if (DEBUG) printf("new connection!\n");
    connected_readers ++;
    break;
  }
    
    // clean up memory on sigint
  case SIGINT: {
    if (DEBUG) printf("got sigint\n");
    clean_and_exit();
    break;
  }
    
  default: {
    printf("Sig handled when it shouldn't be: %d\n", sig);
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

/**
 * Set certiain signals to use signal_handler()
 */
void set_sig_handlers() {
  signal(SIGINT, signal_handler);
  signal(SIGUSR1, signal_handler);
  signal(SIGUSR2, signal_handler);
  signal(SIGALRM, signal_handler);
}

/* **************************************************************
 * Main
 * ************************************************************** */

/**
 * Main
 */
int main(int argc, char* argv[]) {

  DEBUG = (argc > 1 && strcmp(argv[1], "-d") == 0);

  set_sig_handlers();

  key_t mem_key = ftok(zebra_keygen, 0);
  pid_t my_pid = getpid();

  // create shared mem
  shmId = shmget(mem_key , zebra_size, IPC_CREAT|S_IRUSR|S_IWUSR);
  if (shmId < 0) {
    printf("Failed get\n");
    exit(-1);
  }

  // attach to shared mem
  sharedPtr = shmat(shmId, (void*)0, 0);
  if (sharedPtr < 0) {
    printf("Failed attach :(\n");
    clean_and_exit();
    exit(-1); // just in case
  }
  
  // first slot in shared mem is writer's pid
  nth(int, sharedPtr, PID_INDEX) = my_pid;
  // second slot is the id for the current message, starts at 0 and increments
  nth(int, sharedPtr, MSG_ID_INDEX) = 0;
  
  /**
   * Write loop, continues forever, program can be killed with sigint
   */
  while(true) {

    // print some helpful messages
    if(DEBUG) printf("read: %d/%d\n", nth(int, sharedPtr, 2), connected_readers); 

    // check to see if the timeout system messed up
    if (nth(int, sharedPtr, READ_COUNT_INDEX) > connected_readers) 
      connected_readers = nth(int, sharedPtr, READ_COUNT_INDEX);

    
    if (nth(int, sharedPtr, READ_COUNT_INDEX) < connected_readers) {
      // if not all readers have responded to laters message, pause
      if (DEBUG) printf("pausin!\n");
      alarm(4);
      pause(); 
      alarm(0);
    } else {
      // else read new message into memory, increment msg id, and reset read count
      fgets(MSG_LOC(sharedPtr), zebra_msg_size, stdin);
      nth(int, sharedPtr, MSG_ID_INDEX) += 1;
      nth(int, sharedPtr, READ_COUNT_INDEX) = 0;
    }
  }
  
  clean_and_exit();
  return -1;
}


