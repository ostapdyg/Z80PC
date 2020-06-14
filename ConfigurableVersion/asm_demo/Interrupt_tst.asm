    output "Interrupt_tst.bin", t 
    nop
    jp main
main:   
    im 1
    ei
    ld sp, 0xabcd
    ld a, "b"

loop:
    out (0x01), a 
    jp loop

    out (0x01), a

    BLOCK 0x32, 0


ihdlr_serial:
    in a, (0x01)
    ei
    reti

    

outend     


