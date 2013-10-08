
#ifndef __ZEBRA_SHARED_H__
#define __ZEBRA_SHARED_H__

typedef char byte;

byte* int_bytes(int* x) {
  return (byte*)x;
}


int bytes_to_int(byte* bytes) {
  return bytes[0] 
    + (bytes[1] << 8)
    + (bytes[2] << 16)
    + (bytes[3] << 24);
}

#define zebra_keygen "shared_file.txt"

#define zebra_size 1024

#endif
