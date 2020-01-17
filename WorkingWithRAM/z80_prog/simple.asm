    output "simple.bin", t
    org 0x0000  
    jp main

main:   

    ld  a, 5 ;set accumulator to 5
loop:
    add 1
    in a, (0x10)
    out (0x10), a
    jp loop


    outend     