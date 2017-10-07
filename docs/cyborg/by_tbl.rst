============================
Instruction set Lookup Table
============================

===== ======
I-Len T-Cnt
Mnemonic
------------
Address Mode
============

I-Len
    Instruction length. The number of bytes of memory required to store the instruction.
T-Cnt
    Timing Cycle (T-State) count. The number of clock cycles required to execute the instruction.
Mnemonic
    The abbreviated "human readable" identity of the instruction.
Address Mode
    The :doc:`memory addressing mode <addr_mode>` used by the instruction.

The opcode for any instruction may be composed by reading the row and column it is in:

- "ASL" in "Absolute" addressing mode is in row "1-", column "-E"
- Therefore it's opcode (hexadecimal representation) is "1E" (or $1E or 0x1E)