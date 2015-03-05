/*
 This allows us to separate memory implementation (and therefore mapping) from the CPU,
 making it easier to reuse the CPU for say, a NES and a C64, and still be able to define
 specialised memory mapping by subclassing this interface and re-implementing memAccess()
 */

#ifndef BASEMEM_H
#define BASEMEM_H

#include <cstdint>
#include <cstdio>
#include <string>
#include <sstream>
#include <iomanip>

class BaseMemoryInterface {
public:
	BaseMemoryInterface() {
	}
	virtual ~BaseMemoryInterface() {
	} //virtual to allow for destroying anything custom memAccess implementations leave behind

	//These are what the CPU (or anything else, if you like) uses to interface with whatever memAccess maps the given 16-bit address to.
	uint8_t Read(uint16_t address) {
		return memAccess(address);
	}
	uint16_t ReadWord(uint16_t address) {
		return memAccess(address + 1) << 8 | memAccess(address);
	} //little endian ordering
	void Write(uint16_t address, uint8_t value) {
		memAccess(address, true, value);
	}

	std::string DumpRAM(uint16_t start, uint8_t offset) {
		std::ostringstream ss;
		uint16_t address = start;
		int columnCounter = 1; //this is pants but whatever

		while(address < (start + offset)) {
			if(columnCounter == 1) ss << std::hex << address << ": ";

			ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned)ram[address] <<" ";

			if(columnCounter == 16) {
				ss << std::endl;
				columnCounter = 0;
			}

			address++; columnCounter++;
		}

		std::string s = ss.str();
		return s;
	}
	void FileDump(const char* filename, uint16_t address) { //dump a binary file into memory, starting at the address provided
		FILE* bin = fopen(filename, "rb"); //read-only binary stream
		// obtain file size:
		fseek(bin, 0, SEEK_END);
		int binSize = ftell(bin);
		rewind(bin);
		fread(&ram[address], 1, binSize, bin);
	}
protected:
	virtual uint8_t memAccess(uint16_t address, bool write = false,
			uint8_t value = 0) {
		//This is the simplest access routine; basically DMA to all possible 64k of memory
		//Something like the NES would require more complex mapping
		if (write)
			ram[address] = value;
		return ram[address];
	}

	//Always provide 64k of RAM, allow memAccess() to handle mapping and what is actually used.
	//This represents what the CPU is capable of accessing (64k) while memAccess() can choose to only use
	//for example, the range 0x0 - 0x7ff, which would provide 2k of RAM like in the NES.
	uint8_t ram[0xffff];
};

#endif
