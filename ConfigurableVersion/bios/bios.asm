    include "bios.inc"
    output "bios/bios.bin", t

RST0:
    jp init
    
;Args: A, HL, DE, BC
    .porg 0x08
RST8:
    call lookup
    .dw RST8_LOOKUP  ;  Address of lookup table RST8_FUNCS 

    .porg 0x10
RST10:
    call lookup
    .dw RST10_LOOKUP  ;  Address of lookup table RS10_FUNCS

    .porg 0x18
RST18:
    call lookup
    .dw RST18_LOOKUP  ;  Address of lookup table RS18_FUNCS

    .porg 0x20
RST20:
    call lookup
    .dw RST20_LOOKUP  ;  Address of lookup table RS20_FUNCS

    .porg 0x28
RST28:
    call lookup
    .dw RST28_LOOKUP  ;  Address of lookup table RS28_FUNCS

    .porg 0x30
RST30:
    call lookup
    .dw RST30_LOOKUP  ;  Address of lookup table RS30_FUNCS

    .porg 0x38
RST38:
    ei
    reti

    ; .porg 0x100
lookup:
    push de       ;save handler index
    di            ;disable nested interrupts
    exx           ;replace registers with their shadow copies
    pop bc        ;load handler index into bc
    pop hl        ;load return address(with address of lookup table) into hl
    ld de, (hl)   ;load lookup table address into de
    ex de, hl     ;Put the lookup table address into hl
    add hl, bc    ;Get lookup table cell address(table address + handler index)
    ld e, (hl)    ;Load lower byte of handler address
    inc hl        ;Increment by one byte - Z80 is big endian
    ld d, (hl)    ;Load higher byte of handler address
;-------------------------------------------------Current Version
    push de       ;Seems better than FAKE_CALL: push handler address as return address
    exx           ;load back regular registers
    ei            ;enable interrupts
    ret           ;Call handler: stack top will be the return address from RST and will be popped by subsequent return from handler

;-------------------------------------------------Fake call version
    ; ex de, hl   ;Handler address is in HL
;     call FAKE_CALL;Push return address
; FAKE_CALL:
;     ex (sp), hl   ;Change return address to handler address. Disgard old return address
    ; exx           ;load back regular registers
    ; ei            ;enable interrupts
    ; ret           ;Call handler: stack top will be the return address from RST and will be popped by subsequent return from handler
;------------------------------------------------

;------------------------------------------------ Self-modifying version
;     ld (CALL_TO_CHANGE + 1), de ;Set call address to be the one from the table  :SELF_MODYFYING
;     exx
;     ei
; CALL_TO_CHANGE:
;     call    0x0000      ;Dummy address, to be replaced by previous load
;     ret
; -----------------------------------------------


    ; .porg 0x200
RST8_LOOKUP: ;IO handlers
    .dw io_init
    .dw io_putc
    .dw io_puts
    .dw io_bufsize
    .dw io_getc
    .dw io_gets
    .dw io_wait

    ; .porg 0x210
RST10_LOOKUP: ;Storage handlers
    .dw st_init
    .dw st_steps
    .dw st_moves
    .dw st_chunks
    .dw st_readc
    .dw st_writec

RST18_LOOKUP: ;Virtual memory handlers
    .dw vm_init
    .dw vm_reset
    .dw vm_pt_load
    .dw vm_pt_store

RST20_LOOKUP: ;Timer handlers
    .dw tm_init
    .dw tm_scales
    .dw tm_cmps
    .dw tm_ints
    .dw tm_start
    .dw tm_stop
    .dw tm_wait
    .dw tm_valg

RST28_LOOKUP: ;Debug handler
    .dw db_break

RST30_LOOKUP: ;Other device custom handlers
    ;TODO

; Args: character to print in A
    .porg 0x220
io_putc:
    out (0x01), a
    ret

io_puts:
    push hl
io_puts_loop:
    ld a, (hl)
    out (0x01), a
    inc hl
    or a
    jp nz io_puts_loop
io_puts_ret:
    pop hl
    ret

    .porg 0x240
init:
    ld sp, 0x800 
    ld a, 'r'
    ld de, 0x0002
    rst 0x08        ;RST08 with e=0x02 calls putc
    halt

    outend