.text
.align 4
.globl _start
.type _start, @function

_start:
    l.movhi r2, 0xABCD1234
    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start
