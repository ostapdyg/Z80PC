    output "simple.bin", t
    org 0x0000  
    jp main





main:   

loop:
    ld a, "a"
    ld bc, 0x0101
    out (c), a
    ld a, "c"
    in a, (c)
    ld bc, 0x1000
    out (c), a

    jp loop


    outend     
