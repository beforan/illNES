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
         * Instructions needing to skip extra cycles due to page crossing
         * or any other reason may simply increment _skipCycles directly.
         * 
         * Because all instructions are methods on Mos6502
         * they already have access to the CPU's state (registers, ram etc)
         */

        //BRK
        private void Brk(AddressModes _, ushort address, byte ___)
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
        }

        //CLEAR flags
        private void Clc(AddressModes _, ushort __, byte ___)
            => P &= ~PFlags.C;
        private void Cld(AddressModes _, ushort __, byte ___)
            => P &= ~PFlags.D;
        private void Cli(AddressModes _, ushort __, byte ___)
            => P &= ~PFlags.I;
        private void Clv(AddressModes _, ushort __, byte ___)
            => P &= ~PFlags.V;

        //SET flags
        private void Sec(AddressModes _, ushort __, byte ___)
            => P |= PFlags.C;
        private void Sed(AddressModes _, ushort __, byte ___)
            => P |= PFlags.D;
        private void Sei(AddressModes _, ushort __, byte ___)
            => P |= PFlags.I;

        //NOP
        private void Nop(AddressModes _, ushort __, byte ___) { }
    }
}
