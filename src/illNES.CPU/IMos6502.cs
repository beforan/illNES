using illNES.CPU.Types;

namespace illNES.CPU
{
    public interface IMos6502
    {
        /// <summary>
        /// Accumulator register
        /// </summary>
        byte A { get; }

        /// <summary>
        /// General purpose X register
        /// </summary>
        byte X { get; }

        /// <summary>
        /// General purpose Y register
        /// </summary>
        byte Y { get; }

        /// <summary>
        /// Stack pointer
        /// </summary>
        byte S { get; }

        /// <summary>
        /// Program Counter
        /// </summary>
        ushort PC { get; }

        /// <summary>
        /// CPU State Flags
        /// </summary>
        PFlags P { get; }

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
