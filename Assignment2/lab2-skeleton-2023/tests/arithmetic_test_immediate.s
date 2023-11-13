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
    l.addi r1, r0, 0x7
    l.andi r3, r0, 0xa
    l.ori  r4, r0, 0xb
    l.slli r5, r4, 0x3
    l.srli r6, r1, 0x2
    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start
