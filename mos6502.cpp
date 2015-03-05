#include <iostream>
#include <sstream>
#include <iomanip>

#include "mos6502.hpp"
#include "Log.hpp"

mos6502::mos6502(BaseMemoryInterface* m) {
	M = m; //wire up the memory interface

	//initialise registers
	A = 0, X = 0, Y = 0;
	PC = 0xfffc; //reset vector
	S = 0xff; //start at 256 in case reset routine doesn't do this?
	P = 0x24; //the Always 1 flag is set, and we set Interrupt disable at power on

	//initialise any internal bits
	reset = true; //start in reset, means we can execute bootstrap as a reset ;)
	nmi = false, irq = false, xPage = false;
	skipCycles = 0;

	//let's build a lookup table!
	for(int i=0; i<=255; i++) {
		uint8_t j = i & 0xff; //narrow i to an 8-bit value for safety
		switch(j) {
		//ADC
		case 0x69: instructionSet[j] = {j, "ADC", ADC, Imm_, 2, 2}; break;
		case 0x65: instructionSet[j] = {j, "ADC", ADC, ZPg_, 2, 3}; break;
		case 0x75: instructionSet[j] = {j, "ADC", ADC, ZPgX, 2, 4}; break;
		case 0x6D: instructionSet[j] = {j, "ADC", ADC, Abs_, 3, 4}; break;
		case 0x7D: instructionSet[j] = {j, "ADC", ADC, AbsX, 3, 4}; break;
		case 0x79: instructionSet[j] = {j, "ADC", ADC, AbsY, 3, 4}; break;
		case 0x61: instructionSet[j] = {j, "ADC", ADC, IndX, 2, 6}; break;
		case 0x71: instructionSet[j] = {j, "ADC", ADC, IndY, 2, 5}; break;
		//AND
		case 0x29: instructionSet[j] = {j, "AND", AND, Imm_, 2, 2}; break;
		case 0x25: instructionSet[j] = {j, "AND", AND, ZPg_, 2, 2}; break;
		case 0x35: instructionSet[j] = {j, "AND", AND, ZPgX, 2, 3}; break;
		case 0x2D: instructionSet[j] = {j, "AND", AND, Abs_, 3, 4}; break;
		case 0x3D: instructionSet[j] = {j, "AND", AND, AbsX, 3, 4}; break;
		case 0x39: instructionSet[j] = {j, "AND", AND, AbsY, 3, 4}; break;
		case 0x21: instructionSet[j] = {j, "AND", AND, IndX, 2, 6}; break;
		case 0x31: instructionSet[j] = {j, "AND", AND, IndY, 2, 5}; break;
		//ASL
		case 0x0A: instructionSet[j] = {j, "ASL", ASL, Acc_, 1, 2}; break;
		case 0x06: instructionSet[j] = {j, "ASL", ASL, ZPg_, 2, 5}; break;
		case 0x16: instructionSet[j] = {j, "ASL", ASL, ZPgX, 2, 6}; break;
		case 0x0E: instructionSet[j] = {j, "ASL", ASL, Abs_, 3, 6}; break;
		case 0x1E: instructionSet[j] = {j, "ASL", ASL, AbsX, 3, 7}; break;
		//BIT
		case 0x24: instructionSet[j] = {j, "BIT", BIT, ZPg_, 2, 3}; break;
		case 0x2C: instructionSet[j] = {j, "BIT", BIT, Abs_, 3, 4}; break;
		//Branches
		case 0x90: instructionSet[j] = {j, "BCC", BCC, Rel_, 2, 2}; break;
		case 0xB0: instructionSet[j] = {j, "BCS", BCS, Rel_, 2, 2}; break;
		case 0xF0: instructionSet[j] = {j, "BEQ", BEQ, Rel_, 2, 2}; break;
		case 0x30: instructionSet[j] = {j, "BMI", BMI, Rel_, 2, 2}; break;
		case 0xD0: instructionSet[j] = {j, "BNE", BNE, Rel_, 2, 2}; break;
		case 0x10: instructionSet[j] = {j, "BPL", BPL, Rel_, 2, 2}; break;
		case 0x50: instructionSet[j] = {j, "BVC", BVC, Rel_, 2, 2}; break;
		case 0x70: instructionSet[j] = {j, "BVS", BVS, Rel_, 2, 2}; break;
		//BRK
		case 0x00: instructionSet[j] = {j, "BRK", BRK, Imp_, 1, 7}; break;
		//CLEAR flags
		case 0x18: instructionSet[j] = {j, "CLC", CLC, Imp_, 1, 2}; break;
		case 0xD8: instructionSet[j] = {j, "CLD", CLD, Imp_, 1, 2}; break;
		case 0x58: instructionSet[j] = {j, "CLI", CLI, Imp_, 1, 2}; break;
		case 0xB8: instructionSet[j] = {j, "CLV", CLV, Imp_, 1, 2}; break;
		//CMP
		case 0xC9: instructionSet[j] = {j, "CMP", CMP, Imm_, 2, 2}; break;
		case 0xC5: instructionSet[j] = {j, "CMP", CMP, ZPg_, 2, 3}; break;
		case 0xD5: instructionSet[j] = {j, "CMP", CMP, ZPgX, 2, 4}; break;
		case 0xCD: instructionSet[j] = {j, "CMP", CMP, Abs_, 3, 4}; break;
		case 0xDD: instructionSet[j] = {j, "CMP", CMP, AbsX, 3, 4}; break;
		case 0xD9: instructionSet[j] = {j, "CMP", CMP, AbsY, 3, 4}; break;
		case 0xC1: instructionSet[j] = {j, "CMP", CMP, IndX, 2, 6}; break;
		case 0xD1: instructionSet[j] = {j, "CMP", CMP, IndY, 2, 5}; break;
		//CPX
		case 0xE0: instructionSet[j] = {j, "CPX", CPX, Imm_, 2, 2}; break;
		case 0xE4: instructionSet[j] = {j, "CPX", CPX, ZPg_, 2, 3}; break;
		case 0xEC: instructionSet[j] = {j, "CPX", CPX, Abs_, 3, 4}; break;
		//CPY
		case 0xC0: instructionSet[j] = {j, "CPY", CPY, Imm_, 2, 2}; break;
		case 0xC4: instructionSet[j] = {j, "CPY", CPY, ZPg_, 2, 3}; break;
		case 0xCC: instructionSet[j] = {j, "CPY", CPY, Abs_, 3, 4}; break;
		//Decrement Operations
		case 0xC6: instructionSet[j] = {j, "DEC", DEC, ZPg_, 2, 5}; break;
		case 0xD6: instructionSet[j] = {j, "DEC", DEC, ZPgX, 2, 6}; break;
		case 0xCE: instructionSet[j] = {j, "DEC", DEC, Abs_, 3, 6}; break;
		case 0xDE: instructionSet[j] = {j, "DEC", DEC, AbsX, 3, 7}; break;
		case 0xCA: instructionSet[j] = {j, "DEX", DEX, Imp_, 1, 2}; break;
		case 0x88: instructionSet[j] = {j, "DEY", DEY, Imp_, 1, 2}; break;
		//EOR
		case 0x49: instructionSet[j] = {j, "EOR", EOR, Imm_, 2, 2}; break;
		case 0x45: instructionSet[j] = {j, "EOR", EOR, ZPg_, 2, 3}; break;
		case 0x55: instructionSet[j] = {j, "EOR", EOR, ZPgX, 2, 4}; break;
		case 0x4D: instructionSet[j] = {j, "EOR", EOR, Abs_, 3, 4}; break;
		case 0x5D: instructionSet[j] = {j, "EOR", EOR, AbsX, 3, 4}; break;
		case 0x59: instructionSet[j] = {j, "EOR", EOR, AbsY, 3, 4}; break;
		case 0x41: instructionSet[j] = {j, "EOR", EOR, IndX, 2, 6}; break;
		case 0x51: instructionSet[j] = {j, "EOR", EOR, IndY, 2, 5}; break;
		//Increment Operations
		case 0xE6: instructionSet[j] = {j, "INC", INC, ZPg_, 2, 5}; break;
		case 0xF6: instructionSet[j] = {j, "INC", INC, ZPgX, 2, 6}; break;
		case 0xEE: instructionSet[j] = {j, "INC", INC, Abs_, 3, 6}; break;
		case 0xFE: instructionSet[j] = {j, "INC", INC, AbsX, 3, 7}; break;
		case 0xE8: instructionSet[j] = {j, "INX", INX, Imp_, 1, 2}; break;
		case 0xC8: instructionSet[j] = {j, "INY", INY, Imp_, 1, 2}; break;
		//Jumps
		case 0x4C: instructionSet[j] = {j, "JMP", JMP, Abs_, 3, 3}; break;
		case 0x6C: instructionSet[j] = {j, "JMP", JMP, Ind_, 3, 5}; break;
		case 0x20: instructionSet[j] = {j, "JSR", JSR, Abs_, 3, 6}; break;
		//LDA
		case 0xA9: instructionSet[j] = {j, "LDA", LDA, Imm_, 2, 2}; break;
		case 0xA5: instructionSet[j] = {j, "LDA", LDA, ZPg_, 2, 3}; break;
		case 0xB5: instructionSet[j] = {j, "LDA", LDA, ZPgX, 2, 4}; break;
		case 0xAD: instructionSet[j] = {j, "LDA", LDA, Abs_, 3, 4}; break;
		case 0xBD: instructionSet[j] = {j, "LDA", LDA, AbsX, 3, 4}; break;
		case 0xB9: instructionSet[j] = {j, "LDA", LDA, AbsY, 3, 4}; break;
		case 0xA1: instructionSet[j] = {j, "LDA", LDA, IndX, 2, 6}; break;
		case 0xB1: instructionSet[j] = {j, "LDA", LDA, IndY, 2, 5}; break;
		//LDX
		case 0xA2: instructionSet[j] = {j, "LDX", LDX, Imm_, 2, 2}; break;
		case 0xA6: instructionSet[j] = {j, "LDX", LDX, ZPg_, 2, 3}; break;
		case 0xB6: instructionSet[j] = {j, "LDX", LDX, ZPgY, 2, 4}; break;
		case 0xAE: instructionSet[j] = {j, "LDX", LDX, Abs_, 3, 4}; break;
		case 0xBE: instructionSet[j] = {j, "LDX", LDX, AbsY, 3, 4}; break;
		//LDY
		case 0xA0: instructionSet[j] = {j, "LDY", LDY, Imm_, 2, 2}; break;
		case 0xA4: instructionSet[j] = {j, "LDY", LDY, ZPg_, 2, 3}; break;
		case 0xB4: instructionSet[j] = {j, "LDY", LDY, ZPgX, 2, 4}; break;
		case 0xAC: instructionSet[j] = {j, "LDY", LDY, Abs_, 3, 4}; break;
		case 0xBC: instructionSet[j] = {j, "LDY", LDY, AbsX, 3, 4}; break;
		//LSR
		case 0x4A: instructionSet[j] = {j, "LSR", LSR, Acc_, 1, 2}; break;
		case 0x46: instructionSet[j] = {j, "LSR", LSR, ZPg_, 2, 5}; break;
		case 0x56: instructionSet[j] = {j, "LSR", LSR, ZPgX, 2, 6}; break;
		case 0x4E: instructionSet[j] = {j, "LSR", LSR, Abs_, 3, 6}; break;
		case 0x5E: instructionSet[j] = {j, "LSR", LSR, AbsX, 3, 7}; break;
		//ORA
		case 0x09: instructionSet[j] = {j, "ORA", ORA, Imm_, 2, 2}; break;
		case 0x05: instructionSet[j] = {j, "ORA", ORA, ZPg_, 2, 2}; break;
		case 0x15: instructionSet[j] = {j, "ORA", ORA, ZPgX, 2, 3}; break;
		case 0x0D: instructionSet[j] = {j, "ORA", ORA, Abs_, 3, 4}; break;
		case 0x1D: instructionSet[j] = {j, "ORA", ORA, AbsX, 3, 4}; break;
		case 0x19: instructionSet[j] = {j, "ORA", ORA, AbsY, 3, 4}; break;
		case 0x01: instructionSet[j] = {j, "ORA", ORA, IndX, 2, 6}; break;
		case 0x11: instructionSet[j] = {j, "ORA", ORA, IndY, 2, 5}; break;
		//Stack Operations (Push/Pull)
		case 0x48: instructionSet[j] = {j, "PHA", PHA, Imp_, 1, 3}; break;
		case 0x08: instructionSet[j] = {j, "PHP", PHP, Imp_, 1, 3}; break;
		case 0x68: instructionSet[j] = {j, "PLA", PLA, Imp_, 1, 4}; break;
		case 0x28: instructionSet[j] = {j, "PLP", PLP, Imp_, 1, 4}; break;
		//ROL
		case 0x2A: instructionSet[j] = {j, "ROL", ROL, Acc_, 1, 2}; break;
		case 0x26: instructionSet[j] = {j, "ROL", ROL, ZPg_, 2, 5}; break;
		case 0x36: instructionSet[j] = {j, "ROL", ROL, ZPgX, 2, 6}; break;
		case 0x2E: instructionSet[j] = {j, "ROL", ROL, Abs_, 3, 6}; break;
		case 0x3E: instructionSet[j] = {j, "ROL", ROL, AbsX, 3, 7}; break;
		//ROR
		case 0x6A: instructionSet[j] = {j, "ROR", ROR, Acc_, 1, 2}; break;
		case 0x66: instructionSet[j] = {j, "ROR", ROR, ZPg_, 2, 5}; break;
		case 0x76: instructionSet[j] = {j, "ROR", ROR, ZPgX, 2, 6}; break;
		case 0x6E: instructionSet[j] = {j, "ROR", ROR, Abs_, 3, 6}; break;
		case 0x7E: instructionSet[j] = {j, "ROR", ROR, AbsX, 3, 7}; break;
		//Returns
		case 0x40: instructionSet[j] = {j, "RTI", RTI, Imp_, 1, 6}; break;
		case 0x60: instructionSet[j] = {j, "RTS", RTS, Imp_, 1, 6}; break;
		//SET flags
		case 0x38: instructionSet[j] = {j, "SEC", SEC, Imp_, 1, 2}; break;
		case 0xF8: instructionSet[j] = {j, "SED", SED, Imp_, 1, 2}; break;
		case 0x78: instructionSet[j] = {j, "SEI", SEI, Imp_, 1, 2}; break;
		//STA
		case 0x85: instructionSet[j] = {j, "STA", STA, ZPg_, 2, 3}; break;
		case 0x95: instructionSet[j] = {j, "STA", STA, ZPgX, 2, 4}; break;
		case 0x8D: instructionSet[j] = {j, "STA", STA, Abs_, 3, 4}; break;
		case 0x9D: instructionSet[j] = {j, "STA", STA, AbsX, 3, 5}; break;
		case 0x99: instructionSet[j] = {j, "STA", STA, AbsY, 3, 5}; break;
		case 0x81: instructionSet[j] = {j, "STA", STA, IndX, 2, 6}; break;
		case 0x91: instructionSet[j] = {j, "STA", STA, IndY, 2, 6}; break;
		//STX
		case 0x86: instructionSet[j] = {j, "STX", STX, ZPg_, 2, 3}; break;
		case 0x96: instructionSet[j] = {j, "STX", STX, ZPgY, 2, 4}; break;
		case 0x8E: instructionSet[j] = {j, "STX", STX, Abs_, 3, 4}; break;
		//STY
		case 0x84: instructionSet[j] = {j, "STY", STY, ZPg_, 2, 3}; break;
		case 0x94: instructionSet[j] = {j, "STY", STY, ZPgX, 2, 4}; break;
		case 0x8C: instructionSet[j] = {j, "STY", STY, Abs_, 3, 4}; break;
		//Transfer Registers
		case 0xAA: instructionSet[j] = {j, "TAX", TAX, Imp_, 1, 2}; break;
		case 0xA8: instructionSet[j] = {j, "TAY", TAY, Imp_, 1, 2}; break;
		case 0xBA: instructionSet[j] = {j, "TSX", TSX, Imp_, 1, 2}; break;
		case 0x8A: instructionSet[j] = {j, "TXA", TXA, Imp_, 1, 2}; break;
		case 0x9A: instructionSet[j] = {j, "TXS", TXS, Imp_, 1, 2}; break;
		case 0x98: instructionSet[j] = {j, "TYA", TYA, Imp_, 1, 2}; break;
		//Treat anything else (including NOP) as NOP; preserving the opCode will tell us if it's a real NOP or not
		default: instructionSet[j] = {j, "NOP", NOP, Imp_, 1, 2};
		}
	}
}

