addi x1, x0, 10;
addi x2, x0, -10;
addi x3, x0, 0;
addi x5, x0, 0;
blt x0, x1, 20;
addi x3, x0, 1;
addi x4, x0, -1;
xor x1, x1, x4;
addi x1, x1, 1;
beq x0, x1, 16; LOOP
add x5, x5, x2;
addi x1, x1, -1;
jal x0, -12;
beq x0, x3, 16; DONE
addi x4, x0, -1;
xor  x5, x5, x4;
addi x5, x5, 1;
ebreak; RET