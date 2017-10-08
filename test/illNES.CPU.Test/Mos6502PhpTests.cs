using Moq;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace illNES.CPU.Test
{
    public class Mos6502PhpTests
    {
        private Mock<IMemoryInterface> _mMock;
        private IMemoryInterface _m;
        private Mos6502 _cpu;

        private int _pushedValue;
        private int _pushedAddress;

        public Mos6502PhpTests()
        {
            _mMock = new Mock<IMemoryInterface>();
            _mMock.Setup(mem => mem.Read(0xfffc)).Returns(0x0); 
            _mMock.Setup(mem => mem.ReadWord(0xfffc)).Returns(0x8000); //PC points to ROM start at 0x8000
            _mMock.Setup(mem => mem.Read(0x8000)).Returns(0x08); //ROM start returns PHA

            //store values for the push so we can check
            _mMock.Setup(mem => mem.Write(It.IsAny<ushort>(), It.IsAny<byte>()))
                .Callback<ushort, byte>((a, v) =>
                {
                    _pushedValue = v;
                    _pushedAddress = a;
                });

            _m = _mMock.Object;

            _cpu = new Mos6502(_m);

            //get the CPU past the opening BRK reset.
            Enumerable.Range(0, 7).ToList()
                .ForEach(x => _cpu.Tick()); //Tick the expected number of times
        }

        [Fact]
        public void _0x08LeavesRegistersAlone()
        {
            var p = _cpu.P;
            _cpu.Tick();

            //Registers should still be at initial values
            Assert.All(
                new List<byte> { _cpu.A, _cpu.X, _cpu.Y },
                item => Assert.Equal(0, item));

            Assert.Equal(p, _cpu.P);
        }

        [Fact]
        public void _0x08DecrementsS()
        {
            var s = _cpu.S;

            _cpu.Tick();

            Assert.Equal(s - 1, _cpu.S);
        }

        [Fact]
        public void _0x08PushesP()
        {
            _cpu.Tick();

            Assert.Equal((byte)_cpu.P, _pushedValue);
        }

        [Fact]
        public void _0x08PushesToStackMemory()
        {
            var s = _cpu.S;

            _cpu.Tick();

            Assert.Equal(0x1 << 8 | s, _pushedAddress);
        }

        [Fact]
        public void _0x08AdvancesProgramCounterBy1()
        {
            _cpu.Tick();

            Assert.Equal(0x8001, _cpu.PC); //Program Counter has advanced by 1 only
        }

        [Fact]
        public void _0x08Takes3Cycles()
        {
            Enumerable.Range(0, 3).ToList()
                .ForEach(x => _cpu.Tick()); //Tick the expected number of times

            Assert.Equal(0x8001, _cpu.PC); //Program Counter has advanced by this op's length only

            _cpu.Tick(); //This should advance PC by the length of whatever the next op is
            Assert.NotEqual(0x8001, _cpu.PC);
        }
    }
}
