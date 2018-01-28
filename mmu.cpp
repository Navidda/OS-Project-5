#include "mmu.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

int tlb_hits = 0;
int page_faults = 0;
double overhead = 0;

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

MMU::MMU(const string &swap_file): swap_file(swap_file) {
	cnt = 0;
	memset(last_use, 0, sizeof last_use);
	memset(refrence, 0, sizeof refrence);
	for (int i = 0; i < FRAME_NUM; i++)
		free_frames.insert(i);
}


void MMU::fetch_page_from_disk(index_t page_index) {
    cerr << "fetching" << endl;

    ifstream fin(swap_file.c_str());
    fin.seekg(PAGE_SIZE * page_index, fin.beg);
    byte *data = new byte[PAGE_SIZE];
    for (int i = 0; i < PAGE_SIZE; i ++)
        data[i] = fin.get();

	index_t frame_index = get_frame_index();	
	memory.write_frame(frame_index, data);
	
	pg_table.invalidate(frame_index);
	tlb.invalidate(frame_index);
    free_frames.erase(frame_index);

    pg_table.set_frame(page_index, frame_index);

	overhead += 250000;
	overhead += 100;

    delete[] data;
}

byte MMU::get_value(address_t logical_address) {
    index_t page_index = extract_index(logical_address);
	pair<index_t, int> tlb_res = tlb.get_frame(page_index);
	index_t frame_index = tlb_res.first;
	if (tlb_res.second == TLB_MISS) {
		if (!pg_table.is_valid(page_index)){
        	fetch_page_from_disk(page_index);
			page_faults++;
		}
		frame_index = pg_table.get_frame(page_index);
		tlb.set_frame(page_index, frame_index);
	}
	else
		tlb_hits++;

    offset_t offset = extract_offset(logical_address);
    byte res = memory.read(frame_index, offset);

	last_use[frame_index] = cnt++;
	refrence[frame_index] = 1;
	overhead += 0.5;
	overhead += 100;

    return res;
}


FIFO_MMU::FIFO_MMU(const string &swap_file): MMU(swap_file), first_index(0) {}

index_t FIFO_MMU::get_frame_index() {
	int res = first_index;
	first_index = (first_index + 1) % FRAME_NUM;
	return first_index;
}


index_t RandomReplaceMMU::get_frame_index() {
	int res = rand() % FRAME_NUM;
	return res;
}


LRU_MMU::LRU_MMU(const string &swap_file): MMU(swap_file) {}

index_t LRU_MMU::get_frame_index() {
	if (free_frames.size() > 0)
		return *free_frames.begin();
	int idx = 0;
	for (int i = 0; i < FRAME_NUM; i++) {
		if (last_use[i] < last_use[idx]) {
			idx = i;
		}
	}
	return idx;
}


SecondChanceMMU::SecondChanceMMU(const string &swap_file): MMU(swap_file) {
	pos = 0;
}

index_t SecondChanceMMU::get_frame_index() {
	if (free_frames.size() > 0)
		return *free_frames.begin();
	while (true) {
		if (refrence[pos] == 0)
			break;
		refrence[pos] = 0;
		pos = (pos + 1) % FRAME_NUM;
	}
	return pos;
}


MMU* MMUFactory::NewMMU(const string &swap_file, const string &pg_replace_method) {
	if (pg_replace_method == "fifo")
		return new FIFO_MMU(swap_file);
	if (pg_replace_method == "random_replace")
		return new RandomReplaceMMU(swap_file);
	if (pg_replace_method == "lru")
		return new LRU_MMU(swap_file);
	if (pg_replace_method == "second_chance")
		return new SecondChanceMMU(swap_file);
	return NULL;
}
