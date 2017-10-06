using System.Collections.Generic;
using illNES.CPU.Types;

namespace illNES.CPU
{
    internal class InstructionSet
    {
        //Initialised with the ops we care about
        private static readonly IList<Operation> Operations = new List<Operation>
        {
            //TODO pop these as we implement them
            //ADC - examples porting from C++
            //[0x69] = new Operation(Instructions.ADC, AddressModes.Immediate,2,2),
            //[0x65] = new Operation(Instructions.ADC, AddressModes.ZeroPage, 2, 3),
            //[0x75] = new Operation(Instructions.ADC, AddressModes.ZeroPageX, 2, 4),
            //[0x6D] = new Operation(Instructions.ADC, AddressModes.Absolute, 2, 4),
            //[0x7D] = new Operation(Instructions.ADC, AddressModes.AbsoluteX, 3, 4),
            //[0x79] = new Operation(Instructions.ADC, AddressModes.AbsoluteY, 3, 4),
            //[0x61] = new Operation(Instructions.ADC) {j, "ADC", ADC, IndX, 2, 6},
            //[0x71] = new Operation(Instructions.ADC) {j, "ADC", ADC, IndY, 2, 5},
        };

        /// <summary>
        /// Get the Operation for a given OpCode.
        /// If the OpCode is not implemented, a NOP is returned.
        /// </summary>
        /// <param name="key">The OpCode</param>
        /// <returns>The Operation for the provided Code</returns>
        public Operation this[byte key] =>
            Operations[key] ??
            (Operations[key] =
                new Operation(Instructions.NOP, AddressModes.Implied, 1, 2));
    }
}
