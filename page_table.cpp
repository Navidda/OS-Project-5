#include "page_table.h"

#include <cstring>

using namespace std;

TLB::TLB() {
	memset(entries, -1, sizeof entries);
	first_out = 0;
}

int TLB::get_replacement_pos() {
	int pos = first_out % TLB_ENTRIES;
	return pos;
}

pair<index_t, int> TLB::get_frame(index_t index) {
	pair<index_t, int> res;
	for (int i = 0; i < TLB_ENTRIES; i++) {
		if (entries[i].page == index) {
			res = make_pair(entries[i].frame, 1);
			return res;
		}
	}
	res = make_pair(0, -1);
	return res;
}

void TLB::set_frame(index_t page, index_t frame) {
	int pos = get_replacement_pos();
	entries[pos].page = page;
	entries[pos].frame = frame;
	first_out++;
}

void TLB::invalidate(index_t frame_index) {
	for (int i = 0; i < TLB_ENTRIES; i++) {
		if (entries[i].frame == frame_index) {
			entries[i].page = -1;
		}
	}
}

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

void PageTable::invalidate(index_t frame_index) {
	for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		if (entries[i].frame == frame_index) {
			entries[i].valid = 0;
		}
	}
}

