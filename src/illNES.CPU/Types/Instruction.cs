namespace illNES.CPU.Types
{
    /// <summary>
    /// Data structure for a single CPU instruction
    /// </summary>
    internal class Instruction
    {

        public Instruction(Ops execCode, AddressModes mode, int length, int cycles)
        {
            ExecCode = execCode;
            Mode = mode;
            Length = length;
            Cycles = cycles;
        }

        //TODO not sure we need this as it's the key in the instruction set anyway?
        ///// <summary>
        ///// The instruction's OpCode value
        ///// </summary>
        //public byte OpCode { get; set; }

        /// <summary>
        /// The friendly Mnemonic for this instruction.
        /// This is not unique as some Mnemonics are
        /// multiple ops with different addressing modes.
        /// </summary>
        public string Mnemonic => ExecCode.ToString();

        /// <summary>
        /// Enum value for the OpCode.
        /// - Allows our code to read more nicely,
        /// - Provides Mnemonic automatically without magic strings
        /// </summary>
        public Ops ExecCode { get; private set; }

        /// <summary>
        /// The addressing mode of this instruction
        /// </summary>
        public AddressModes Mode { get; private set; }

        /// <summary>
        /// The byte length of this instruction in memory
        /// </summary>
        public int Length { get; private set; }

        /// <summary>
        /// The default number of CPU cycles this instruction expends.
        /// Extra cycles may be conditionally expended.
        /// </summary>
        public int Cycles{ get; private set; }
    }
}
