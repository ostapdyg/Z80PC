    output "simple.bin", t
    org 0x0000  
    jp main

main:   

    ld  a, 5 ;set accumulator to 5
    add 10
    out (3), a
    jp main


    outend     