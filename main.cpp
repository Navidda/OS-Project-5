#include "mmu.h"
#include <iostream>
#include <fstream>

using namespace std;

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

	MMU *mmu = MMUFactory::NewMMU("backing_store.bin", "second_chance");
	address_t address;
	while(fin >> address) {
		cout << address << " -> " << mmu->get_value(address) << endl;
	}

    return 0;
}
