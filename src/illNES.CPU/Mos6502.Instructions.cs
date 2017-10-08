using illNES.CPU.Types;

namespace illNES.CPU
{
    public partial class Mos6502
    {
        /* 
         * This file contains a method for every instruction.
         * 
         * Signatures are normalised so they can be passed as Funcs
         * and called with ease (see `Mos6502.Exec()`)
         * 
         * (AddressModes mode, ushort address, byte value)
         * 
         * Any parameter not used by a method will be underscore named.
         * 
         * All instruction methods return the number of EXTRA cycles to skip
         * above and beyond those in the Operation metadata
         * 
         * Because all instructions are methods on Mos6502
         * they already have access to the CPU's state (registers, ram etc)
         */

        private int Nop(AddressModes _, ushort __, byte ___) => 0;
    }
}
