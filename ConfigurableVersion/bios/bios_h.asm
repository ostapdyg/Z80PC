    macro .porg addr
        if $ < addr
        .block addr-$
        endif
        .org addr
    endm

    ; define print