void mos6502::Tick() {
	if (skipCycles == 0) {
		Instruction* op = &instructionSet[M->Read(PC)]; //fetch the opcode from the Program Counter's memory address, and retrieve the Instruction object

		uint16_t address; //this will store the FINAL address from which values should be read/written, as a result of the addressing mode

		//check for interrupts and implement them as BRK
		if (reset || nmi || (irq && !(P & fIval))) //IRQ requires the interrupt flag to be off, the others don't
			op = &instructionSet[0x00];

		//Take care of addressing
		address = getAddress(op->Mode);

		PC += op->iLength; //increment PC by the expected amount, as we are done reading params

		//Now we can happily execute the current op, as PC is lined up for the next one
		skipCycles = exec(op, address); //exec returns how many cycles the op "took"

		*Log::Output << DumpRegisters() << std::endl; //we executed an op? log the register states

	} else {
		*Log::Output << "Skipping cycles: " << skipCycles << std::endl;
	}
	skipCycles--;

	reset = false;
}

//check Zero and Negative flags (commonly done together)
void mos6502::flagZN(uint8_t reg) {
	P = (P & ~fNval) | ((reg & (1 << 7)) << fNbit); //clear fN, then set it if reg.7 is set
	P = (P & ~fZval) | ((reg==0) << fZbit); //clear fZ, then set it if reg==0
}

