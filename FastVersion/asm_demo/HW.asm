;==============================================================================
;
; Blink demo
; Z80-MBC2 - HW ref: A040618 
;
;==============================================================================

rx_port         .equ    $01             ; IOS "serial Rx" read port address
opcode_port     .equ    $01             ; IOS opcode write port address
tx_opcode       .equ    $01             ; IOS "serial Tx" operation opcode
usrLED_opcode   .equ    $00             ; IOS "user LED" operation opcode
eos             .equ    $00             ; End of string
cr              .equ    $0d             ; Carriage return
lf              .equ    $0a             ; Line feed

    output "HW.bin", t
                .org    $0000
				jp		start			; reset jump to start
                .block  16              ; Local SP area

;
; Main program
;

start           ld      sp, $           ; Init stack

                ld      hl, message     ; Print a message
                call    puts
                ld      a, $01          ; Blinks...
halt                
message         .byte   cr, lf, "Hello, world!", cr, lf, eos


;
; Send a string to the serial line, HL contains the pointer to the string
;

puts            push    af
                push    hl
puts_loop       ld      a, (hl)
                cp      eos             ; End of string reached?
                jr      z, puts_end     ; Yes
                call    putc
                inc     hl              ; Increment character pointer
                jr      puts_loop       ; Transmit next character
puts_end        pop     hl
                pop     af
                ret

;
; Send a single character to the serial line (A contains the character)
;

putc            push    af              ; Save A
                ld      a, tx_opcode    ; A = IOS Serial Tx operation opcode
                ;out     (opcode_port), a; Send to IOS the Tx operation opcode
                pop     af              ; Restore the output char into A
                out     (01), a ; Write A to the serial
                ret
                
.end
outend