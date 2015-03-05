Reset: LDX #$FF ; Load X with 255
       TXS      ; Transfer X to S (resets the Stack Pointer)
                ; Could initialise I/O here if needed
                ; (will do later for input)
Init:  CLI      ; Enable Interrupts
       CLD      ; or SED, to choose maths mode
Main:  LDA #$01
       STA $0200
       LDA #$05
       STA $0201
       LDA #$08
       STA $0202
       BRK
       NOP
       LDA #$01
       STA $0200
       LDA #$05
       STA $0201
       LDA #$08
       STA $0202
       BRK