using illNES.CPU.Types;
using System.Collections.Generic;

namespace illNES.CPU
{
    public partial class Mos6502
    {
        private IList<Operation> BuildInstructionSet()
        {
            var nop = new Operation(Instructions.NOP, AddressModes.Implied, 1, 2, Nop);
            var set = new List<Operation>(new Operation[byte.MaxValue])
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
                
                //BRK
                [0x00] = new Operation(Instructions.BRK, AddressModes.Implied, 1, 7, Brk),

                //CLEAR flags
                [0x18] = new Operation(Instructions.CLC, AddressModes.Implied, 1, 2, Clc),
                [0xD8] = new Operation(Instructions.CLD, AddressModes.Implied, 1, 2, Cld),
                [0x58] = new Operation(Instructions.CLI, AddressModes.Implied, 1, 2, Cli),
                [0xB8] = new Operation(Instructions.CLV, AddressModes.Implied, 1, 2, Clv),

                //SET flags
                [0x38] = new Operation(Instructions.SEC, AddressModes.Implied, 1, 2, Sec),
                [0xF8] = new Operation(Instructions.SED, AddressModes.Implied, 1, 2, Sed),
                [0x78] = new Operation(Instructions.SEI, AddressModes.Implied, 1, 2, Sei),

                //Transfer Registers
                [0xAA] = new Operation(Instructions.TAX, AddressModes.Implied, 1, 2, Tax),
		        [0xA8] = new Operation(Instructions.TAY, AddressModes.Implied, 1, 2, Tay),
		        [0xBA] = new Operation(Instructions.TSX, AddressModes.Implied, 1, 2, Tsx),
		        [0x8A] = new Operation(Instructions.TXA, AddressModes.Implied, 1, 2, Txa),
		        [0x9A] = new Operation(Instructions.TXS, AddressModes.Implied, 1, 2, Txs),
		        [0x98] = new Operation(Instructions.TYA, AddressModes.Implied, 1, 2, Tya)
        };

            //any we didn't specify above should be NOP
            for (int i = 0; i < set.Count; i++)
                set[i] = set[i] ?? nop;
            
            return set;
        }
    }
}
