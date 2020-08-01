# Basic Input-Output System(BIOS) for Z80 PC

## Interface

All BIOS functions are called by the **`RST p`** instruction. Different **`RST`** locations indicate different function groups and contents of **`E`** register determine the exact function to be called. Please note that before any such call, the **`D`** register must contain `0`.
***

## BIOS calls implementation details

There are 8 different addresses that can be called: `0x00`, `0x08`, `0x10`, `0x18`, `0x20`, `0x28`, `0x30`, `0x38`. As there are only 8 bytes free for each of the adresses, function logic cannot be implemented directly there. Instead, low-order nibble(lower 4 bits) of E register are used to look up a specific function address to be called;

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
|0x0000         |`io_init`     | Initialize input buffer queue |
|0x0002         |`io_putc`     | Output character stored in register **`A`** to screen|
|0x0004         |`io_puts`     | Output null-terminated string beginning at address **`HL`** to screen |
|0x0006         |`io_bufsize`  | Return number of unread chars in the input buffer via **`A`** |
|0x0008         |`io_getc`     | Return one char from input buffer via **`A`** or `0` if it is empty |
|0x000a         |`io_gets`     | Move contents of input buffer to the location specified by **`HL`** register  |
|0x000c         |`io_wait`     | Halt and wait for corresponding interrupt  |

### `RST 0x10` - Storage communication interface

| Register `DE` | Handler name | Description |
|---------------|--------------|-------------|
|0x0000         |` st_init `   | Initialize storage interface system|
|0x0002         |` st_steps `  | Set the step size to 2 to the power of **`A`**(max 32) |
|0x0004         |` st_moves `  | Move cursor by the number of steps specified in **`HL`** |
|0x0006         |` st_chunks ` | Set the chunk size to 2 to the power of **`A`**(max 32) |
|0x0008         |` st_readc `  | Read one chunk from storage into location specified by **`HL`**|
|0x000a         |` st_writec ` | Write one chunk starting at **`HL`** into the storage device |

### `RST 0x18` - Virtual memory management

| Register `DE` | Handler name  | Description |
|---------------|---------------|-------------|
|0x0000         |` vm_init `    | Initialize virtual memory system |
|0x0002         |` vm_reset `   | Reset pagetable logical address to 0xfe|
|0x0004         |` vm_pt_load ` | Load pagetable starting at **`HL`** into the MMU|
|0x0006         |` vm_pt_store `| Dump pagetable from MMU to RAM starting at **`HL`** |

### `RST 0x20` - Timer configuration

| Register `DE` | Handler name  | Description |
|---------------|---------------|-------------|
|0x0000         |` tm_init `    | Initialize timer system |
|0x0002         |` tm_scales `  | Set timer prescaler to 2 to the power of **`A`**(max 32) |
|0x0004         |` tm_cmps `    | Set timer compare register to **`A`** |
|0x0006         |` tm_ints `    | Set timer interrupt vector to **`A`** |
|0x0008         |` tm_start `   | Start the timer |
|0x000a         |` tm_stop `    | Stop the timer|
|0x000c         |` tm_wait `    | Wait for the timer to trigger an interrupt |
|0x000e         |` tm_valg `    | Get timer count via **`A`** |

### `RST 0x28` - Debug interface

| Register `DE` | Handler name  | Description |
|---------------|---------------|-------------|
|Any            |` db_break `    | Breakpoint. Output the state of the CPU and halt until serial interrupt |

### `RST 0x30` - Other devices

| Register `DE` | Handler name  | Description |
|---------------|---------------|-------------|
|0x0000         |` dv_call `| Call a device handler specified by **`A`** contents; Arguments are passed via memory starting at **`HL`** |
|0x0002         |` dv_add ` | Set a device handler specified by **`A`** contents as a function starting at **`HL`** |

### `RST 0x38` - Mode 1 Interrupt handler

Doesn't do anything, may be used for `db_break`, `tm_wait`, `io_recieve`.
