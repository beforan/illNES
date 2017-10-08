using illNES.CPU.Types;
using Moq;
using System.Collections.Generic;
using Xunit;

namespace illNES.CPU.Test
{
    public class Mos6502ConstructorTests
    {
        [Fact]
        public void InitialValues()
        {
            var m = new Mock<IMemoryInterface>();

            var cpu = new Mos6502(m.Object);

            //Main registers should be 0
            Assert.All(
                new List<byte> { cpu.A, cpu.X, cpu.Y },
                item => Assert.Equal(0, item));

            Assert.Equal(0xff, cpu.S);

            Assert.Equal(0xfffc, cpu.PC);

            Assert.Equal(PFlags.R | PFlags.I, cpu.P);
        }
    }
}
