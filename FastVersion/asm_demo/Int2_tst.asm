    output "Int2_tst.bin", t 
    nop
    jp main
INT_TABLE: 
    .WORD ihdlr_serial
    .WORD ihdlr_other

main:
    ld a, 0
    ld I, a 
    ld c, 0x01
    im 2
    ei
    ld sp, 0xabcd
    ld a, "a"

loop:
    out (0x01), a 
    jp loop

    BLOCK 0x22, 0

ihdlr_other:

    in b, (c);
    ei
    reti


ihdlr_serial:
    in a, (0x01)
    ei
    reti

outend     
