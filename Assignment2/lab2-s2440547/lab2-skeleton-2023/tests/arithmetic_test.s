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
    l.add r1, r2, r3
    l.sub r2, r4, r5
    l.and r3, r1, r2
    l.or  r4, r1, r2
    l.sll r5, r4, r3
    l.srl r6, r2, r3
    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
    .word 0x40ffccff
    .size _start, .-_start
