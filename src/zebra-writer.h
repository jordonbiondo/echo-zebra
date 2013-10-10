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

#define nth(type, byte_buffer, index) ((type*)byte_buffer)[index]

#endif /* __ZEBRA_WRITER_H__ */
