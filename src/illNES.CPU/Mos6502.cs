using illNES.CPU.Types;

namespace illNES.CPU
{
    public partial class Mos6502 : IMos6502
    {
        private readonly IMemoryInterface _m;
        private static readonly InstructionSet Operations = new InstructionSet();

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
        }

        /// <inheritdoc />
        public void Tick()
        {
            //Only actually tick if we are not skipping cycles
            if (_skipCycles == 0)
            {
                var op = Operations[_m.Read(PC)]; //set the op code value from the program counter's memory address

                // check for interrupts and implement them as a BRK op
                if (_reset || _nmi || _irq && (P & PFlags.I) == 0) //IRQ requires the interrupt flag to be off, the others don't
                    op = Operations[0];

                // Handle addressing mode
                var address = GetAddress(op.Mode);

                PC += op.Length; //increment PC by the expected amount, as GetAddress() has read the op params

                _skipCycles = Exec(op, address);
            }

            //Decrement after the tick (if any) since Exec() will have set the appropriate skip value
            _skipCycles--;

            //At the end of any cycle in which reset was true, set it to false,
            //as we will have performed the reset by now.
            _reset = false;
        }

        /// <summary>
        /// Execute an instruction
        /// </summary>
        /// <param name="op">The Operation containing the instruction details</param>
        /// <param name="address">The address</param>
        /// <returns>The number of cycles the Operation "took"</returns>
        private int Exec(Operation op, ushort address)
        {
            //TODO implement more ops

            //For now we ignore op, and just NOP!
            return Operations[0xea].Cycles;
        }

        /// <summary>
        /// Gets the actual memory address for an Operation, based on its addressing mode.
        /// </summary>
        /// <param name="mode">The addressing mode</param>
        /// <returns>The address</returns>
        private ushort GetAddress(AddressModes mode)
        {
            //TODO implement all addressing modes
            ushort address = 0;

            //set the offset value to X or Y reg ahead of time, even if we don't need it
            byte offset = mode.HasFlag(AddressModes.OffsetY) ? Y : X;

            //conditionally execute addressing stages in order
            if (mode.HasFlag(AddressModes.SetParamAddress))
                address = (ushort)((PC + 1) & ushort.MaxValue);

            if (mode.HasFlag(AddressModes.ReadByte))
                address = _m.Read(address);

            if (mode.HasFlag(AddressModes.ReadWord))
                address = _m.ReadWord(address);

            if (mode.HasFlag(AddressModes.CheckWrap))
                if ((address + offset) > (address | byte.MaxValue))
                    _xPage = true;

            if (mode.HasFlag(AddressModes.Offset))
                address = (ushort)((address + offset) & ushort.MaxValue);

            if (mode.HasFlag(AddressModes.ByteWrapAddress))
                address = (ushort)(address & byte.MaxValue);

            if (mode.HasFlag(AddressModes.ReadWordLate))
                address = _m.ReadWord(address);

            return (ushort)(address & ushort.MaxValue); //safety wrap
        }

        /// <inheritdoc />
        public string DumpRegisters()
        {
            throw new System.NotImplementedException();
        }
    }
}
