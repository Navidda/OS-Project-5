#ifndef __PAGE_TABLE_H__
#define __PAGE_TABLE_H__

#include "memtypes.h"

#include <iostream>

class TLB {
public:
	TLB();
	std::pair<index_t, int> get_frame(index_t index);
	void set_frame(index_t index, index_t frame);
	int get_replacement_pos();
private:
	class Entry {
		public:
			index_t page;
			index_t frame;
	};

	Entry entries[PAGE_TABLE_ENTRIES];
	int first_out;

};

class PageTable {
public:
    PageTable();
    bool is_valid(index_t index);
    index_t get_frame(index_t index);
    void set_frame(index_t index, index_t frame);
	void invalidate(index_t frame_index);
private:
    class Entry {
    public:
        index_t frame;
        bool valid;
    };

    Entry entries[PAGE_TABLE_ENTRIES]; 
};

#endif