//BCD converters
uint8_t mos6502::bcd2dec(uint8_t val) {
	return ( (val/16*10) + (val%16) );
}
uint8_t mos6502::dec2bcd(uint8_t val) {
	return ( (val/10*16) + (val%10) );
}

uint16_t mos6502::getAddress(AddrModes mode) {
	/*
	 we use a simple bitmask to work out which steps to run for a given addressing mode
	 Bit :         Action
	 0        :        set address to PC+1                                   address = PC+1
	 1        :        read the byte at address                              address = Read(address)
	 2        :        read the word at address                              address = ReadWord(address)
	 3        :        check address+offset for wrapping                     if((address + offset) > 0xffff) skipExtra++
	 4        :        offset address                                        address = (address + offset)
	 5        :        wrap address inside an 8-bit range                    address = address & 0xff
	 6        :        read the word at address (needed twice for ordering)  address = ReadWord(address)
	 7        :        offset register flag; 0 use X for offset, 1 use Y for offset

	 All modes do a final 16-bit wrap, since it doesn't hurt the ones that don't need it
	 address = address & 0xffff

	 Immediate: 00000001 : 0x01
	 ZeroPage : 00000011 : 0x03
	 ZeroPageX: 00110011 : 0x33
	 ZeroPageY: 10110011 : 0xB3
	 Absolute : 00000101 : 0x05
	 AbsoluteX: 00011101 : 0x1D
	 AbsoluteY: 10011101 : 0x9D
	 Indirect : 00000111 : 0x07
	 IndirectX: 01110011 : 0x73
	 IndirectY: 10011111 : 0x9F
	 Relative : 00000011 : 0x03 //relative looks like zero page, but the address is used as an offset by BRANCH instructions
	 Implied  : 00000000 : 0x00 //no address needed for either of these, so step bits are all 0
	 Accumulat: 10000000 : 0x80 //but we use the X/Y flag bit to differentiate them
	 */
	uint16_t address = 0; //the address we will return; initialise as 0 for Implied/Accumulator mode
	uint8_t offset = (mode & (1 << 7)) ? Y : X; //set the offset value to that of the expected register
	xPage = false; //assume we haven't crossed a page boundary until we do

	//conditionally execute instructions in order as per the bitmask
	if (mode & 1) {
		address = PC + 1;
		*Log::Output << "getAddress: Set address to PC+1: 0x" << std::hex
				<< std::setw(4) << std::setfill('0') << address << std::endl;
	}
	if (mode & (1 << 1)) {
		address = M->Read(address);
		*Log::Output << "getAddress: Read(address): 0x" << std::hex
				<< std::setw(4) << std::setfill('0') << address << std::endl;
	}
	if (mode & (1 << 2)) {
		address = M->ReadWord(address);
		*Log::Output << "getAddress: ReadWord(address): 0x" << std::hex
				<< std::setw(4) << std::setfill('0') << address << std::endl;
	}
	if (mode & (1 << 3)) {
		if ((address + offset) > (address | 0xff))
			xPage = true;
		*Log::Output << "getAddress: Check for page wrap: "
				<< (xPage ? "true" : "false") << std::endl;
	}
	if (mode & (1 << 4)) {
		address = (address + offset);
		*Log::Output << "getAddress: Offset address: 0x" << std::hex
				<< std::setw(4) << std::setfill('0') << address << std::endl;
	}
	if (mode & (1 << 5)) {
		address = address & 0xff;
		*Log::Output << "getAddress: Truncate address to 8-bit range: 0x"
				<< std::hex << std::setw(2) << std::setfill('0') << address
				<< std::endl;
	}
	if (mode & (1 << 6)) {
		address = M->ReadWord(address);
		*Log::Output << "getAddress: ReadWord(address): 0x" << std::hex
				<< std::setw(4) << std::setfill('0') << address << std::endl;
	}
	address = address & 0xffff;
	*Log::Output << "getAddress: Final Address: 0x" << std::hex << std::setw(4)
			<< std::setfill('0') << address << std::endl;
	return address;
}

