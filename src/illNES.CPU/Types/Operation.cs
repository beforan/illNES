using System;

namespace illNES.CPU.Types
{
    /// <summary>
    /// Data structure for a single CPU operation
    /// </summary>
    internal class Operation
    {
        public Operation(Instructions instruction, AddressModes mode, ushort length, int cycles,
            Action<AddressModes, ushort, byte> exec)
        {
            Instruction = instruction;
            Mode = mode;
            Length = length;
            Cycles = cycles;
            Exec = exec;
        }

        /// <summary>
        /// The friendly Mnemonic for this operation's instruction.
        /// This is not unique as some instructions are in
        /// multiple ops with different addressing modes.
        /// </summary>
        public string Mnemonic => Instruction.ToString();

        /// <summary>
        /// Enum value for the Instruction.
        /// - Allows our code to read more nicely,
        /// - Provides Mnemonic automatically without magic strings
        /// </summary>
        public Instructions Instruction { get; private set; }

        /// <summary>
        /// The addressing mode of this Operation
        /// </summary>
        public AddressModes Mode { get; private set; }

        /// <summary>
        /// The byte length of this Operation in memory.
        /// - The first byte is the OpCode
        /// - One or Two bytes may follow with parameters depending on the Addressing Mode
        /// </summary>
        public ushort Length { get; private set; }

        /// <summary>
        /// The default number of CPU cycles this Operation expends.
        /// Extra cycles may be conditionally expended.
        /// </summary>
        public int Cycles { get; private set; }

        /// <summary>
        /// The code to execute
        /// </summary>
        public Action<AddressModes, ushort, byte> Exec { get; private set; }
    }
}
