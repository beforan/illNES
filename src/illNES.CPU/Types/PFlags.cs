using System;

namespace illNES.CPU.Types
{
    [Flags]
    public enum PFlags
    {
        /// <summary>
        /// A handy flag for no flags set ;)
        /// </summary>
        None = 0,

        /// <summary>
        /// Carry.
        /// This holds the carry out of the most significant
        /// bit in any arithmetic operation. In subtraction operations however, this
        /// flag is cleared - set to 0 - if a borrow is required, set to 1 - if no
        /// borrow is required. The carry flag is also used in shift and rotate
        /// logical operations.
        /// </summary>
        C = 1,

        /// <summary>
        /// Zero.
        /// This is set to 1 when any arithmetic or logical
        /// operation produces a zero result, and is set to 0 if the result is
        /// non-zero.
        /// </summary>
        Z = 2,

        /// <summary>
        /// Interrupt Disable.
        /// This is an interrupt enable/disable flag. If it is set,
        /// interrupts are disabled. If it is cleared, interrupts are enabled.
        /// </summary>
        I = 4,

        /// <summary>
        /// Decimal Mode Status.
        /// This is the decimal mode status flag. When set, and an Add with
        /// Carry or Subtract with Carry instruction is executed, the source values are
        /// treated as valid BCD (Binary Coded Decimal, eg. 0x00-0x99 = 0-99) numbers.
        /// The result generated is also a BCD number.
        /// </summary>
        D = 8,

        /// <summary>
        /// Break.
        /// This is set when a software interrupt (BRK instruction) is executed.
        /// </summary>
        B = 16,

        /// <summary>
        /// Always Set.
        /// Not used. Supposed to be logical 1 at all times.
        /// </summary>
        R = 32,

        /// <summary>
        /// oVerflow.
        /// When an arithmetic operation produces a result
        /// too large to be represented in a byte, V is set.
        /// </summary>
        V = 64,

        /// <summary>
        /// Negative / Sign.
        /// This is set if the result of an operation is
        /// negative, cleared if positive.
        /// </summary>
        N = 128
    }
}
