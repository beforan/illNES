namespace illNES.CPU
{
    public interface I6502
    {
        /// <summary>
        /// Perform a tick of the emulated CPU.
        /// In theory this would be one 6502 cycle,
        /// but that may depend on implementation.
        /// </summary>
        void Tick();

        /// <summary>
        /// Dump the current values in all the CPU registers.
        /// </summary>
        /// <returns></returns>
        string DumpRegisters();
    }
}
