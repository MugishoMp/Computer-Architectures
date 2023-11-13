.text
.align 4
.globl _start
.type _start, @function

_start:
    l.addi r2, r0, 1
    l.addi r3, r0, 5
    l.sfeq r1, r2
    l.bf no_branch_1
    l.addi r3, r0, 1
    l.sfeq r1, r2
    l.bf end_program
    l.nop

end_program:
    l.addi r4, r0, 66
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start