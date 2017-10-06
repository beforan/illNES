using System;

namespace illNES.CPU
{
    public class BaseMemoryInterface : IMemoryInterface
    {
        private readonly byte[] _ram;

        /// <summary>
        /// Initialise the interface with 64k address range, as per the MOS6502.
        /// This represents what the CPU is capable of accessing (64k) while AccessMemory() can choose to only use
        /// for example, the range 0x0 - 0x7ff, which would provide 2k of RAM like in the NES.
        /// </summary>
        public BaseMemoryInterface()
        {
            _ram = new byte[0x10000];
        }

        /// <summary>
        /// Accept external ram.
        /// This is really only for unit testing purposes,
        /// as it still requires the 64k address range of the MOS6502
        /// to avoid the rest of the implementation breaking.
        /// </summary>
        /// <param name="ram"></param>
        public BaseMemoryInterface(byte[] ram)
        {
            if(ram.Length != 0x10000)
                throw new ArgumentException("Expected 64k of RAM");

            _ram = ram;
        }

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
            if (value.HasValue) _ram[address] = value.Value;
            return _ram[address];
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