using Moq;
using System.Linq;
using Xunit;

namespace illNES.CPU.Test
{
    public class Mos6502TxsTests
    {
        private Mock<IMemoryInterface> _mMock;
        private IMemoryInterface _m;
        private Mos6502 _cpu;

        public Mos6502TxsTests()
        {
            _mMock = new Mock<IMemoryInterface>();
            _mMock.Setup(mem => mem.Read(0xfffc)).Returns(0x0);
            _mMock.Setup(mem => mem.ReadWord(0xfffc)).Returns(0x8000); //PC points to ROM start at 0x8000
            _mMock.Setup(mem => mem.Read(0x8000)).Returns(0x9a); //ROM start returns TSX
            _m = _mMock.Object;

            _cpu = new Mos6502(_m);

            //get the CPU past the opening BRK reset.
            Enumerable.Range(0, 7).ToList()
                .ForEach(x => _cpu.Tick()); //Tick the expected number of times
        }

        [Fact]
        public void _0x9aLeavesNonTargetRegistersAlone()
        {
            _cpu.Tick();

            Assert.Equal(0, _cpu.A);
            Assert.Equal(0, _cpu.X);
            Assert.Equal(0, _cpu.Y);
        }

        [Fact]
        public void _0x9aAdvancesProgramCounterBy1()
        {
            _cpu.Tick();

            Assert.Equal(0x8001, _cpu.PC); //Program Counter has advanced by 1 only
        }

        [Fact]
        public void _0x9aTransfersXtoS()
        {
            _cpu.Tick();

            Assert.Equal(_cpu.X, _cpu.S);
        }

        [Fact]
        public void _0x9aTakes2Cycles()
        {
            Enumerable.Range(0, 2).ToList()
                .ForEach(x => _cpu.Tick()); //Tick the expected number of times

            Assert.Equal(0x8001, _cpu.PC); //Program Counter has advanced by this op's length only

            _cpu.Tick(); //This should advance PC by the length of whatever the next op is
            Assert.NotEqual(0x8001, _cpu.PC);
        }
    }
}
