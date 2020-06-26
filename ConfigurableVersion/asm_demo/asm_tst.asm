    output "asm_tst.bin", t 
    nop
    di
    jp main


main:
    nop
    nop 
    ld a, 1;
    jp main


    nop 
    jr nc, 0x3f
    jr nc, 0x0f
;kill_str .byte "!!!"