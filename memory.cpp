#include "memory.h"

#include <cstring>

using namespace std;

byte Memory::read(index_t frame_index, offset_t offset) {
    return data[frame_index][offset];
}

void Memory::write_frame(index_t frame_index, byte *frame_data) {
    memcpy(data[frame_index], frame_data, PAGE_SIZE);
}

Memory::Memory() {};