std::string mos6502::DumpRegisters() {
	std::ostringstream ss;
	ss << "PC = 0x" << std::hex << std::setw(4) << std::setfill('0') << PC
			<< std::endl;
	ss << "A  = 0x" << std::hex << std::setw(2) << std::setfill('0')
			<< (unsigned) A << "(" << std::dec << (unsigned) A << ")"
			<< std::endl;
	ss << "X  = 0x" << std::hex << std::setw(2) << std::setfill('0')
			<< (unsigned) X << "(" << std::dec << (unsigned) X << ")"
			<< std::endl;
	ss << "Y  = 0x" << std::hex << std::setw(2) << std::setfill('0')
			<< (unsigned) Y << "(" << std::dec << (unsigned) Y << ")"
			<< std::endl;
	ss << "S  = 0x" << std::hex << std::setw(2) << std::setfill('0')
			<< (unsigned) S << "(" << std::dec << (unsigned) S << ")"
			<< std::endl;
	//status flags CZIDBRVN
	ss << "fC = " << ((P & fCval) ? 1 : 0)
	  << " fZ = " << ((P & fZval) ? 1 : 0)
	  << " fI = " << ((P & fIval) ? 1 : 0)
	  << " fD = " << ((P & fDval) ? 1 : 0)
	  << " fB = " << ((P & fBval) ? 1 : 0)
	  << " fR = " << ((P & fRval) ? 1 : 0)
	  << " fV = " << ((P & fVval) ? 1 : 0)
	  << " fN = " << ((P & fNval) ? 1 : 0)
	  << std::endl;

	std::string s = ss.str();
	return s;
}

