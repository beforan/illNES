using System.Collections.Generic;
using illNES.CPU.Types;

namespace illNES.CPU
{
    internal class InstructionSet
    {
        //Initialised with the ops we care about
        private static readonly IList<Instruction> Instructions = new List<Instruction>
        {
            //TODO pop these as we implement them
            //ADC - examples porting from C++
            //[0x69] = new Instruction(Ops.ADC, AddressModes.Immediate,2,2),
            //[0x65] = new Instruction(Ops.ADC, AddressModes.ZeroPage, 2, 3),
            //[0x75] = new Instruction(Ops.ADC, AddressModes.ZeroPageX, 2, 4),
            //[0x6D] = new Instruction(Ops.ADC, AddressModes.Absolute, 2, 4),
            //[0x7D] = new Instruction(Ops.ADC, AddressModes.AbsoluteX, 3, 4),
            //[0x79] = new Instruction(Ops.ADC, AddressModes.AbsoluteY, 3, 4),
            //[0x61] = new Instruction(Ops.ADC) {j, "ADC", ADC, IndX, 2, 6},
            //[0x71] = new Instruction(Ops.ADC) {j, "ADC", ADC, IndY, 2, 5},
        };

        /// <summary>
        /// Get the instruction metadata for a given opcode.
        /// If the OpCode is not implemented, a NOP Instruction is returned.
        /// </summary>
        /// <param name="key">The OpCode</param>
        /// <returns>An Instruction containing all the relevant metadata</returns>
        public Instruction this[byte key] =>
            Instructions[key] ??
            (Instructions[key] =
                new Instruction(Ops.NOP, AddressModes.Implied, 1, 2));
    }
}
