# 10 + 3 = 13
# 7 - 2 = 5
# 1101 & 0101 = 0101
# 1101 | 0101 = 1101
# 1101 << 5 = 1 1010 0000
# 0101 >> 5 = 0

.text
.align 4
.globl _start
.type _start, @function

_start:
    l.addi r1, r2, 0x3
    l.subi r2, r4, 0x2
    l.andi r3, r1, 0xa
    l.ori  r4, r1, 0xa
    l.slli r5, r4, 0x3
    l.srli r6, r2, 0x3
    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start
