    include "bios_h.asm"
    output "bios/bios.bin", t

prev:
    ; block print-$
    ; ; disp smth
    .porg 5h
print:
    jp print    ; jp 100
    outend