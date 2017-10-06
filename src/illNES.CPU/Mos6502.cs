using System.Resources;
using illNES.CPU.Types;

namespace illNES.CPU
{
    public class Mos6502 : IMos6502
    {
        private readonly IMemoryInterface _m;

        /// <inheritdoc />
        public byte A { get; private set; }

        /// <inheritdoc />
        public byte X { get; private set; }

        /// <inheritdoc />
        public byte Y { get; private set; }

        /// <inheritdoc />
        public byte S { get; private set; }

        /// <inheritdoc />
        public ushort PC { get; private set; } = 0xfffc; // Initialise to the reset vector

        /// <inheritdoc />
        public PFlags P { get; private set; } = PFlags.R | PFlags.I; //the Always 1 flag is set, and we set Interrupt disable at power on

        //Internal state bits
        private bool _reset = true; //start in reset, means we can execute bootstrap as a reset ;)
        private bool _nmi;
        private bool _irq;
        private bool _xPage;
        private int _skipCycles;

        public Mos6502(IMemoryInterface m)
        {
            _m = m;

            //TODO In the C++ ctor we build an instruction set lookup table here...
        }

        /// <inheritdoc />
        public void Tick()
        {
            throw new System.NotImplementedException();
        }

        /// <inheritdoc />
        public string DumpRegisters()
        {
            throw new System.NotImplementedException();
        }
    }
}
