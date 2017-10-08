using System;

namespace illNES.CPU.Types
{
    /// <summary>
    /// Not only a useful enum of the 6502's addressing modes,
    /// the values are flags for operations used in the addressing,
    /// So the values describe how to address in that mode!
    /// </summary>
    [Flags]
    internal enum AddressModes
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
        OffsetY = 128,

        Absolute = 
            SetParamAddress |
            ReadWord,
        AbsoluteX =
            SetParamAddress |
            ReadWord |
            CheckWrap |
            Offset,
        AbsoluteY =
            SetParamAddress |
            ReadWord |
            CheckWrap |
            Offset |
            OffsetY,

        Immediate = SetParamAddress,

        Implied = None,
        /// <summary>
        /// This abuses the OffsetY Flag to differentiate it from Implied Mode
        /// </summary>
        Accumulator = OffsetY,

        //IndirectY: 10011111 : 0x9F
        Indirect = 
            SetParamAddress |
            ReadByte |
            ReadWord,
        IndirectX = 
            SetParamAddress |
            ReadByte |
            Offset |
            ByteWrapAddress |
            ReadWordLate,
        IndirectY =
            SetParamAddress |
            ReadByte |
            ReadWord |
            CheckWrap |
            Offset |
            OffsetY,

        ZeroPage = 
            SetParamAddress |
            ReadByte,
        ZeroPageX = 
            SetParamAddress |
            ReadByte |
            Offset |
            ByteWrapAddress,
        ZeroPageY =
            SetParamAddress |
            ReadByte |
            Offset |
            ByteWrapAddress |
            OffsetY,
        /// <summary>
        /// This abuses the OffsetY Flag to differentiate it from ZeroPage Mode
        /// </summary>
        Relative =
            SetParamAddress |
            ReadByte |
            OffsetY
    }
}
