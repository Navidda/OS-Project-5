#include "mmu.h"
#include <iostream>
#include <fstream>

using namespace std;

extern int tlb_hits;
extern int page_faults;
extern double overhead;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "usage: ./run <input file address>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (fin.fail() || fin.peek() == EOF) {
        cout << "invalid file name\n";
        return 1;
    }

	int number_of_entries = 0;

	MMU *mmu = MMUFactory::NewMMU("backing_store.bin", "fifo");
	address_t address;
	while (fin >> address) {
		cout << address << " -> " << mmu->get_value(address) << endl;
		number_of_entries++;
	}

	cout << "Page Fault = " << ((double)page_faults/(double)number_of_entries) * 100 << "%" << endl;
	cout << "TLB Hit Ratio = " << ((double)tlb_hits/(double)number_of_entries) * 100 << "%" << endl;
	cout << "Overhead = " << overhead << " ns" << endl;

    return 0;
}
