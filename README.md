# illNES

illNES is an ill-conceived NES emulator project, that came to life due to a temporary obsession with emulating a MOS6502 (what can I say, I grew up with a C64 and got nostalgic).

It began due to a desire to get more comfortable with C++, bitwise operations and Eclipse.

However, the c++ version is abandoned, and the incomplete MOS6502 is retained here in this orphaned branch for historical reasons.

## Status

illNES's original C++ version is woefully incomplete, and now abandoned in favour of the C# version.

Current status:

* Incomplete MOS6502 CPU
* Complete DMA 64k MemoryInterface to the CPU
* Complete Quick simple memory mapped display module
* No cart loading
* No NES PPU (display)
* No Input
* No NES APU (sound)
* No game compatibility!

## How do I get set up?

Please see the below section about contribution. If you want to use code from this repository in a project, clone the repo and take the bits you need, or fork it if you like. The code is under the ISC License, so please retain the license text and copyright if you use anything!

That said, if you just want to get what's here and compile and run stuff, here are some notes:

### Summary of set up
You'll need a C++ compiler and linker, not too much else.

illNES has been successfully compiled under the following environments:

* MinGW (GCC)
* Visual Studio 2013 (MSBuild)
  * Create project from existing files
  * Add SDL2 via Nuget
  * Exclude any files you don't want the entry point from (e.g. `mem-test.cpp`)
  * ????
  * Profit!

### Configuration
There is none at this time

There are currently two programs you can compile; the entry points are in the following files:

* mem-test.cpp
* cpu-test.cpp

Please don't try and compile with both those files; they both declare `main()`!

Other than those two, you should probably include all other source files ;)

### Dependencies
Currently qndDisplay depends upon SDL2, so you'll need to link against that for your platform. In Modern Visual Studio this can be added via Nuget :)

No other dependencies at this time.

### How to run tests
#### mem-test.cpp
mem-test is really a self contained test program for the basic DMA-style MemoryInterface. Just compile it and run it!

#### cpu-test.cpp
cpu-test's `main()` requires one parameter, and accepts an optional second:

* First parameter is for an assembled binary to run on the CPU.
* Second parameter is the path to a plain text log file.

The assembled binary must be valid 6502 assembly, and only use opcodes supported by the emulated CPU.

Look in the `assembly` branch for some of the tests I've used in development.

The branch contains some assembly source in `src` and also some binaries of those source files in `bin`.

The log file doesn't need to exist already. If no log file path is supplied, the program will use stdout instead. This isn't unique to cpu-test.cpp - it's thanks to the Log module.

## Contribution

This C++ version is now abandoned, so no contribution is accepted to this branch.

However, the code is available under the ISC License, so feel free to take anything here and build it to try and run things, or use it in your own projects or as the basis for your own 6502 computer emulator or anything else you might like!
