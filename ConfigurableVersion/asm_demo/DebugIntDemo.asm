    output "DebugIntDemo.bin", t 
    nop
    jp main
INT_TABLE: 
    .WORD ihdlr_serial
    .WORD ihdlr_other

main:
    ld a, 0
    ld I, a 
    ld c, 0x01;
    im 2
    ei
    ld sp, 0xabcd;
    ld a, 1
    ld bc, 0xbc

outer_loop:
    ccf
    call ihdlr_debug
    ld hl, 0
loop:
    ; out (0x01), a 
    inc hl
    cp h
    jp nz, loop

    inc a
    jp outer_loop


    BLOCK 0x22, 0

ihdlr_other:

    in b, (c);
    ei
    reti


ihdlr_serial:
    in a, (0x01)
    out (0x02), a
    ei
    reti

;11001100

ihdlr_debug:
    ld (old_sp), sp
    push af
    push hl
    ; push ix
    ; ld ix

    out (0x30), a   ;A

    push af
    pop hl

    ld (reg_buffer), hl     ;To output AF
    ld a, (reg_buffer)      ;f
    out (0x31), a


    ld a, b
    out (0x32), a

    ld a, c
    out (0x33), a

    ld a, d
    out (0x34), a

    ld a, e
    out (0x35), a

    ld (reg_buffer), hl     ;To output HL
    ld a, (reg_buffer+1)    ;Hlh
    out (0x36), a
    ld a, (reg_buffer)      ;HLl
    out (0x37), a

    ld (reg_buffer), ix     ;To output IX
    ld a, (reg_buffer+1)    ;IXh
    out (0x38), a
    ld a, (reg_buffer)      ;IXl
    out (0x39), a

    ld (reg_buffer), IY     ;To output IY
    ld a, (reg_buffer+1)    ;IYh
    out (0x3a), a
    ld a, (reg_buffer)      ;IYl
    out (0x3b), a

    ld (reg_buffer), SP     ;To output SP
    ld a, (reg_buffer+1)    ;SPh
    out (0x3c), a
    ld a, (reg_buffer)      ;SPl
    out (0x3d), a

    ld hl, (old_sp)

    ; ld (reg_buffer), (hl)     ;8-bit load, for some reason
    ; inc hl
    ; ld (reg_buffer + 1), (hl)     ;second portion of PC
    ; ld a, (reg_buffer+1)    ;PCh
    ; out (0x3e), a
    ; ld a, (reg_buffer)      ;PCl
    ; out (0x3f), a
    inc hl
    ld a, (HL)      ;PCh
    out (0x3e), a
    dec hl
    ld a, (hl)
    out (0x3f), a


    ; out
    out (0x40), a

    pop hl
    pop af

    out (0x00), a           ;Wait to continue
    ei
    reti
reg_buffer .byte 0, 0;Buffer
old_sp .byte 0, 0

outend     
