#include <iostream>
#include <cstdint>
#include "mos6502.hpp"
#include "BaseMemoryInterface.hpp"

using namespace std; //makes things easier for testing...

int main() {
  //Memory Read/Write testing
  BaseMemoryInterface* mem = new BaseMemoryInterface();
  
  //initial CPU testing
  mos6502* cpu = new mos6502(mem);
  
  //some vars to test reads with
  uint8_t byte1,byte2;
  uint16_t word1;
  
  //do some reading and writing, and check output
  mem->Write(0x1,99); //write a simple 8-bit number in zero-page
  cout << "[0x0001] wrote value 99 (0x63)" << endl;
  
  byte1 = mem->Read(0x1); //check the write with a read :)
  cout << "[0x0001] read byte: " << dec << (unsigned)byte1 << " (" << hex << (unsigned)byte1 << ")" <<endl;
  
  //write a 16 bit number (1000) as two 8-bit values to consecutive 16-bit addresses
  mem->Write(0xff01,0xe8); //LSB to lower address: 0xe8 = 232
  cout << "[0xff01] wrote value 232 (0xe8)" << endl;
  mem->Write(0xff02,0x03); //MSB to higher address: 0x03 = 3
  cout << "[0xff02] wrote value 3 (0x03)" << endl;
  
  //confirm the writes with 8-bit reads
  byte1 = mem->Read(0xff01);
  cout << "[0xff01] read byte: " << dec << (unsigned)byte1 << " (" << hex << (unsigned)byte1 << ")" <<endl;
  byte2 = mem->Read(0xff02);
  cout << "[0xff02] read byte: " << dec << (unsigned)byte2 << " (" << hex << (unsigned)byte2 << ")" <<endl;
  
  //try and read both consecutive bytes as a little-endian word, knowing the expected output
  word1 = mem->ReadWord(0xff01);
  cout << "[0xff01,0xff02] read word: " << dec << word1 << " (" << hex << word1 << ")" <<endl; //expected value = 1000 (0x03e8), big-endian value = 59395 (0xe803)
  
  //test wraparound ;)
  //write a 16 bit number (1000) as two 8-bit values to wrapped 16-bit addresses
  mem->Write(0xffff,0xe8); //LSB to "lower" address: 0xe8 = 232
  cout << "[0xffff] wrote value 232 (0xe8)" << endl;
  mem->Write(0x0,0x03); //MSB to "higher" address: 0x03 = 3
  cout << "[0x0000] wrote value 3 (0x03)" << endl;
  
  //confirm the writes with 8-bit reads
  byte1 = mem->Read(0xffff);
  cout << "[0xffff] read byte: " << dec << (unsigned)byte1 << " (" << hex << (unsigned)byte1 << ")" <<endl;
  byte2 = mem->Read(0x0);
  cout << "[0x0000] read byte: " << dec << (unsigned)byte2 << " (" << hex << (unsigned)byte2 << ")" <<endl;
  
  //try and read both "consecutive" bytes as a little-endian word, knowing the expected output
  word1 = mem->ReadWord(0xffff);
  cout << "[0xffff,0x0000] read word: " << dec << word1 << " (" << hex << word1 << ")" <<endl; //expected value = 1000 (0x03e8), big-endian value = 59395 (0xe803)
  
  cin.get(); //wait for input so we can see the results
  return 0;
}