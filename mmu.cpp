#include "mmu.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>

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

MMU::MMU(const string &swap_file): swap_file(swap_file) {}

void MMU::fetch_page_from_disk(index_t page_index) {
    cerr << "fetching" << endl;

    ifstream fin(swap_file.c_str());
    fin.seekg(PAGE_SIZE * page_index, fin.beg);
    byte *data = new byte[PAGE_SIZE];
    for (int i = 0; i < PAGE_SIZE; i ++)
        data[i] = fin.get();

	index_t frame_index = get_frame_index();
    memory.write_frame(frame_index, data);
    pg_table.set_frame(page_index, frame_index);

    delete[] data;
}

byte MMU::get_value(address_t logical_address) {
    index_t page_index = extract_index(logical_address), frame_index;
    if (!pg_table.is_valid(page_index))
        fetch_page_from_disk(page_index);
    frame_index = pg_table.get_frame(page_index);
    offset_t offset = extract_offset(logical_address);
    byte res = memory.read(frame_index, offset);
    return res;
}

byte Memory::read(index_t frame_index, offset_t offset) {
    return data[frame_index][offset];
}

void Memory::write_frame(index_t frame_index, byte *frame_data) {
    memcpy(data[frame_index], frame_data, PAGE_SIZE);
}

Memory::Memory() {};

FIFO_MMU::FIFO_MMU(const string &swap_file): MMU(swap_file), first_index(0) {}

index_t FIFO_MMU::get_frame_index() {
	int res = first_index;
	first_index = (first_index + 1) % FRAME_NUM;
	return first_index;
}

MMU* MMUFactory::NewMMU(const string &swap_file, const string &pg_replace_method) {
	if (pg_replace_method == "fifo")
		return new FIFO_MMU(swap_file);
	return NULL;
}
