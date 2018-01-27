#ifndef __MMU__
#define __MMU__

#include "memtypes.h"

#include <string>
#include <queue>
#include <set>

index_t extract_index(address_t address);
offset_t extract_offset(address_t address);
address_t assemble_address(index_t index, offset_t offset);

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

class Memory {
public:
    Memory();
    byte read(index_t index, offset_t offset);
    void write_frame(index_t frame_index, byte *data);
private:
    byte data[FRAME_NUM][FRAME_SIZE];
};

class MMU {
public:
    MMU(const std::string &swap_file);
    byte get_value(address_t logical_address);
protected:
    void fetch_page_from_disk(index_t index);
    address_t get_physical_address(address_t logical_address);

	virtual index_t get_frame_index() = 0; // for page replacement
	std::set<index_t> free_frames;

	Memory memory;
    PageTable pg_table;
    TLB tlb;
    std::string swap_file;

	int cnt;
};

class FIFO_MMU : public MMU {
public:
	FIFO_MMU(const std::string &swap_file);
protected:
	index_t get_frame_index();

	index_t first_index;
};

class RandomReplaceMMU : public MMU {
public:
	RandomReplaceMMU(const std::string &swap_file): MMU(swap_file) {};
protected:
	index_t get_frame_index();
};

class LRU_MMU : public MMU {
public:
	LRU_MMU(const std::string &swap_file);
protected:
	index_t get_frame_index();
	
	int last_use[FRAME_NUM];
};

class SecondChanceMMU : public MMU {
public:
	SecondChanceMMU(const std::string &swap_file);
protected:
	index_t get_frame_index();

	index_t pos;
	bool refrence[FRAME_NUM];
};

class MMUFactory {
public:
	static MMU *NewMMU(const std::string &swap_file, const std::string &pg_replace_method);
};

#endif
