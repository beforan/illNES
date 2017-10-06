using System;

namespace illNES.CPU.Types
{
    /// <summary>
    /// Flags for operations to take for addressing modes
    /// </summary>
    [Flags]
    internal enum AddressFlags
    {
        /// <summary>
        /// A handy flag for no flags set ;)
        /// </summary>
        None = 0,

        /// <summary>
        /// Set the target address to PC+1
        /// </summary>
        SetParamAddress = 1,

        /// <summary>
        /// Set address to the byte value at the current address
        /// </summary>
        ReadByte = 2,

        /// <summary>
        /// Set address to the word value at the current address
        /// </summary>
        ReadWord = 4,

        /// <summary>
        /// Check for a Page wrap, and record if we need to skip a cycle
        /// </summary>
        CheckWrap = 8,

        /// <summary>
        /// Offset the address by the X or Y register,
        /// depending if the OffsetY Flag is set
        /// </summary>
        Offset = 16,

        /// <summary>
        /// Wrap the address inside an 8-bit range
        /// </summary>
        ByteWrapAddress = 32,

        /// <summary>
        /// Set address to the word value at the current address
        /// (later in the order)
        /// </summary>
        ReadWordLate = 64,

        /// <summary>
        /// Set the Offset Register to Y instead of X
        /// </summary>
        OffsetY = 128
    }
}
