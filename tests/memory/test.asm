addi x1, x0, 10;    x1 = 10
addi x2, x0, 20;    x2 = 20
addi x3, x0, 30;    x3 = 30
addi x4, x0, 40;    x4 = 40
add x5, x1, x2;     x5 = x1 + x2 = 10 + 20 = 30
sub x6, x3, x1;     x6 = x3 - x1 = 30 - 10 = 20
sll x7, x1, x2;     x7 = x1 << (x2 & 0x1F) = 10 << (20 & 0x1F) = 10 << 20
slt x8, x1, x2;     x8 = (x1 < x2) ? 1 : 0 = (10 < 20) ? 1 : 0 = 1
sltu x9, x1, x2;    x9 = (unsigned x1 < unsigned x2) ? 1 : 0 = (10 < 20) ? 1 : 0 = 1
xor x10, x2, x3;    x10 = x1 ^ x2 = 10 ^ 20
srl x11, x7, x8;    x11 = x1 >> (x2 & 0x1F) = 10 >> (20 & 0x1F) = 10 >> 20
sra x12, x7, x8;    x12 = x1 >> (x2 & 0x1F) (arithmetic) = 10 >> 20
or x13, x2, x3;     x13 = x1 | x2 = 10 | 20
and x14, x2, x3;    x14 = x1 & x2 = 10 & 20