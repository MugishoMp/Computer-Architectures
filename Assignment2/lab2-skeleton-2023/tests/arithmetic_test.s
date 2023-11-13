.text
.align 4
.globl _start
.type _start, @function

_start:
    l.add r1, r2, r3
    l.sub r2, r4, r5
    l.and r3, r1, r2
    l.or  r4, r1, r2
    l.sll r5, r4, r3
    l.srl r6, r2, r3
    l.sfeq r7, r4, r5
    l.sfgtu r8, r5, r4
    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start
