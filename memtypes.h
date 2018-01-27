#ifndef __MEMTYPES__
#define __MEMTYPES__

const int PAGE_TABLE_ENTRIES = 256;
const int PAGE_SIZE = 256;
const int TLB_ENTRIES = 16;
const int FRAME_SIZE = 256;
const int FRAME_NUM = 256;

typedef unsigned int address_t;
typedef char byte;
typedef unsigned char index_t;
typedef unsigned char offset_t;

#endif
