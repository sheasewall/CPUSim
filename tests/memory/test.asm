;     Load test values into registers
addi x5,x0, 10      ; x5 = 10
addi x6,x0, 10      ; x6 = 10 (equal to x5)
addi x7,x0, 20      ; x7 = 20
addi x8,x0, -5      ; x8 = -5
addi x9,x0, -10     ; x9 = -10
;
;
add x10,x5,x7;
add x11,x5,x8;
add x12,x5,x9;