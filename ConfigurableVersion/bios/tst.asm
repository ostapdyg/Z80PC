    output "tst.bin", t

    org 0x08
;Args: A, HL, DE, BC
RST8:
    call lookup
    .dw 0x1080  ;  Address of lookup table RST8_FUNCS 


RST10:
    call lookup
    .dw 0x1100  ;  Address of lookup table RS10_FUNCS

lookup:
    push de       ;save handler index
    di
    exx           ;replace egisters with their shadow copies
    pop bc        ;load handler index into bc
    pop hl        ;load return address(with address of lookup table) into hl
    ld de, (hl)   ;load lookup table address into de
    ex de, hl     ;
    ; ld de, bc
    add hl, bc    ;Get lookup table cell address(table address + handler index)
    ld e, (hl)    ;Load lower byte of handler address
    inc hl
    ld d, (hl)    ;Load higher byte of handler address
;-------------------------------------------------Fake call version
    ex de, hl     ;Handler address is in HL
    call FAKE_CALL;Push return address 
FAKE_CALL:
    ex (sp), hl   ;Change return address to handler address
    exx
    ei
    ret           ;Call handler: stack top will be the return address from RST
;-------------------------------------------------
;     ; ------------------------------------------------ Self-modyfying version
;     ld (CALL_TO_CHANGE + 1), de ;Set call address to be the one from the table  :SELF_MODYFYING
;     exx
;     ei
; CALL_TO_CHANGE:
;     call    0x0000      ;
;     ret
;     ; ---------------------------------------------------------


    outend