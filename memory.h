#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "memtypes.h"

class Memory {
public:
    Memory();
    byte read(index_t index, offset_t offset);
    void write_frame(index_t frame_index, byte *data);
private:
    byte data[FRAME_NUM][FRAME_SIZE];
};

#endif
