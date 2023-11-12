.text
.align 4
.globl _start
.type _start, @function

_start:
    l.nop
    .word 0x40ffccff
    .size _start, .-_start
