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
 * Write a int to memory
 */
inline void write_int(byte* mem, int n) {
  int x = n;
  memcpy(mem, int_bytes(&x), sizeof(int));
  return;
}

/**
 * Reset the read count ticker
 */
inline void reset_read_count() {
  write_int(READ_COUNT_LOC(sharedPtr), 0);
}

#endif /* __ZEBRA_WRITER_H__ */
