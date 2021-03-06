namespace illNES.CPU {
    
    /// <summary>
    /// The public API for the CPU to interface with RAM, 64k at a time ;)
    /// </summary>
    public interface IMemoryInterface {
        
        /// <summary>
        /// Read a byte value at a 16bit memory address
        /// </summary>
        /// <param name="address">The memory address to read from</param>
        /// <returns>The byte stored at the requested address</returns>
        byte Read(ushort address);

        /// <summary>
        /// Read a 16 bit value from two consecutive memory addresses.
        /// `address` contains LSB, `address+1` contains MSB.
        /// The bytes are put back together little endian, as follows:
        /// 
        /// address (LSB): 0xee
        /// address + 1 (MSB): 0xff
        /// result (MSBLSB): 0xffee
        /// </summary>
        /// <param name="address">The memory address to start reading at</param>
        /// <returns>
        /// The 16 bit value comprised of the byte stored at `address`,
        /// and the byte stored at `address+1`
        /// </returns>
        ushort ReadWord(ushort address);

        /// <summary>
        /// Write a byte value to the specified 16bit memory address.
        /// </summary>
        /// <param name="address">The memory address to write to</param>
        /// <param name="value">The byte value to write</param>
        void Write(ushort address, byte value);

        /// <summary>
        /// Dump up to 256 contiguous values from memory, starting at a given address.
        /// </summary>
        /// <param name="start">The memory address to start from</param>
        /// <param name="offset">The offset relative to `start` to cease dumping at</param>
        /// <returns></returns>
        string DumpRam(ushort start, byte offset);

        /// <summary>
        /// Dump the contents of a binary file into memory, starting at the specified address.
        /// </summary>
        /// <param name="filePath">Path to the file</param>
        /// <param name="address">The memory address to start the dump at</param>
        void FileDump(string filePath, ushort address);
    }
}