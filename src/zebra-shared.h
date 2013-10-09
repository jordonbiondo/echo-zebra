
#ifndef __ZEBRA_SHARED_H__
#define __ZEBRA_SHARED_H__


/**
 * location in shared me where the string message will go 
 */
#define MSG_LOC(ptr) (ptr + sizeof(pid_t) + sizeof(int) + sizeof(int))


#define zebra_keygen "shared_file.txt"

/**
 * Mem size
 */
#define zebra_size (1024)

/**
 * Size used for string
 */
#define zebra_msg_size (1024 - sizeof(pid_t) - (sizeof(int) * 2))

#endif
