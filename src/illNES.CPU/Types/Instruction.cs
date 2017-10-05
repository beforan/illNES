namespace illNES.CPU.Types
{
    /// <summary>
    /// Data structure for a single CPU instruction
    /// </summary>
    internal class Instruction
    {
        /// <summary>
        /// The instruction's OpCode value
        /// </summary>
        public byte OpCode { get; set; }

        /// <summary>
        /// The friendly Mnemonic for this instruction.
        /// This is not unique as some Mnemonics are
        /// multiple ops with different addressing modes.
        /// </summary>
        public string Mnemonic { get; set; }

        /// <summary>
        /// Enum value for the OpCode, purely for nicer code
        /// </summary>
        public Ops ExecCode { get; set; }

        /// <summary>
        /// The addressing mode of this instruction
        /// </summary>
        public AddressModes Mode { get; set; }

        /// <summary>
        /// The byte length of this instruction in memory
        /// </summary>
        public int Length { get; set; }

        /// <summary>
        /// The default number of CPU cycles this instruction expends.
        /// Extra cycles may be conditionally expended.
        /// </summary>
        public int Cycles{ get; set; }
    }
}
