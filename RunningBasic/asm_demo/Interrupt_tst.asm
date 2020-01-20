    output "Interrupt_tst.bin", t 
    nop
    jp main
main:   

    im 1
    ei
    ld sp, 0xabcd

loop:
        nop 
        nop
        nop
      jp loop

    BLOCK 40, 0


ihdlr_serial:
    ld a, "c"
    in a, (0x01)
    inc a
    out (0x01), a
    ei
    reti

outend     


