# Basic Input-Output System(BIOS) for Z80 PC

## Interface

All BIOS functions are called by the **`RST p`** instruction. Different **`RST`** locations indicate different function groups and contents of **`E`** register determine the exact function to be called. Please note that before any such call, the **`D`** register must contain 0.
***
## BIOS calls implementation details

There are 8 different addresses that can be called: `0x00`, `0x08`, `0x10`, `0x18`, `0x20`, `0x28`, `0x30`, `0x38` As there are only 8 bytes free for each of the adresses, function logic cannot be implemented directly there. Instead, low-order nibble(lower 4 bits) of E register are used to look up a specific function address to be called;

### BIOS call example

```armasm
    ;Print 'c' on terminal
    ld de, 0x0002
    ld a, 'c'
    rst 0x08
```

### Call handler implementation example

```armasm
0x08: ld hl, RST08_FUNCS ;function table address(different for each location)
0x0b: add hl, de
0x0c: ld d, (hl)
0x0d: inc hl
0x0e: ld e, (hl)
0x0f: jp (hl)
;
;            ....
;
; Handler functions table
RST08FUNCS: .dw 0x2000   ;clrscreen
            .dw 0x2035   ;putc
            .dw 0x206e   ;puts
;            ....

```

Please note that the actual implementation is slightly more complicated, as to preserve registers


***

## Complete BIOS calls reference (WIP)

### `RST 0x08` - IO functions

| Register `DE` | Handler name | Description |
|---------------|--------------|-------------|
|0x0000         |`putc`        | Output character stored in register **`A`** to screen|
|0x0002         |`puts`        | Output null-terminated string beginning at address **`HL`** to screen |
|0x0004         |`bufsize`     | Return size of input buffer via **`A`** |
|0x0006         |`getc`        | Return one char from input buffer via **`A`** or `0` if it is empty |


### `RST 0x10` - 
