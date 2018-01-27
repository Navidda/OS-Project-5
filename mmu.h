#ifndef __MMU__
#define __MMU__

#include "memtypes.h"
#include <string>

index_t extract_index(address_t address);
offset_t extract_offset(address_t address);
address_t assemble_address(index_t index, offset_t offset);

class TLB {
public:

};

class PageTable {
public:
    PageTable();
    bool is_valid(index_t index);
    index_t get_frame(index_t index);
    void set_frame(index_t index, index_t frame);
private:
    class Entry {
    public:
        index_t frame;
        bool valid;
    };

    Entry entries[PAGE_TABLE_ENTRIES]; 
};

class Memory {
public:
    Memory();
    byte read(index_t index, offset_t offset);
    index_t write_frame(byte *data);
private:
    // set <index_t> free_pages;
    index_t first_empty;
    byte data[FRAME_NUM][FRAME_SIZE];
};

class MMU {
public:
    MMU(std::string swap_file);
    byte get_value(address_t logical_address);
private:
    void fetch_page_from_disk(index_t index);
    Memory memory;
    PageTable pg_table;
    TLB tlb;
    std::string swap_file;    
    address_t get_physical_address(address_t logical_address);
};

#endif
