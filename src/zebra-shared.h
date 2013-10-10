#ifndef __ZEBRA_SHARED_H__
#define __ZEBRA_SHARED_H__

#include <stdbool.h>

bool DEBUG = false;

/**
 * location in shared me where the string message will go 
 */
#define MSG_LOC(ptr) (ptr + sizeof(pid_t) + sizeof(int) + sizeof(int))

/**
 * as if shared mem was an int[] use these indexes for storing
 */
#define PID_INDEX 0
#define MSG_ID_INDEX 1
#define READ_COUNT_INDEX 2

/**
 * Shared file for ftok
 */
#define zebra_keygen "shared_file.txt"

/**
 * Mem size
 */
#define zebra_size (1024)

/**
 * Size used for string
 */
#define zebra_msg_size (1024 - sizeof(pid_t) - (sizeof(int) * 2))

#endif /* __ZEBRA_SHARED_H__ */
