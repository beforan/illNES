[![Build Status](https://travis-ci.org/beforan/illNES.svg?branch=develop)](https://travis-ci.org/beforan/illNES)
# illNES

An ill-conceived NES emulator.

# Definitions

Just chucking these here for now, until they are moved to a home in the Wiki.

## MOS6502.

The following terminology is used throughout the CPU code.

- **Instruction** - represents an action the the cpu can take, usually on some memory address and/or CPU register
- **Address** - An indexed location in RAM, or the index itself.
- **Register** - essentially a fixed size variable that can hold a value. Some registers have special meaning.
- **Mnemonic** - A 3 letter string that represents an instruction.
- **Addressing Mode** - The mode by which the address for the instruction to operate on is acquired.
- **Operation** - A combination of an Instruction and a given Addressing Mode.
- **OpCode** - A byte value that represents a unique operation.