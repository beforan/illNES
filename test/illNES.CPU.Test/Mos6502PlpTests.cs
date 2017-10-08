using Moq;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace illNES.CPU.Test
{
    public class Mos6502PlpTests
    {
        private Mock<IMemoryInterface> _mMock;
        private IMemoryInterface _m;
        private Mos6502 _cpu;

        public Mos6502PlpTests()
        {
            _mMock = new Mock<IMemoryInterface>();
            _mMock.Setup(mem => mem.Read(0xfffc)).Returns(0x0); 
            _mMock.Setup(mem => mem.ReadWord(0xfffc)).Returns(0x8000); //PC points to ROM start at 0x8000
            _mMock.Setup(mem => mem.Read(0x8000)).Returns(0x28); //ROM start returns PLP

            _m = _mMock.Object;

            _cpu = new Mos6502(_m);

            //get the CPU past the opening BRK reset.
            Enumerable.Range(0, 7).ToList()
                .ForEach(x => _cpu.Tick()); //Tick the expected number of times

            _cpu.S = 0xfe; //lineup the stack pointer to make life easier ;)
            _mMock.Setup(mem => mem.Read(0x01ff)).Returns(7); //top of the stack returns a value we can check for
        }

        [Fact]
        public void _0x28LeavesRegistersAlone()
        {
            _cpu.Tick();

            //Registers should still be at initial values
            Assert.All(
                new List<byte> { _cpu.A, _cpu.X, _cpu.Y },
                item => Assert.Equal(0, item));
        }

        [Fact]
        public void _0x28IncrementsS()
        {
            var s = _cpu.S;

            _cpu.Tick();

            Assert.Equal(s + 1, _cpu.S);
        }

        [Fact]
        public void _0x28PullsPFromStackMemory()
        {
            _cpu.Tick();

            Assert.Equal(7, (byte)_cpu.P);
        }

        [Fact]
        public void _0x28AdvancesProgramCounterBy1()
        {
            _cpu.Tick();

            Assert.Equal(0x8001, _cpu.PC); //Program Counter has advanced by 1 only
        }

        [Fact]
        public void _0x28Takes3Cycles()
        {
            Enumerable.Range(0, 4).ToList()
                .ForEach(x => _cpu.Tick()); //Tick the expected number of times

            Assert.Equal(0x8001, _cpu.PC); //Program Counter has advanced by this op's length only

            _cpu.Tick(); //This should advance PC by the length of whatever the next op is
            Assert.NotEqual(0x8001, _cpu.PC);
        }
    }
}
