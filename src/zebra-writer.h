
#ifndef __ZEBRA_WRITER_H__
#define __ZEBRA_WRITER_H__
#include "zebra-shared.h"

/**
 * id of the shared mem
 */
int shmId;

/**
 * Pointer to the shared memoryx
 */
char* sharedPtr;

/**
 * Where the writer should put it's pid in shared mem
 */
#define PID_WRITE_LOC(ptr) ptr+0

/**
 * Where the readers and writers will keep track of the read counts in shared mem
 */
#define READ_COUNT_LOC(ptr) ptr+sizeof(pid_t)


/**
 * location in shared me where the string message will go 
 */
#define MSG_LOC(ptr) ptr+sizeof(pid_t)+sizeof(int)

/**
 * Handle signals
 */
void signal_handler(int);

/**
 * Clean up shared mem and exit
 */
void clean_and_exit();

/**
 * Get the number of reads from shared mem
 */
inline int read_count() {
  return bytes_to_int(READ_COUNT_LOC(sharedPtr));
}

/**
 * Reset the read count ticker
 */
inline void reset_read_count() {
  int zero = 0;
  memcpy(READ_COUNT_LOC(sharedPtr), int_bytes(&zero), sizeof(int));
}
#endif
