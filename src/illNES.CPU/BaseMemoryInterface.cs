namespace illNES.CPU
{
    public class BaseMemoryInterface : IMemoryInterface
    {
        /// <summary>
        /// Always provide 64k of RAM, allow AccessMemory() to handle mapping and what is actually used.
        /// This represents what the CPU is capable of accessing (64k) while AccessMemory() can choose to only use
        /// for example, the range 0x0 - 0x7ff, which would provide 2k of RAM like in the NES.
        /// </summary>
        protected byte[] ram = new byte[0xffff];

        /// <summary>
        /// Accesses memory at a given address, and writes a value if provided.
        /// Overriding this method allows for changing the way in which RAM is accessed.
        /// </summary>
        /// <param name="address">The address to access</param>
        /// <param name="value">An optional value to write</param>
        /// <returns>The value at the memory address when the method returns</returns>
        protected virtual byte AccessMemory(ushort address, byte? value = null)
        {
            //This provides direct access to the full 64k of RAM
            if (value.HasValue) ram[address] = value.Value;
            return ram[address];
        }

        public string DumpRam(ushort start, byte offset)
        {
            throw new System.NotImplementedException();
        }

        public void FileDump(string filePath, ushort address)
        {
            throw new System.NotImplementedException();
        }

        public byte Read(ushort address)
            => AccessMemory(address);

        public ushort ReadWord(ushort address)
            => (ushort)(AccessMemory((ushort)(address + 1)) << 8 | AccessMemory(address));

        public void Write(ushort address, byte value)
            => AccessMemory(address, value);
    }
}