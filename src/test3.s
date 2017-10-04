LDX #$08
Decrement:
DEX
STX $0200
CPX #$03
BNE Decrement
STX $0201
BRK