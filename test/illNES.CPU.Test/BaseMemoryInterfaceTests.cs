using System;
using Xunit;

namespace illNES.CPU.Test
{
    public class BaseMemoryInterfaceTests
    {
        [Fact]
        public void DefaultConstructorInitialises64KRam()
        {
            //Assert that we can read a value from 0xffff without error

            var mem = new BaseMemoryInterface();

            mem.Read(0xffff);

            //No formal assertion required, since the test will throw if there's an issue.
        }

        [Theory]
        [InlineData(0xffff)] //one smaller
        [InlineData(0x10001)] //one larger
        public void ConstructorThrowsIfNot64KRam(int size)
        {
            var ram = new byte[size];

            Assert.Throws<ArgumentException>(
                () => new BaseMemoryInterface(ram));
        }

        [Fact]
        public void ConstructorInitialises64KRam()
        {
            var ram = new byte[0x10000];
            var mem = new BaseMemoryInterface(ram);

            mem.Read(0xffff);

            //No formal assertion required, since the test will throw if there's an issue.
        }

        [Fact]
        public void WriteSetsValueAtAddress()
        {
            var ram = new byte[0x10000];
            var mem = new BaseMemoryInterface(ram);

            const ushort address = 17;
            const byte val = 5;

            mem.Write(address, val);

            Assert.Equal(val, ram[address]);
        }

        [Fact]
        public void ReadGetsByteValueAtAddress()
        {
            var ram = new byte[0x10000];
            var mem = new BaseMemoryInterface(ram);

            const ushort address = 73;
            const byte val = 32;

            ram[address] = val;

            Assert.Equal(val, mem.Read(address));
        }

        [Fact]
        public void ReadWordGetsCorrectWordValueFromConsecutiveAddresses()
        {
            var ram = new byte[0x10000];
            var mem = new BaseMemoryInterface(ram);

            const ushort address = 210;
            const byte msb = 0xff;
            const byte lsb = 0xee;
            const ushort word = 0xffee;

            //this is the correct behaviour for 6502's program counter
            ram[address] = lsb;
            ram[address+1] = msb;

            Assert.Equal(word, mem.ReadWord(address));
        }

        [Fact]
        public void ReadWordAt0xffffWrapsAround()
        {
            var ram = new byte[0x10000];
            var mem = new BaseMemoryInterface(ram);

            const ushort address = 0xffff;
            const byte msb = 0xff;
            const byte lsb = 0xee;
            const ushort word = 0xffee;

            //this is the correct behaviour for 6502's program counter
            ram[address] = lsb;
            ram[(address+1) & 0xffff] = msb; //We manually wrap the "address+1" for the setup write

            Assert.Equal(word, mem.ReadWord(address));
        }
    }
}
