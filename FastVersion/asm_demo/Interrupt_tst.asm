    output "Interrupt_tst.bin", t 
    nop
    jp main
main:   

    ld a, "a"
    out (0x01), a
    in a, (0x01)
    out (0x01), a
    im 1
    ei
    ld sp, 0xabcd
    ld a, "a"

loop:
        //out (0x01), a 
      jp loop

    BLOCK 50, 0


ihdlr_serial:
    in a, (0x01)
    out (0x01), a
    ei
    reti

outend     