//all our instructions
int mos6502::exec(Instruction* op, uint16_t address) {
	bool bitbuddy; //used to store single bit values here and there
	int result; //used for ADC/SBC as it needs to be higher than 8-bit precision

	int skipExtra = 0; //how many extra cycles to skip, if any
	uint8_t value = 0; //set the value up here as many instructions will need it
	if (op->Mode == Acc_)
		value = A; //here we handle Accumulator "addressing" mode, when we set the value
	else
		value = M->Read(address);
	*Log::Output << "exec: Instruction: " << op->Mnemonic << " 0x" << std::hex
			<< std::setw(2) << std::setfill('0') << (unsigned) op->OpCode
			<< std::endl;
	*Log::Output << "exec: Value: " << std::hex << std::setw(2)
			<< std::setfill('0') << (unsigned) value << "(" << std::dec
			<< (unsigned) value << ")" << std::endl;
	//now switch on the basic instruction and execute it with the provided address/value
	switch (op->ExecCode) {

	//ADC
	case ADC:
		result = value + A + ((P & fCval) ? 1 : 0); //always do the Binary calculation, so we can set flags off it
		P = (P & ~fVval) | (((A & (1 << 7)) != (result & (1 << 7))) << fVbit); //set fV if A.7 is changing
		flagZN(result & 0xff);

		if(P & fDval) { //BCD mode
			result = bcd2dec(value) + bcd2dec(A) + ((P & fCval) ? 1 : 0); //redo the calculation
			if(result > 99) {
				P |= fCval;
				result -= 100; //wrap in the range 0-99
			} else {
				P &= ~fCval;
			}
		} else { //Binary
			P = (P & ~fCval) | ((result > 0xff) << fCbit); //set carry if result > 255
		}

		value = result & 0xff; //truncate to 8-bit for safety, and set value
		A = (P & fDval) ? dec2bcd(value) : value; //set A to the final result, converting if necessary
		break;

	//AND
	case AND:
		A = A & value;
		flagZN(A);
		if(xPage) skipExtra++;
		break;

	//ASL
	case ASL:
		P = (P & ~fCval) | ((value & (1 << 7)) << fCbit); //fC = value.7
		value = (value << 1) & 0xFE; //& 0xfe clears value.0
		flagZN(value);
		if(op->Mode == Acc_) A = value;
		else M->Write(address, value);
		break;

	//BIT
	case BIT:
		flagZN(A & value); //fZ set by (A & value), don't worry about fN
		P = (P & ~fVval) | ((value & (1 << 6)) << fVbit); //fV = value.6
		P = (P & ~fNval) | ((value & (1 << 7)) << fNbit); //fN = value.7
		break;

	//Branches
	case BCC:
		if(!(P & fCval)) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;
	case BCS:
		if(P & fCval) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;
	case BEQ:
		if(P & fZval) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;
	case BMI:
		if(P & fNval) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;
	case BNE:
		if(!(P & fZval)) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;
	case BPL:
		if(!(P & fNval)) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;
	case BVC:
		if(!(P & fVval)) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;
	case BVS:
		if(P & fVval) {
			PC += value;
			skipExtra++;
			if(xPage) skipExtra++;
		}
		break;

	//BRK / interrupts
	case BRK:
		//here we have to fiddle stuff for interrupts
		if (!reset && !nmi && !irq) //software BRK instruction
			PC++; //increment PC again, cos BRK is...well: BRKen.
		//stick current state on the stack (unless resetting) and decrement S (EVEN if resetting)
		if (!reset)
			M->Write((0x1 << 8 | S), (PC >> 8)); //stick high byte of PC on the stack
		S--;
		if (!reset)
			M->Write((0x1 << 8 | S), PC & 0xff); //stick low byte of PC on the stack
		S--;
		if (!reset)
			M->Write((0x1 << 8 | S), P | fBval); //stick status register on the stack, with the Break flag set
		S--;

		//disable interrupts while the interrupt executes
		P |= fIval;

		//set address based on Break type
		if (!reset && !nmi) //must be IRQ or BRK
			address = 0xfffe;
		else if (reset)
			address = 0xfffc;
		else if (nmi)
			address = 0xfffa;

		PC = M->ReadWord(address);
		break;

	//Clear Flags
	case CLC: P &= ~fCval; break;
	case CLD: P &= ~fDval; break;
	case CLI: P &= ~fIval; break;
	case CLV: P &= ~fVval; break;

	//Compare Operations
	case CMP:
		P = (P & ~fCval) | ((A >= value) << fCbit); //set fC if A >= value
		value = A - value;
		flagZN(value);
		if(xPage) skipExtra++;
		break;
	case CPX:
		P = (P & ~fCval) | ((X >= value) << fCbit); //set fC if X >= value
		value = X - value;
		flagZN(value);
		break;
	case CPY:
		P = (P & ~fCval) | ((Y >= value) << fCbit); //set fC if Y >= value
		value = Y - value;
		flagZN(value);
		break;

	//Decrement Operations
	case DEC:
		value = (value-1) & 0xff;
		M->Write(address,value);
		flagZN(value);
		break;
	case DEX: X--; flagZN(X); break;
	case DEY: Y--; flagZN(Y); break;

	//EOR
	case EOR:
		A = A ^ value;
		flagZN(A);
		if(xPage) skipExtra++;
		break;

	//Increment Operations
	case INC:
		value = (value+1) & 0xff;
		M->Write(address,value);
		flagZN(value);
		break;
	case INX: X++; flagZN(X); break;
	case INY: Y++; flagZN(Y); break;

	//Jumps
	case JMP: PC = address; break;
	case JSR:
		M->Write((0x1 << 8 | S--), ((PC-1) >> 8)); //stick high byte of PC-1 on the stack
		M->Write((0x1 << 8 | S--), (PC-1) & 0xff); //stick low byte of PC-1 on the stack
		PC = address;
		break;

	//Load Registers
	case LDA:
		A = value; //Load A with the value
		flagZN(A); //flag check against A
		if(xPage) skipExtra++; //skip an extra cycle if a page boundary was crossed during addressing
		break;
	case LDX:
		X = value; //Load X with the value
		flagZN(X); //flag check against X
		if(xPage) skipExtra++; //skip an extra cycle if a page boundary was crossed during addressing
		break;
	case LDY:
		Y = value; //Load Y with the value
		flagZN(Y); //flag check against Y
		if(xPage) skipExtra++; //skip an extra cycle if a page boundary was crossed during addressing
		break;

	//LSR
	case LSR:
		P = (P & ~fCval) | ((value & (1 << 0)) << fCbit); //fC = value.0
		value = (value >> 1) & 0x7F; //&7f clears value.7
		flagZN(value);
		if(op->Mode == Acc_) A = value;
		else M->Write(address, value);
		break;

	//ORA
	case ORA:
		A = A | value;
		flagZN(A);
		if(xPage) skipExtra++;
		break;

	//Stack operations (Push/Pull)
	case PHA: M->Write((0x1 << 8 | S--), A); break;
	case PHP: M->Write((0x1 << 8 | S--), P); break;
	case PLA: A = M->Read(0x1 << 8 | ++S); flagZN(A); break;
	case PLP: P = M->Read(0x1 << 8 | ++S); break;

	//ROL
	case ROL:
		bitbuddy = (value & (1<<7)); //store bit7
		//perform the shift
		value = (value << 1) & 0xFE; //the & ensures bit 0 is 0
		//set bit 0 based on Carry flag
		if(P & fCval) value |= (1<<0);
		else value &= ~(1<<0);
		flagZN(value); //check flags
		//set carry flag based on bit7
		if(bitbuddy) P |= fCval;
		else P &= ~fCval;
		//write the modified value back to memory, or A
		if(op->Mode == Acc_) A = value;
		else M->Write(address, value);
		break;

	//ROR
	case ROR:
		bitbuddy = (value & (1<<0)); //store bit0
		//perform the shift
		value = (value >> 1) & 0x7F; //the & ensures bit 7 is 0
		//set bit 7 based on Carry flag
		if(P & fCval) value |= (1<<7);
		else value &= ~(1<<7);
		flagZN(value); //check flags
		//set carry flag based on bit0
		if(bitbuddy) P |= fCval;
		else P &= ~fCval;
		//write the modified value back to memory, or A
		if(op->Mode == Acc_) A = value;
		else M->Write(address, value);
		break;

	//Returns
	case RTI:
		P = M->Read(S++);
		PC = M->ReadWord(S++);
		S++; //increment again, to account for the fact we just read a word in one go
		break;
	case RTS:
		P = M->Read(S++);
		PC = M->ReadWord(S++);
		S++; //increment again, to account for the fact we just read a word in one go
		PC++; //also increment PC for returning, unlike RTI
		break;

	//SBC
	case SBC:
		if(P & fDval) { //BCD
			result = bcd2dec(value) - bcd2dec(A) - ((P & fCval) ? 0 : 1);
			P = (P & fVval) | ((result > 99 || result < 0) << fVbit); //set fV if out of range, this is expected to be inaccurate on the hardware
		} else { //Binary
			result = value - A - ((P & fCval) ? 0 : 1);
			P = (P & fVval) | ((result > 127 || result < -128) << fVbit); //set fV if out of range
		}

		value = result & 0xff;
		flagZN(value); //fZ works but fN will be inaccurate for BCD? as per the hardware
		P = (P & fCval) | ((value >= 0) << fCbit); //this works for Binary and BCD

		A = (P & fDval) ? dec2bcd(value) : value; //set A to the final result, converting if necessary
		break;

	//Set Flags
	case SEC: P |= fCval;	break;
	case SED: P |= fDval;	break;
	case SEI: P |= fIval;	break;

	//Store registers
	case STA: M->Write(address, A); break;
	case STX: M->Write(address, X); break;
	case STY: M->Write(address, Y); break;

	//Transfer Registers
	case TAX: X = A; flagZN(X); break;
	case TAY: Y = A; flagZN(Y); break;
	case TSX: X = S; flagZN(X); break;
	case TXA: A = X; flagZN(A); break;
	case TXS: S = X; break;
	case TYA: A = Y; flagZN(A); break;

	//NOP
	case NOP: break;
	}
	return op->tCount + skipExtra;
}
