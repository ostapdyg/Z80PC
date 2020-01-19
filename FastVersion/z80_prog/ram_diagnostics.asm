    output "ram_diagnostics.bin", t
    org 0x0000  
    jp main


main:   
    ld bc, 0xf000
    out (c), a


    ld bc, 0x0000
loop:
    ld a, (bc)
    inc bc
    ld hl, 0x02ff
    sbc hl, bc
    jp nz, loop

    ld bc, 0xf000
    out (c), a
    ld bc, 0xa000
    out(c), a

outend    