#ifndef __MMU__
#define __MMU__

#include "memtypes.h"
#include <string>

class TLB {
public:

};

class PageTable {
public:
    
};

class MMU {
public:
    MMU(std::string swap_file);
    int get_value(address_t logical_address);
private:
    std::string swap_file;    
    address_t get_physical_address(address_t logical_address);
};

#endif