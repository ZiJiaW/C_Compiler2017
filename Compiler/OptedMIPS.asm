.data
t140: .asciiz " "
t149: .asciiz " "
t160: .asciiz "gcd of 319, 377 = "
t164: .asciiz " "
t165: .asciiz "gcd of 98, 63 = "
t169: .asciiz "gcd of 1280, 1250 = "
t179: .asciiz "   fib of 20 = "
t186: .asciiz "your array length should be less than 100!!!"
t196: .asciiz "original array: "
t200: .asciiz " "
t207: .asciiz "sorted array: "
t211: .asciiz " "
.text
jal main
li $v0, 10
syscall
gcd:
subi $sp, $sp, 76
sw $ra, 72($sp)
sw $fp, 68($sp)
move $fp, $sp
lw $t0, 80($fp)
li $t1, 0
sw $t0, 0($fp)
bne $t0, $t1, Label_t0
lw $t0, 76($fp)
move $v0, $t0
lw $ra, 72($sp)
lw $fp, 68($sp)
addi $sp, $sp, 84
jr $ra
j Label_t4
Label_t0:
Label_t4:
lw $t0, 76($fp)
li $t1, 0
sw $t0, 8($fp)
bne $t0, $t1, Label_t5
lw $t0, 80($fp)
move $v0, $t0
lw $ra, 72($sp)
lw $fp, 68($sp)
addi $sp, $sp, 84
jr $ra
j Label_t9
Label_t5:
Label_t9:
lw $t0, 80($fp)
lw $t1, 76($fp)
sub $t2, $t0, $t1
sw $t0, 80($fp)
sw $t1, 76($fp)
sw $t2, 24($fp)
bltz $t2, Label_t10
lw $t0, 80($fp)
lw $t1, 76($fp)
sub $t2, $t0, $t1
subi $sp, $sp, 4
sw $t2, 0($sp)
move $t3, $t1
subi $sp, $sp, 4
sw $t3, 0($sp)
sw $t0, 80($fp)
sw $t1, 76($fp)
sw $t2, 36($fp)
sw $t3, 40($fp)
jal gcd
sw $v0, 44($fp)
lw $v0, 44($fp)
lw $ra, 72($sp)
lw $fp, 68($sp)
addi $sp, $sp, 84
jr $ra
j Label_t19
Label_t10:
lw $t0, 80($fp)
subi $sp, $sp, 4
sw $t0, 0($sp)
lw $t1, 76($fp)
lw $t2, 80($fp)
sub $t3, $t1, $t2
subi $sp, $sp, 4
sw $t3, 0($sp)
sw $t0, 48($fp)
sw $t1, 76($fp)
sw $t2, 80($fp)
sw $t3, 60($fp)
jal gcd
sw $v0, 64($fp)
lw $v0, 64($fp)
lw $ra, 72($sp)
lw $fp, 68($sp)
addi $sp, $sp, 84
jr $ra
Label_t19:
lw $ra, 72($sp)
lw $fp, 68($sp)
addi $sp, $sp, 84
jr $ra
compare:
subi $sp, $sp, 36
sw $ra, 32($sp)
sw $fp, 28($sp)
move $fp, $sp
lw $t0, 40($fp)
lw $t1, 36($fp)
sub $t2, $t0, $t1
sw $t0, 40($fp)
sw $t1, 36($fp)
sw $t2, 8($fp)
blez $t2, Label_t25
li $v0, 1
lw $ra, 32($sp)
lw $fp, 28($sp)
addi $sp, $sp, 44
jr $ra
j Label_t30
Label_t25:
Label_t30:
lw $t0, 40($fp)
lw $t1, 36($fp)
sub $t2, $t0, $t1
sw $t0, 40($fp)
sw $t1, 36($fp)
sw $t2, 20($fp)
bgtz $t2, Label_t31
li $t0, 1
neg $t1, $t0
move $v0, $t1
lw $ra, 32($sp)
lw $fp, 28($sp)
addi $sp, $sp, 44
jr $ra
j Label_t37
Label_t31:
Label_t37:
lw $ra, 32($sp)
lw $fp, 28($sp)
addi $sp, $sp, 44
jr $ra
bubblesort:
subi $sp, $sp, 128
sw $ra, 124($sp)
sw $fp, 120($sp)
move $fp, $sp
sw $s0, 116($fp)
sw $s1, 112($fp)
sw $s2, 108($fp)
lw $t0, 128($fp)
subi $s0, $t0, 1
sw $t0, 128($fp)
Label_t41:
li $s1, 0
Label_t44:
move $t0, $s1
sll $t0, $t0, 2
add $t0, $t0, $gp
lw $t1, 0($t0)
addi $t2, $s1, 1
sll $t2, $t2, 2
add $t2, $t2, $gp
lw $t3, 0($t2)
sub $t4, $t1, $t3
sw $t0, 20($fp)
sw $t1, 24($fp)
sw $t2, 32($fp)
sw $t3, 36($fp)
sw $t4, 40($fp)
blez $t4, Label_t46
move $t0, $s1
sll $t0, $t0, 2
add $t0, $t0, $gp
lw $t1, 0($t0)
move $t2, $s1
addi $t3, $s1, 1
move $s2, $t1
sll $t3, $t3, 2
add $t3, $t3, $gp
lw $t4, 0($t3)
sll $t2, $t2, 2
add $t2, $t2, $gp
sw $t4, 0($t2)
move $t5, $s2
addi $t6, $s1, 1
sll $t6, $t6, 2
add $t6, $t6, $gp
sw $t5, 0($t6)
sw $t0, 44($fp)
sw $t1, 48($fp)
sw $t2, 52($fp)
sw $t3, 60($fp)
sw $t4, 64($fp)
sw $t5, 76($fp)
sw $t6, 72($fp)
j Label_t65
Label_t46:
Label_t65:
move $t0, $s1
subi $t1, $s0, 1
addi $s1, $t0, 1
sub $t2, $s1, $t1
sw $t0, 104($fp)
sw $t1, 88($fp)
sw $t2, 92($fp)
blez $t2, Label_t44
subi $s0, $s0, 1
subi $t0, $s0, 1
sw $t0, 100($fp)
bgez $t0, Label_t41
lw $ra, 124($sp)
lw $fp, 120($sp)
lw $s0, 116($sp)
lw $s1, 112($sp)
lw $s2, 108($sp)
addi $sp, $sp, 132
jr $ra
fib:
subi $sp, $sp, 44
sw $ra, 40($sp)
sw $fp, 36($sp)
move $fp, $sp
lw $t0, 44($fp)
li $t1, 0
sw $t0, 0($fp)
bne $t0, $t1, Label_t74
li $v0, 0
lw $ra, 40($sp)
lw $fp, 36($sp)
addi $sp, $sp, 48
jr $ra
j Label_t78
Label_t74:
Label_t78:
lw $t0, 44($fp)
li $t1, 1
sw $t0, 4($fp)
bne $t0, $t1, Label_t79
li $v0, 1
lw $ra, 40($sp)
lw $fp, 36($sp)
addi $sp, $sp, 48
jr $ra
j Label_t83
Label_t79:
Label_t83:
lw $t0, 44($fp)
subi $t1, $t0, 1
subi $sp, $sp, 4
sw $t1, 0($sp)
sw $t0, 44($fp)
sw $t1, 12($fp)
jal fib
sw $v0, 16($fp)
lw $t0, 44($fp)
subi $t1, $t0, 2
subi $sp, $sp, 4
sw $t1, 0($sp)
sw $t0, 44($fp)
sw $t1, 24($fp)
jal fib
sw $v0, 28($fp)
lw $t0, 16($fp)
lw $t1, 28($fp)
add $t2, $t0, $t1
move $v0, $t2
lw $ra, 40($sp)
lw $fp, 36($sp)
addi $sp, $sp, 48
jr $ra
lw $ra, 40($sp)
lw $fp, 36($sp)
addi $sp, $sp, 48
jr $ra
complexfor:
subi $sp, $sp, 304
sw $ra, 300($sp)
sw $fp, 296($sp)
move $fp, $sp
sw $s0, 292($fp)
sw $s1, 288($fp)
sw $s2, 284($fp)
sw $s3, 280($fp)
sw $s4, 276($fp)
sw $s5, 272($fp)
sw $s6, 268($fp)
sw $s7, 264($fp)
li $s1, 1
li $s2, 1
Label_t96:
move $t0, $s1
move $t1, $s1
sll $t0, $t0, 2
add $t0, $t0, $fp
sw $t1, 72($t0)
move $t2, $s1
move $t3, $s1
sll $t2, $t2, 2
add $t2, $t2, $fp
sw $t3, 32($t2)
addi $s1, $s1, 1
subi $t4, $s1, 8
sw $t0, 112($fp)
sw $t1, 116($fp)
sw $t2, 120($fp)
sw $t3, 124($fp)
sw $t4, 132($fp)
blez $t4, Label_t96
li $s1, 1
Label_t106:
li $s0, 2
li $t0, 2
li $s5, 3
li $t1, 0
sw $t0, 136($fp)
beq $t0, $t1, Label_t110
move $t0, $s0
subi $sp, $sp, 4
sw $t0, 0($sp)
subi $sp, $sp, 4
li $t1, 0
sw $t1, 0($sp)
sw $t0, 140($fp)
jal compare
sw $v0, 144($fp)
lw $t0, 144($fp)
move $t1, $s5
lw $t2, 144($fp)
mulo $t3, $t2, $s0
sll $t1, $t1, 2
add $t1, $t1, $fp
lw $t4, 72($t1)
li $s2, 1
move $s6, $t4
sw $t0, 16($fp)
sw $t1, 160($fp)
sw $t2, 144($fp)
sw $t3, 0($fp)
sw $t4, 164($fp)
Label_t122:
lw $t0, 16($fp)
add $t1, $s6, $t0
sll $t1, $t1, 2
add $t1, $t1, $fp
lw $t2, 32($t1)
move $s4, $t2
move $t3, $t2
sll $t3, $t3, 2
add $t3, $t3, $fp
lw $t4, 72($t3)
move $t5, $s4
move $t6, $s6
move $s7, $t4
sll $t6, $t6, 2
add $t6, $t6, $fp
sw $t5, 32($t6)
move $t7, $s6
move $t8, $s4
sll $t8, $t8, 2
add $t8, $t8, $fp
sw $t7, 72($t8)
li $s3, 1
move $s6, $s7
sw $t0, 16($fp)
sw $t1, 176($fp)
sw $t2, 180($fp)
sw $t3, 184($fp)
sw $t4, 188($fp)
sw $t5, 196($fp)
sw $t6, 192($fp)
sw $t7, 204($fp)
sw $t8, 200($fp)
Label_t136:
move $t0, $s3
sll $t0, $t0, 2
add $t0, $t0, $fp
lw $t1, 32($t0)
move $a0, $t1
li $v0, 1
syscall
la $a0, t140
li $v0, 4
syscall
addi $s3, $s3, 1
subi $t2, $s3, 8
sw $t0, 212($fp)
sw $t1, 216($fp)
sw $t2, 224($fp)
blez $t2, Label_t136
li $s3, 1
Label_t145:
move $t0, $s3
sll $t0, $t0, 2
add $t0, $t0, $fp
lw $t1, 72($t0)
move $a0, $t1
li $v0, 1
syscall
la $a0, t149
li $v0, 4
syscall
addi $s3, $s3, 1
subi $t2, $s3, 8
sw $t0, 228($fp)
sw $t1, 232($fp)
sw $t2, 240($fp)
blez $t2, Label_t145
addi $s2, $s2, 1
lw $t0, 0($fp)
sub $t1, $s2, $t0
sw $t0, 0($fp)
sw $t1, 252($fp)
blez $t1, Label_t122
j Label_t156
Label_t110:
Label_t156:
addi $s1, $s1, 1
subi $t0, $s1, 1
sw $t0, 260($fp)
blez $t0, Label_t106
lw $ra, 300($sp)
lw $fp, 296($sp)
lw $s0, 292($sp)
lw $s1, 288($sp)
lw $s2, 284($sp)
lw $s3, 280($sp)
lw $s4, 276($sp)
lw $s5, 272($sp)
lw $s6, 268($sp)
lw $s7, 264($sp)
addi $sp, $sp, 304
jr $ra
main:
subi $sp, $sp, 128
sw $ra, 124($sp)
move $fp, $sp
la $a0, t160
li $v0, 4
syscall
subi $sp, $sp, 4
li $t0, 319
sw $t0, 0($sp)
subi $sp, $sp, 4
li $t0, 377
sw $t0, 0($sp)
jal gcd
sw $v0, 12($fp)
lw $a0, 12($fp)
li $v0, 1
syscall
la $a0, t164
li $v0, 4
syscall
la $a0, t165
li $v0, 4
syscall
subi $sp, $sp, 4
li $t0, 98
sw $t0, 0($sp)
subi $sp, $sp, 4
li $t0, 63
sw $t0, 0($sp)
jal gcd
sw $v0, 16($fp)
lw $a0, 16($fp)
li $v0, 1
syscall
la $a0, t169
li $v0, 4
syscall
subi $sp, $sp, 4
li $t0, 1280
sw $t0, 0($sp)
subi $sp, $sp, 4
li $t0, 1250
sw $t0, 0($sp)
jal gcd
sw $v0, 20($fp)
lw $t0, 20($fp)
mulo $t1, $t0, 10
div $t2, $t1, 20
mulo $t3, $t2, 2
move $a0, $t3
li $v0, 1
syscall
sw $t0, 20($fp)
sw $t1, 24($fp)
sw $t2, 28($fp)
sw $t3, 32($fp)
jal complexfor
la $a0, t179
li $v0, 4
syscall
subi $sp, $sp, 4
li $t0, 20
sw $t0, 0($sp)
jal fib
sw $v0, 36($fp)
lw $a0, 36($fp)
li $v0, 1
syscall
li $v0, 5
syscall
move $s2, $v0
subi $t0, $s2, 100
sw $t0, 44($fp)
blez $t0, Label_t182
la $a0, t186
li $v0, 4
syscall
lw $ra, 124($sp)
addi $sp, $sp, 128
jr $ra
j Label_t187
Label_t182:
Label_t187:
li $s0, 0
Label_t189:
li $v0, 5
syscall
move $s1, $v0
move $t0, $s1
move $t1, $s0
sll $t1, $t1, 2
add $t1, $t1, $gp
sw $t0, 0($t1)
addi $s0, $s0, 1
sub $t2, $s0, $s2
sw $t0, 52($fp)
sw $t1, 48($fp)
sw $t2, 64($fp)
bltz $t2, Label_t189
la $a0, t196
li $v0, 4
syscall
li $s0, 0
Label_t198:
la $a0, t200
li $v0, 4
syscall
move $t0, $s0
sll $t0, $t0, 2
add $t0, $t0, $gp
lw $t1, 0($t0)
move $a0, $t1
li $v0, 1
syscall
addi $s0, $s0, 1
sub $t2, $s0, $s2
sw $t0, 68($fp)
sw $t1, 72($fp)
sw $t2, 84($fp)
bltz $t2, Label_t198
move $t0, $s2
subi $sp, $sp, 4
sw $t0, 0($sp)
sw $t0, 88($fp)
jal bubblesort
la $a0, t207
li $v0, 4
syscall
li $s0, 0
Label_t209:
la $a0, t211
li $v0, 4
syscall
move $t0, $s0
sll $t0, $t0, 2
add $t0, $t0, $gp
lw $t1, 0($t0)
move $a0, $t1
li $v0, 1
syscall
addi $s0, $s0, 1
sub $t2, $s0, $s2
sw $t0, 92($fp)
sw $t1, 96($fp)
sw $t2, 108($fp)
bltz $t2, Label_t209
lw $ra, 124($sp)
addi $sp, $sp, 128
jr $ra
