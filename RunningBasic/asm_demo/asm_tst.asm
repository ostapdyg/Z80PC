    output "asm_tst.bin", t 
    nop
    di
    jp main


main:
    nop
    nop 
    ld a, 1;
    jp main