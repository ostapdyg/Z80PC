    output "io_tst.bin", t 
    nop
    jp main


main:
    ld a, 0x05
    out (0xab), a
    jp main