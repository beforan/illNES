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

        private int Brk(AddressModes _, ushort address, byte __)
        {
            //here we have to fiddle stuff for interrupts
            if (!_reset && !_nmi && !_irq) //software BRK instruction
                PC++; //increment PC again, cos BRK is, well, BRKen.
            
            //stick current state on the stack (unless resetting) and decrement S (EVEN if resetting)
            if (!_reset)
                _m.Write((ushort)(0x1 << 8 | S), (byte)(PC >> 8)); //stick high byte of PC on the stack
            S--;
            if (!_reset)
                _m.Write((ushort)(0x1 << 8 | S), (byte)(PC & 0xff)); //stick low byte of PC on the stack
            S--;
            if (!_reset)
                _m.Write((ushort)(0x1 << 8 | S), (byte)(P | PFlags.B)); //stick status register on the stack, with the Break flag set
            S--;

            //disable interrupts while the interrupt executes
            P |= PFlags.I;

            //set address based on Break type
            if (!_reset && !_nmi) //must be IRQ or BRK
                address = 0xfffe;
            else if (_reset)
                address = 0xfffc;
            else if (_nmi)
                address = 0xfffa;

            PC = _m.ReadWord(address);

            return 0;
        }
    }
}
