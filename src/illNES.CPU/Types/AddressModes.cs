namespace illNES.CPU.Types
{
    /// <summary>
    /// Not only a useful enum of the 6502's addressing modes,
    /// the values are flags for operations used in the addressing,
    /// So the values describe how to address in that mode!
    /// </summary>
    internal enum AddressModes
    {
        Absolute = 
            AddressFlags.SetParamAddress |
            AddressFlags.ReadWord,
        AbsoluteX =
            AddressFlags.SetParamAddress |
            AddressFlags.ReadWord |
            AddressFlags.CheckWrap |
            AddressFlags.Offset,
        AbsoluteY =
            AddressFlags.SetParamAddress |
            AddressFlags.ReadWord |
            AddressFlags.CheckWrap |
            AddressFlags.Offset |
            AddressFlags.OffsetY,

        Immediate = AddressFlags.SetParamAddress,

        Implied = AddressFlags.None,
        /// <summary>
        /// This abuses the OffsetY Flag to differentiate it from Implied Mode
        /// </summary>
        Accumulator = AddressFlags.OffsetY,

        //IndirectY: 10011111 : 0x9F
        Indirect = 
            AddressFlags.SetParamAddress |
            AddressFlags.ReadByte |
            AddressFlags.ReadWord,
        IndirectX = 
            AddressFlags.SetParamAddress |
            AddressFlags.ReadByte |
            AddressFlags.Offset |
            AddressFlags.ByteWrapAddress |
            AddressFlags.ReadWordLate,
        IndirectY =
            AddressFlags.SetParamAddress |
            AddressFlags.ReadByte |
            AddressFlags.ReadWord |
            AddressFlags.CheckWrap |
            AddressFlags.Offset |
            AddressFlags.OffsetY,

        ZeroPage = 
            AddressFlags.SetParamAddress |
            AddressFlags.ReadByte,
        ZeroPageX = 
            AddressFlags.SetParamAddress |
            AddressFlags.ReadByte |
            AddressFlags.Offset |
            AddressFlags.ByteWrapAddress,
        ZeroPageY =
            AddressFlags.SetParamAddress |
            AddressFlags.ReadByte |
            AddressFlags.Offset |
            AddressFlags.ByteWrapAddress |
            AddressFlags.OffsetY,
        /// <summary>
        /// This abuses the OffsetY Flag to differentiate it from ZeroPage Mode
        /// </summary>
        Relative =
            AddressFlags.SetParamAddress |
            AddressFlags.ReadByte |
            AddressFlags.OffsetY
    }
}
