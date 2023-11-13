.text
.align 4
.globl _start
.type _start, @function

_start:
    l.add r1, r2, r3       # R1 = R2 + R3
    l.sub r2, r4, r5       # R2 = R4 - R5
    l.and r3, r1, r2       # R3 = R1 & R2
    l.or  r4, r1, r2       # R4 = R2 | R5
    l.sll r5, r4, r3       # R5 = R3 << R4
    l.srl r6, r2, r3       # R6 = R2 >> R3
    l.sfeq r7, r4, r5      # R7 = (R4 == R5)
    l.sfgtu r8, r5, r4     # R8 = (R5 >u R4)

    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start
