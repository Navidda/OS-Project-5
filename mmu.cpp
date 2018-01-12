#include "mmu.h"
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

PageTable::PageTable() {
    memset(entries, 0, sizeof entries);
}

bool PageTable::is_valid(index_t index) {
    return entries[index].valid;
}

index_t PageTable::get_frame(index_t index) {
    return entries[index].frame;
}

void PageTable::set_frame(index_t index, index_t frame) {
    entries[index].frame = frame;
    entries[index].valid = 1;
}

index_t extract_index(address_t address) {
    return (index_t) (address >> 8);
}

offset_t extract_offset(address_t address) {
    return (offset_t) (address);
}

address_t assemble_address(index_t index, offset_t offset) {
    address_t res = index;
    return (res << 8) + offset;
}

MMU::MMU(string swap_file): swap_file(swap_file) {}

void MMU::fetch_page_from_disk(index_t index) {
    cerr << "fetching\n";
    ifstream fin(swap_file.c_str());
    fin.seekg(PAGE_SIZE * index, fin.beg);
    byte *data = new byte[PAGE_SIZE];
    for (int i = 0; i < PAGE_SIZE; i ++)
        data[i] = fin.get();
    index_t physical_index = memory.write_frame(data);
    pg_table.set_frame(index, physical_index);
    delete[] data;
}

byte MMU::get_value(address_t logical_address) {
    index_t logical_index = extract_index(logical_address), physical_index;
    if (!pg_table.is_valid(logical_index))
        fetch_page_from_disk(logical_index);
    physical_index = pg_table.get_frame(logical_index);
    offset_t offset = extract_offset(logical_address);
    byte res = memory.read(physical_index, offset);
    return res;
}

byte Memory::read(index_t index, offset_t offset) {
    return data[index][offset];
}

index_t Memory::write_frame(byte *frame_data) {
    memcpy(data[first_empty], frame_data, PAGE_SIZE);
    return first_empty ++;
}

Memory::Memory(): first_empty(0) {};
