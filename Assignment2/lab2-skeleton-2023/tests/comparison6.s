.text
.align 4
.globl _start
.type _start, @function


end_program:
    l.addi r4, r0, 66
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start

_start:
    l.addi r2, r0, 1
    l.addi r3, r0, 1
    l.sfges r3, r2
    l.bnf end_program
    l.addi r2, r0, 0xffff
    l.sfges r3, r2
    l.bf end_program
    l.nop
