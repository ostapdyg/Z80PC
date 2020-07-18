    org 0
    ld sp, 0xfeff
    jp main

print:
    push bc
    ld bc, 0x0800
    out (c), e  
    pop bc
    ret

main:
    ld bc, 0xbcde
    ld e, 0xab
    call print
    ld e, b
    call print
    ; ld bc, 
    call chngpgtbl
    call pagetable_test
    ;----
    jp fill_ram_end

chngpgtbl:
    ; ld bc, 0xff03
    ld a, 0x05
    ld (0xff03), a
    ret

    

pagetable_test:
    push hl
    push bc
    push af
    push de
    ld HL, fill_ram
    ld bc, fill_ram_end
pgtst_loop:
    ld e, (HL)
    call print
    inc H
    ld a, b
    cp h
    jp nz, pgtst_loop
    pop de
    pop af
    pop bc
    pop hl
    ret

;---------------------
;----RAM fill loop----
;---------------------
    org 0x0200 
fill_ram:
    dc 0x100, 0x02
    dc 0x100, 0x03
    dc 0x100, 0x04
    dc 0x100, 0x05
    dc 0x100, 0x06
    dc 0x100, 0x07
    dc 0x100, 0x08
    dc 0x100, 0x09
    dc 0x100, 0x0a
    dc 0x100, 0x0b
    dc 0x100, 0x0c
    dc 0x100, 0x0d
    dc 0x100, 0x0e
    dc 0x100, 0x0f
    dc 0x100, 0x10
    dc 0x100, 0x11
    dc 0x100, 0x12
fill_ram_end:
    org 0x1300
