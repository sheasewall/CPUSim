addi x1, x0, 5;
addi x2, x0, 2000;
addi x3, x0, 1024;
addi x4, x0, 2047;
addi x5, x0, -2047;
addi x6, x0, -2048;
addi x7, x0, -1;
ebreak;
sw x7, 0(x0);
sh x4, 0(x0);
sb x2, 3(x0);
ebreak;
ecall;