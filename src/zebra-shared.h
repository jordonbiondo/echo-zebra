
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



#define zebra_keygen "shared_file.txt"

#define zebra_size 1024

#endif
