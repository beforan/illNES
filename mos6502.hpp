#ifndef MOS6502_H
#define MOS6502_H

#include <cstdint>
#include "BaseMemoryInterface.hpp"

enum PFlagValues { fCval = 0x1, fZval = 0x2, fIval = 0x4, fDval = 0x8,
					fBval = 0x10, fRval = 0x20, fVval = 0x40, fNval = 0x80 };
enum PFlagBits { fCbit = 0, fZbit, fIbit, fDbit, fBbit, fRbit, fVbit, fNbit };

enum AddrModes {
  Abs_ = 0x5, AbsX = 0x1D, AbsY = 0x9D,
  Acc_ = 0x80, Imm_ = 0x1, Imp_ = 0, Rel_ = 0x3,
  Ind_ = 0x7, IndX = 0x73, IndY = 0x9F,
  ZPg_ = 0x3, ZPgX = 0x33, ZPgY = 0xB3
};
enum Ops { //this is reaaaally just to make life slightly nicer for me in the current exec() implementation
  ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI,
  BNE, BPL, BRK, BVC, BVS, CLC, CLD, CLI,
  CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR,
  INC, INX, INY, JMP, JSR, LDA, LDX, LDY, 
  LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL,
  ROR, RTI, RTS, SBC, SEC, SED, SEI, STA,
  STX, STY, TAX, TAY, TSX, TXA, TXS, TYA
};

struct Instruction {
  uint8_t OpCode;
  std::string Mnemonic;
  Ops ExecCode;
  AddrModes Mode;
  int iLength;
  int tCount;
};

class mos6502 {
public:
  mos6502(BaseMemoryInterface* m);
  ~mos6502();
  void Tick();
  
  std::string DumpRegisters();
protected:
  //internal stuff, used for our implementation
  uint16_t getAddress(AddrModes mode); //retrieves a target memory address by the appropriate mode 
  int exec(Instruction* op, uint16_t address); //used to execute an op using the provided address, and returns how many cycles the op required
  void flagZN(uint8_t reg); //checks the provided value (from a register!) and sets the Z and N flags accordingly
  uint8_t bcd2dec(uint8_t val);
  uint8_t dec2bcd(uint8_t val);
  
  bool reset, nmi, irq; //internal interrupt flags
  int skipCycles; //how many more cycles to skip before we fetch the next opcode
  bool xPage; //did we cross a page boundary the last time we addressed? some ops need more cycles if true
  BaseMemoryInterface* M; //Pointer to Memory Interface
  Instruction instructionSet[256]; //lookup table for opcodes
  
  //registers
  uint8_t A, X, Y, S, P;
  uint16_t PC;
};

#endif
