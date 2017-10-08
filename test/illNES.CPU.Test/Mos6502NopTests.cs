using illNES.CPU.Types;
using Moq;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace illNES.CPU.Test
{
    public class Mos6502NopTests
    {
        [Fact]
        public void _0xeaLeavesRegistersAlone()
        {
            var m = new Mock<IMemoryInterface>();
            m.Setup(mem => mem.Read(0xfffc)).Returns(0xea); //Program Counter points to a NOP

            var cpu = new Mos6502(m.Object);

            cpu.Tick(); //This should get our NOP, and execute it

            //Registers should still be at initial values
            Assert.All(
                new List<byte> { cpu.A, cpu.X, cpu.Y, cpu.S },
                item => Assert.Equal(0, item));

            Assert.Equal(PFlags.R | PFlags.I, cpu.P);
        }

        [Fact]
        public void _0xeaAdvancesProgramCounterBy1()
        {
            var m = new Mock<IMemoryInterface>();
            m.Setup(mem => mem.Read(0xfffc)).Returns(0xea); //Program Counter points to a NOP

            var cpu = new Mos6502(m.Object);

            cpu.Tick(); //This should get our NOP, and execute it

            Assert.Equal(0xfffd, cpu.PC); //Program Counter has advanced by 1 only
        }

        [Fact]
        public void _0xeaTakes2Cycles()
        {
            var m = new Mock<IMemoryInterface>();
            m.Setup(mem => mem.Read(0xfffc)).Returns(0xea); //Program Counter points to a NOP

            var cpu = new Mos6502(m.Object);

            Enumerable.Range(0, 2).ToList()
                .ForEach(x => cpu.Tick()); //Tick the expected number of times

            Assert.Equal(0xfffd, cpu.PC); //Program Counter has advanced by this op's length only

            cpu.Tick(); //This should advance PC by the length of whatever the next op is
            Assert.NotEqual(0xfffd, cpu.PC);
        }
    }
}
