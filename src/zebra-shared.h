
#ifndef __ZEBRA_SHARED_H__
#define __ZEBRA_SHARED_H__

typedef char byte;

/**
 * Int to bytes
 */
byte* int_bytes(int* x) {
  return (byte*)x;
}


/**
 * Bytes to int
 */
int bytes_to_int(byte* bytes) {
  return bytes[0] 
    + (((int)bytes[1]) << 8)
    + (((int)bytes[2]) << 16)
    + (((int)bytes[3]) << 24);
}


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


#define zebra_keygen "shared_file.txt"

#define zebra_size 1024

#define zebra_msg_size (1024 - sizeof(pid_t) - sizeof(int))

#endif
