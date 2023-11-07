    .text
    .align 4
    .globl  _start
    .type   _start, @function
_start:
    l.addi  r1, r1, 10
    l.addi  r2, r2, 5
    l.addi  r3, r3, 20
    l.addi  r4, r4, -1
    l.addi  r5, r5, 0
    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
    .word   0x40ffccff
    .size   _start, .-_start