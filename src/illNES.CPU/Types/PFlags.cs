using System;

namespace illNES.CPU.Types
{
    [Flags]
    public enum PFlags
    {
        /// <summary>
        /// A handy flag for no flags set ;)
        /// </summary>
        None = 0,
        /// <summary>
        /// Carry
        /// </summary>
        C = 1,
        /// <summary>
        /// Zero
        /// </summary>
        Z = 2,
        /// <summary>
        /// Interrupt
        /// </summary>
        I = 4,
        /// <summary>
        /// Decimal Math mode
        /// </summary>
        D = 8,
        /// <summary>
        /// Always Zero
        /// </summary>
        B = 16,
        /// <summary>
        /// Reset
        /// </summary>
        R = 32,
        /// <summary>
        /// oVerflow
        /// </summary>
        V = 64,
        /// <summary>
        /// Negative / Sign
        /// </summary>
        N = 128
    }
}
