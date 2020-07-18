    org 0
    ; ld a, 'c'
    ; ld bc, 0x0800
    ; out (c), a
    jp boot


msg:
    ; ld  bc,10*256   ; Busy status port
    ; in  a,(c)       ; Poll until the port is not busy
    ; bit 0,a
    ; jnz msg
    ld  bc, 0x0800    ; Port to write a character out
    out (c),e
    ret


page_table_setup:           ; pre-defined mmu
;CHNG_PG: dw 0xFF01 
    ld HL, 0xffff
    ld A, 0xff
pts_loop:
    ld (HL), A  
    ld E, L
    call msg
    ld E, A
    call msg
    dec HL
    dec A
    jp nz, pts_loop
    ret

readback:
    ld E, 3
    call msg
    ld HL, fill_ram
    ld DE, fill_ram_end
read_loop:
    ld A, H
    cp D
    ret z
    ;ld BC, 0x0800
    ;ld A, (HL)
    ; out (C), A
    ld E, (HL)
    call msg
    inc H
    jp read_loop 

;     org 0x80 ;IM 0x80 IORQ response
; im2_handler:
;     jmp int_common
boot:
    ;initial setup
    ld SP, 0xffdf
program:                    ; read virtual address -> display physical address and data
    ld E, 0xff
    call msg
    call msg
    ; call page_table_setup
    ; call readback
    ; halt

;---------------------
;----RAM fill loop----
;---------------------
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


; read_end:
;     ret



;0000 - 000000
;0100 - 000100
;0200 - 000200 

;   org 0x080
;     dw  im2_handler
; im2_handler:
;     push de
;     ld  de,int_im2_msg
;     jmp int_common
; int_im2_msg:
;     db  "_IM2_",'$'
; boot: