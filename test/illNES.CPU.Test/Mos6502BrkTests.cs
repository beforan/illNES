using illNES.CPU.Types;
using Moq;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace illNES.CPU.Test
{
    public class Mos6502BrkTests
    {
        private Mock<IMemoryInterface> _mMock;
        private IMemoryInterface _m;
        private Mos6502 _cpu;

        public Mos6502BrkTests()
        {
            _mMock = new Mock<IMemoryInterface>();
            _mMock.Setup(mem => mem.Read(0xfffc)).Returns(0x0);
            _mMock.Setup(mem => mem.ReadWord(0xfffc)).Returns(0x8000); //Reset Vector points to ROM start at 0x8000
            _mMock.Setup(mem => mem.Read(0x8000)).Returns(0x0); //ROM start returns BRK
            _mMock.Setup(mem => mem.ReadWord(0xfffe)).Returns(0x1234); //BRK Vector points to arbitrary value ;)
            _m = _mMock.Object;

            _cpu = new Mos6502(_m);

            //get the CPU past the opening BRK reset.
            Enumerable.Range(0, 7).ToList()
                .ForEach(x => _cpu.Tick()); //Tick the expected number of times
        }

        [Fact]
        public void _0x0LeavesRegistersAlone()
        {
            _cpu.Tick();

            //Registers should still be at initial values
            Assert.All(
                new List<byte> { _cpu.A, _cpu.X, _cpu.Y },
                item => Assert.Equal(0, item));
        }

        [Fact]
        public void _0x0DecrementsS()
        {
            var s = _cpu.S;

            _cpu.Tick();

            //Registers should still be at initial values
            Assert.Equal(s - 3, _cpu.S);
        }

        [Fact]
        public void _0x0SetsTheInterruptFlag()
        {
            //TODO we need the I flag to be off first to test properly...
            _cpu.Tick();

            //Registers should still be at initial values
            Assert.True(_cpu.P.HasFlag(PFlags.I));
        }


        [Fact]
        public void _0x0SetsProgramCounterToTheValueAtBrkVector()
        {
            _cpu.Tick();

            Assert.Equal(0x1234, _cpu.PC); //Program Counter is set based on BRK Vector
        }

        //Don't do this one since we can't prove it using PC
        //as we set the value of it explicitly
        //Also in practice if it didn't take 7 cycles,
        //our test setup would be inaccurate
        //and all Instructions tests would fail.
        //[Fact]
        //public void _0x0Takes7Cycles()
    }
}
