============================
Instruction set Lookup Table
============================

===== ======
I-Len T-Cnt
----- ------
Mnemonic
------------
Address Mode
============

I-Len
    Instruction length. The number of bytes of memory required to store the instruction.
T-Cnt
    Timing Cycle (T-State) count. The number of clock cycles required to execute the instruction.
Mnemonic
    The abbreviated "human readable" identity of the instruction.
Address Mode
    The :doc:`memory addressing mode <addr_mode>` used by the instruction.

The opcode for any instruction may be composed by reading the row and column it is in:

- ``ASL`` in **Absolute** addressing mode is in row ``1-``, column ``-E``
- Therefore it's opcode (hexadecimal representation) is ``1E`` (or ``$1E`` or ``0x1E``)

Some instructions (or more specifically :doc:`Addressing Modes <addr_mode>`) require either one or two bytes of data as a parameter.

Under these circumstances, this data immediately follows the instruction itself.

============= ===============
Mnemonic Code Assembled Code
============= ===============
``PLA``       ``$68``
``BEQ $03``   ``$F0 $9F``
``JMP $A5B6`` ``$4C $B6 $A5``
============= ===============

Key to Addressing modes:
------------------------

============ =============== ============= =============
**Implied**  **Accumulator** **Immediate** **Zero Page**
\-           A               #             0
**Absolute** **Indirect**    **X-indexed** **Y-indexed**
/            >               X             Y
============ =============== ============= =============

.. include:: by_tbl-table.rst

.. note::
    - Blank opcodes, e.g., ``F2``, and all opcodes whose low nibbles are ``3``, ``7``, ``B`` and ``F`` are undefined in the 6502 instruction set. 