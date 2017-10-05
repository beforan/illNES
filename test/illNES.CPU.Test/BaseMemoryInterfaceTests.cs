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
            var mem = new BaseMemoryInterface();

            mem.Read(0xffff);

            //No formal assertion required, since the test will throw if there's an issue.
        }

        //[Fact]
        //public void ReadReturnsWhatWasWritten()
        //{
        //    var mem = new BaseMemoryInterface();

        //    const byte val = 5;
        //    const ushort address = 0;

        //    mem.Write(address, val);

        //    Assert.Equal(val, mem.Read(address));
        //}

        //[Fact]
        //public void ReadWordReturnsWhatWasWrittenCorrectly()
        //{
        //    var mem = new BaseMemoryInterface();

        //    const byte msb = 0xff;
        //    const byte lsb = 0xee;

        //    const ushort word = 0xffee;

        //    const ushort address = 0;

        //    //Write both bytes to consecutive addresses
        //    mem.Write(address, lsb);
        //    mem.Write(address+1, msb);

        //    Assert.Equal(word, mem.ReadWord(address));
        //}
    }
}
