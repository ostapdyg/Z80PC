    output "ram_diagnostics.bin", t
    org 0x0000  
    jp main


main:   
    ld bc, 0xf00f
    out (c), a

lp:
    nop
    ld bc, 0x0000
loop:
    ld a, (bc)
    inc bc
    ld hl, 0xffff
    sbc hl, bc
    jp nz, loop

    ld bc, 0x000f
    out (c), a
    jp lp



outend