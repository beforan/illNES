Reset:
LDX #$FF    ;LDX Imm
TXS -
CLI -
CLD -
Main:
LDA #$01    ;LDA Imm A=0x01
STA $01     ;STA ZPg 0x0001=0x01
LDY #$01    ;LDY Imm Y=0x01
STX $00,Y   ;STX ZPY 0x0001=0xFF
LDA $01     ;LDA ZPg A=0xFF
LDA #$01    ;LDA Imm A=0x01
STA $0001,Y ;STA AbY 0x0002=0x01
LDX $01,Y   ;LDX ZPY X=0x01
LDA #$05    ;LDA Imm A=0x05
STA $02,X   ;STA ZPX 0x0003=0x05
LDA $01,X   ;LDA ZPX A=0x01
STA $0200,X ;STA AbX 0x0201=0x01
LDA $0002,Y ;LDA AbY A=0x05
LDY #$FF    ;LDY Imm Y=0xFF
STA $0101,Y ;STA AbY 0x0200=0x05 (page boundary, STA doesn't care)
LDA $0001,X ;LDA AbX A=0x01
LDX #$FE    ;LDX Imm X=0xFE
STA $0103,X ;STA AbX 0x0201=0x01 (page boundary, STA doesn't care)
LDX $01     ;LDX ZPg X=0xFF
LDA $0101,X ;LDA AbX A=0x05 (page boundary)
STA $0203   ;STA Abs 0x0203=0x05
LDA $0102,Y ;LDA AbY A=0x01 (page boundary)
Indirects:
LDA #$02    ;LDA Imm A=0x02
STA $04     ;LDA ZPg 0x0004=0x02
STA ($04,X) ;STA InX 0x0205=0x02
STA $01     ;STA ZPg 0x0001=0x02
LDA ($01),Y ;LDA InY A=0x01 (page boundary)
LDX $0201   ;LDX Abs X=0x01
LDA ($02,X) ;LDA InX A=0x02
STA ($03),Y ;STA InY 0x0304=0x02 (page boundary, STA doesn't care)
LDA #$07    ;LDA Imm A=0x07
LDY #$01    ;LDY Imm Y=0x01
STA ($03),Y ;STA InY 0x0206=0x07
LDA #$02    ;LDA Imm A=0x02
STA $03     ;STA ZPg 0x0003=0x02
LDY #$01    ;LDY Imm Y=0x01
LDA ($03),Y ;LDA InY A=0x05
