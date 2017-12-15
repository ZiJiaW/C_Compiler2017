.data
t272: .asciiz "i shouldn't < 0!"
t285: .asciiz "i should be 10, not "
t297: .asciiz "In case 2: k="
t308: .asciiz "In case 4: reduce(104)="
t332: .asciiz "In case 5: reduce_to_m(100,50)="
t355: .asciiz "   "
t359: .asciiz "result: "
t360: .asciiz "default:"
t20: .asciiz " "
t21: .asciiz " "
t22: .asciiz " "
t23: .asciiz " "
t26: .asciiz " "
t136: .asciiz "-t*(r+31-(e-'0')/3+0)+(-+11/5+(ch2[9]-'a')*2)<=r+31"
t138: .asciiz "-t*(r+31-(e-'0')/3+0)+(-+11/5+(ch2[9]-'a')*2)>r+31"
t182: .asciiz " "
t62: .asciiz " "
t71: .asciiz "For is ok!"
.text
jal main
li $v0, 10
syscall
testDeclare:
subi $sp, $sp, 176
sw $ra, 172($sp)
sw $fp, 168($sp)
move $fp, $sp
sw $s0, 164($fp)
sw $s1, 160($fp)
sw $s2, 156($fp)
sw $s3, 152($fp)
sw $s4, 148($fp)
li $t0, 312
li $t1, 142
add $t2, $t0, $t1
move $s3, $t2
li $t3, 97
subi $t4, $t3, 32
move $s0, $t4
li $t5, 67
addi $t6, $t5, 32
move $s1, $t6
li $t7, 51
addi $t8, $t7, 1
move $s4, $t8
li $t9, 312
sw $t0, 88($fp)
subi $t0, $t9, 300
sw $t1, 92($fp)
li $t1, 0
sll $at, $t1, 2
add $at, $at, $fp
sw $t0, 0($at)
sw $t2, 96($fp)
li $t2, 142
sw $t3, 100($fp)
subi $t3, $t2, 140
sw $t4, 104($fp)
li $t4, 1
sll $at, $t4, 2
add $at, $at, $fp
sw $t3, 0($at)
move $a0, $s3
li $v0, 1
syscall
la $a0, t20
li $v0, 4
syscall
move $a0, $s0
li $v0, 11
syscall
la $a0, t21
li $v0, 4
syscall
move $a0, $s1
li $v0, 11
syscall
la $a0, t22
li $v0, 4
syscall
move $a0, $s4
li $v0, 11
syscall
la $a0, t23
li $v0, 4
syscall
sw $t5, 108($fp)
sw $t6, 112($fp)
li $t6, 0
sll $at, $t6, 2
add $at, $at, $fp
lw $t5, 0($at)
move $a0, $t5
li $v0, 1
syscall
la $a0, t26
li $v0, 4
syscall
sw $t7, 116($fp)
sw $t8, 120($fp)
li $t8, 1
sll $at, $t8, 2
add $at, $at, $fp
lw $t7, 0($at)
move $a0, $t7
li $v0, 1
syscall
lw $ra, 172($sp)
sw $t0, 128($fp)
sw $t2, 132($fp)
sw $t3, 136($fp)
sw $t5, 140($fp)
sw $t7, 144($fp)
sw $t9, 124($fp)
lw $fp, 168($sp)
lw $s0, 164($sp)
lw $s1, 160($sp)
lw $s2, 156($sp)
lw $s3, 152($sp)
lw $s4, 148($sp)
addi $sp, $sp, 176
jr $ra
testForAndArray:
subi $sp, $sp, 140
sw $ra, 136($sp)
sw $fp, 132($sp)
move $fp, $sp
sw $s0, 128($fp)
sw $s1, 124($fp)
sw $s2, 120($fp)
sw $s3, 116($fp)
li $t0, 97
move $s2, $t0
sw $t0, 16($fp)
Label_t30:
move $t0, $s2
li $t1, 97
sub $t2, $t0, $t1
move $t3, $s2
sll $at, $t2, 2
add $at, $at, $gp
sw $t3, 1548($at)
move $t4, $s2
li $t5, 97
sub $t6, $t4, $t5
sll $at, $t6, 2
add $at, $at, $gp
lw $t7, 1548($at)
move $a0, $t7
li $v0, 11
syscall
addi $s2, $s2, 1
move $t8, $s2
subi $t9, $t8, 122
sw $t0, 20($fp)
sw $t1, 24($fp)
sw $t2, 28($fp)
sw $t3, 32($fp)
sw $t4, 36($fp)
sw $t5, 40($fp)
sw $t6, 44($fp)
sw $t7, 48($fp)
sw $t8, 52($fp)
sw $t9, 56($fp)
blez $t9, Label_t30
li $s0, 50
li $s3, 0
li $s1, 100
Label_t46:
move $t0, $s3
move $t1, $s1
add $t2, $t0, $t1
move $s3, $t2
move $t3, $s1
move $t4, $s0
sw $t0, 60($fp)
sw $t1, 64($fp)
sw $t2, 68($fp)
sw $t3, 72($fp)
sw $t4, 76($fp)
bne $t3, $t4, Label_t51
li $s0, 20
j Label_t55
Label_t51:
move $t0, $s1
subi $t1, $t0, 1
move $s1, $t1
sw $t0, 80($fp)
sw $t1, 84($fp)
Label_t55:
subi $s1, $s1, 1
move $t0, $s1
move $t1, $s0
sub $t2, $t0, $t1
sw $t0, 88($fp)
sw $t1, 92($fp)
sw $t2, 96($fp)
bgez $t2, Label_t46
la $a0, t62
li $v0, 4
syscall
move $a0, $s3
li $v0, 1
syscall
li $s1, 0
Label_t64:
move $t0, $s1
subi $t1, $t0, 0
sw $t0, 100($fp)
sw $t1, 104($fp)
bgez $t1, Label_t66
lw $ra, 136($sp)
lw $fp, 132($sp)
lw $s0, 128($sp)
lw $s1, 124($sp)
lw $s2, 120($sp)
lw $s3, 116($sp)
addi $sp, $sp, 140
jr $ra
j Label_t70
Label_t66:
Label_t70:
la $a0, t71
li $v0, 4
syscall
subi $s1, $s1, 1
move $t0, $s1
subi $t1, $t0, 0
sw $t0, 108($fp)
sw $t1, 112($fp)
bltz $t1, Label_t64
lw $ra, 136($sp)
lw $fp, 132($sp)
lw $s0, 128($sp)
lw $s1, 124($sp)
lw $s2, 120($sp)
lw $s3, 116($sp)
addi $sp, $sp, 140
jr $ra
sum:
subi $sp, $sp, 20
sw $ra, 16($sp)
sw $fp, 12($sp)
move $fp, $sp
lw $t0, 24($fp)
lw $t1, 20($fp)
add $t2, $t0, $t1
move $v0, $t2
lw $ra, 16($sp)
sw $t0, 0($fp)
sw $t1, 4($fp)
sw $t2, 8($fp)
lw $fp, 12($sp)
addi $sp, $sp, 28
jr $ra
lw $ra, 16($sp)
lw $fp, 12($sp)
addi $sp, $sp, 28
jr $ra
testExpression:
subi $sp, $sp, 352
sw $ra, 348($sp)
sw $fp, 344($sp)
move $fp, $sp
sw $s0, 340($fp)
sw $s1, 336($fp)
sw $s2, 332($fp)
li $s2, 0
li $t0, 32
li $t1, -13
addi $t2, $t1, 31
li $t3, 57
subi $t4, $t3, 48
div $t5, $t4, 3
sub $t6, $t2, $t5
addi $t7, $t6, 0
mulo $t8, $t0, $t7
neg $t9, $t8
sw $t0, 12($fp)
li $t0, 2
sw $t1, 16($fp)
neg $t1, $t0
sw $t2, 20($fp)
sw $t3, 24($fp)
li $t3, 9
sll $at, $t3, 2
add $at, $at, $gp
lw $t2, 1548($at)
sw $t4, 28($fp)
subi $t4, $t2, 97
sw $t5, 32($fp)
mulo $t5, $t4, 2
sw $t6, 36($fp)
add $t6, $t1, $t5
sw $t7, 40($fp)
add $t7, $t9, $t6
move $s0, $t7
move $a0, $s0
li $v0, 1
syscall
sw $t8, 44($fp)
li $t8, 32
sw $t9, 48($fp)
li $t9, -13
sw $t0, 52($fp)
addi $t0, $t9, 31
sw $t1, 56($fp)
li $t1, 57
sw $t2, 60($fp)
subi $t2, $t1, 48
div $t3, $t2, 3
sw $t4, 64($fp)
sub $t4, $t0, $t3
sw $t5, 68($fp)
addi $t5, $t4, 0
sw $t6, 72($fp)
mulo $t6, $t8, $t5
sw $t7, 76($fp)
neg $t7, $t6
sw $t8, 80($fp)
li $t8, 2
sw $t9, 84($fp)
neg $t9, $t8
sw $t0, 88($fp)
sw $t1, 92($fp)
li $t1, 9
sll $at, $t1, 2
add $at, $at, $gp
lw $t0, 1548($at)
sw $t2, 96($fp)
subi $t2, $t0, 97
sw $t3, 100($fp)
mulo $t3, $t2, 2
sw $t4, 104($fp)
add $t4, $t9, $t3
sw $t5, 108($fp)
add $t5, $t7, $t4
sw $t6, 112($fp)
li $t6, -13
sw $t7, 116($fp)
addi $t7, $t6, 31
sw $t8, 120($fp)
sub $t8, $t5, $t7
sw $t0, 128($fp)
sw $t2, 132($fp)
sw $t3, 136($fp)
sw $t4, 140($fp)
sw $t5, 144($fp)
sw $t6, 148($fp)
sw $t7, 152($fp)
sw $t8, 156($fp)
sw $t9, 124($fp)
bgtz $t8, Label_t105
la $a0, t136
li $v0, 4
syscall
j Label_t137
Label_t105:
la $a0, t138
li $v0, 4
syscall
Label_t137:
li $t0, 3
neg $t1, $t0
addi $t2, $t1, 0
li $t3, -13
addi $t4, $t3, 31
subi $sp, $sp, 4
sw $t4, 0($sp)
li $t5, 57
subi $t6, $t5, 48
div $t7, $t6, 3
neg $t8, $t7
addi $t9, $t8, 0
subi $sp, $sp, 4
sw $t9, 0($sp)
sw $t1, 160($fp)
sw $t2, 164($fp)
sw $t3, 168($fp)
sw $t4, 172($fp)
sw $t5, 176($fp)
sw $t6, 180($fp)
sw $t7, 184($fp)
sw $t8, 188($fp)
sw $t9, 192($fp)
jal sum
sw $v0, 196($fp)
lw $t0, 164($fp)
lw $t1, 196($fp)
add $t2, $t0, $t1
move $s1, $t2
sw $t0, 164($fp)
sw $t1, 196($fp)
sw $t2, 200($fp)
Label_t156:
move $t0, $s2
move $t1, $s1
add $t2, $t0, $t1
move $s2, $t2
addi $s1, $s1, 1
move $t3, $s1
li $t5, 9
sll $at, $t5, 2
add $at, $at, $gp
lw $t4, 1548($at)
subi $t6, $t4, 97
mulo $t7, $t6, 2
addi $t8, $t7, 15
subi $sp, $sp, 4
sw $t8, 0($sp)
li $t9, 15
sw $t0, 204($fp)
div $t0, $t9, 7
sw $t1, 208($fp)
neg $t1, $t0
sw $t2, 212($fp)
li $t2, 8
sw $t3, 216($fp)
sub $t3, $t1, $t2
subi $sp, $sp, 4
sw $t3, 0($sp)
sw $t0, 240($fp)
sw $t1, 244($fp)
sw $t2, 248($fp)
sw $t3, 252($fp)
sw $t4, 220($fp)
sw $t6, 224($fp)
sw $t7, 228($fp)
sw $t8, 232($fp)
sw $t9, 236($fp)
jal sum
sw $v0, 256($fp)
lw $t0, 216($fp)
lw $t1, 256($fp)
sub $t2, $t0, $t1
sw $t0, 216($fp)
sw $t1, 256($fp)
sw $t2, 260($fp)
blez $t2, Label_t156
la $a0, t182
li $v0, 4
syscall
move $a0, $s1
li $v0, 1
syscall
li $t0, 32
li $t1, -13
addi $t2, $t1, 31
li $t3, 57
subi $t4, $t3, 48
div $t5, $t4, 3
sub $t6, $t2, $t5
addi $t7, $t6, 0
mulo $t8, $t0, $t7
neg $t9, $t8
sw $t0, 264($fp)
li $t0, 2
sw $t1, 268($fp)
neg $t1, $t0
sw $t2, 272($fp)
sw $t3, 276($fp)
li $t3, 9
sll $at, $t3, 2
add $at, $at, $gp
lw $t2, 1548($at)
sw $t4, 280($fp)
subi $t4, $t2, 97
sw $t5, 284($fp)
mulo $t5, $t4, 2
sw $t6, 288($fp)
add $t6, $t1, $t5
sw $t7, 292($fp)
add $t7, $t9, $t6
move $v0, $t7
lw $ra, 348($sp)
sw $t0, 304($fp)
sw $t1, 308($fp)
sw $t2, 312($fp)
sw $t4, 316($fp)
sw $t5, 320($fp)
sw $t6, 324($fp)
sw $t7, 328($fp)
sw $t8, 296($fp)
sw $t9, 300($fp)
lw $fp, 344($sp)
lw $s0, 340($sp)
lw $s1, 336($sp)
lw $s2, 332($sp)
addi $sp, $sp, 352
jr $ra
lw $ra, 348($sp)
lw $fp, 344($sp)
lw $s0, 340($sp)
lw $s1, 336($sp)
lw $s2, 332($sp)
addi $sp, $sp, 352
jr $ra
sq_dist:
subi $sp, $sp, 96
sw $ra, 92($sp)
sw $fp, 88($sp)
move $fp, $sp
sw $s0, 84($fp)
lw $t0, 104($fp)
lw $t1, 108($fp)
sub $t2, $t0, $t1
lw $t3, 104($fp)
lw $t4, 108($fp)
sub $t5, $t3, $t4
mulo $t6, $t2, $t5
lw $t7, 96($fp)
lw $t8, 100($fp)
sub $t9, $t7, $t8
sw $t0, 4($fp)
lw $t0, 96($fp)
sw $t1, 8($fp)
lw $t1, 100($fp)
sw $t2, 12($fp)
sub $t2, $t0, $t1
sw $t3, 16($fp)
mulo $t3, $t9, $t2
sw $t4, 20($fp)
add $t4, $t6, $t3
move $s0, $t4
sw $t5, 24($fp)
move $t5, $s0
sw $t6, 28($fp)
addi $t6, $t5, 1
sw $t7, 32($fp)
mulo $t7, $t6, 2
sw $t8, 36($fp)
div $t8, $t7, 2
sw $t9, 40($fp)
subi $t9, $t8, 1
move $v0, $t9
lw $ra, 92($sp)
sw $t0, 44($fp)
sw $t1, 48($fp)
sw $t2, 52($fp)
sw $t3, 56($fp)
sw $t4, 60($fp)
sw $t5, 64($fp)
sw $t6, 68($fp)
sw $t7, 72($fp)
sw $t8, 76($fp)
sw $t9, 80($fp)
lw $fp, 88($sp)
lw $s0, 84($sp)
addi $sp, $sp, 112
jr $ra
lw $ra, 92($sp)
lw $fp, 88($sp)
lw $s0, 84($sp)
addi $sp, $sp, 112
jr $ra
fact:
subi $sp, $sp, 32
sw $ra, 28($sp)
sw $fp, 24($sp)
move $fp, $sp
lw $t0, 32($fp)
li $t1, 1
sw $t0, 0($fp)
bne $t0, $t1, Label_t233
li $v0, 1
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 36
jr $ra
j Label_t237
Label_t233:
Label_t237:
lw $t0, 32($fp)
lw $t1, 32($fp)
subi $t2, $t1, 1
subi $sp, $sp, 4
sw $t2, 0($sp)
sw $t0, 4($fp)
sw $t1, 8($fp)
sw $t2, 12($fp)
jal fact
sw $v0, 16($fp)
lw $t0, 4($fp)
lw $t1, 16($fp)
mulo $t2, $t0, $t1
move $v0, $t2
lw $ra, 28($sp)
sw $t0, 4($fp)
sw $t1, 16($fp)
sw $t2, 20($fp)
lw $fp, 24($sp)
addi $sp, $sp, 36
jr $ra
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 36
jr $ra
reduce:
subi $sp, $sp, 32
sw $ra, 28($sp)
sw $fp, 24($sp)
move $fp, $sp
lw $t0, 32($fp)
li $t1, 1
sw $t0, 0($fp)
bne $t0, $t1, Label_t244
li $v0, 1
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 36
jr $ra
j Label_t248
Label_t244:
Label_t248:
lw $t0, 32($fp)
lw $t1, 32($fp)
subi $t2, $t1, 1
subi $sp, $sp, 4
sw $t2, 0($sp)
sw $t0, 4($fp)
sw $t1, 8($fp)
sw $t2, 12($fp)
jal reduce
sw $v0, 16($fp)
lw $t0, 4($fp)
lw $t1, 16($fp)
add $t2, $t0, $t1
move $v0, $t2
lw $ra, 28($sp)
sw $t0, 4($fp)
sw $t1, 16($fp)
sw $t2, 20($fp)
lw $fp, 24($sp)
addi $sp, $sp, 36
jr $ra
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 36
jr $ra
reduce_to_m:
subi $sp, $sp, 44
sw $ra, 40($sp)
sw $fp, 36($sp)
move $fp, $sp
sw $s0, 32($fp)
li $s0, 0
Label_t256:
move $t0, $s0
lw $t1, 48($fp)
add $t2, $t0, $t1
move $s0, $t2
lw $t3, 48($fp)
subi $t3, $t3, 1
move $t4, $t3
lw $t5, 44($fp)
sub $t6, $t4, $t5
sw $t0, 4($fp)
sw $t1, 8($fp)
sw $t2, 12($fp)
sw $t3, 48($fp)
sw $t4, 16($fp)
sw $t5, 20($fp)
sw $t6, 24($fp)
bgez $t6, Label_t256
move $t0, $s0
move $v0, $t0
lw $ra, 40($sp)
sw $t0, 28($fp)
lw $fp, 36($sp)
lw $s0, 32($sp)
addi $sp, $sp, 52
jr $ra
lw $ra, 40($sp)
lw $fp, 36($sp)
lw $s0, 32($sp)
addi $sp, $sp, 52
jr $ra
Lower:
subi $sp, $sp, 16
sw $ra, 12($sp)
sw $fp, 8($sp)
move $fp, $sp
lw $t0, 16($fp)
addi $t1, $t0, 32
move $v0, $t1
lw $ra, 12($sp)
sw $t0, 0($fp)
sw $t1, 4($fp)
lw $fp, 8($sp)
addi $sp, $sp, 20
jr $ra
lw $ra, 12($sp)
lw $fp, 8($sp)
addi $sp, $sp, 20
jr $ra
main:
subi $sp, $sp, 536
sw $ra, 532($sp)
move $fp, $sp
li $v0, 5
syscall
move $s0, $v0
li $v0, 12
syscall
move $s3, $v0
move $t0, $s0
subi $t1, $t0, 0
sw $t0, 296($fp)
sw $t1, 300($fp)
bgez $t1, Label_t268
la $a0, t272
li $v0, 4
syscall
lw $ra, 532($sp)
addi $sp, $sp, 536
jr $ra
j Label_t273
Label_t268:
Label_t273:
move $t0, $s0
li $t1, 2
mulo $t2, $t1, 5
addi $t3, $t2, 1
subi $t4, $t3, 1
sw $t0, 304($fp)
sw $t1, 308($fp)
sw $t2, 312($fp)
sw $t3, 316($fp)
sw $t4, 320($fp)
beq $t0, $t4, Label_t274
la $a0, t285
li $v0, 4
syscall
move $a0, $s0
li $v0, 1
syscall
lw $ra, 532($sp)
addi $sp, $sp, 536
jr $ra
j Label_t286
Label_t274:
Label_t286:
move $t0, $s3
li $t1, 50
sw $t0, 324($fp)
bne $t1, $t0, Label_t290
move $t0, $s0
addi $t1, $t0, 6
div $t2, $t1, 2
subi $sp, $sp, 4
sw $t2, 0($sp)
sw $t0, 328($fp)
sw $t1, 332($fp)
sw $t2, 336($fp)
jal fact
sw $v0, 340($fp)
lw $s2, 340($fp)
la $a0, t297
li $v0, 4
syscall
move $a0, $s2
li $v0, 1
syscall
j Label_t287
Label_t290:
li $t0, 51
lw $t1, 324($fp)
sw $t1, 324($fp)
bne $t0, $t1, Label_t299
li $t0, 1000
move $t1, $s0
div $t2, $t0, $t1
move $t3, $s3
add $t4, $t2, $t3
subi $t5, $t4, 48
subi $sp, $sp, 4
sw $t5, 0($sp)
sw $t0, 344($fp)
sw $t1, 348($fp)
sw $t2, 352($fp)
sw $t3, 356($fp)
sw $t4, 360($fp)
sw $t5, 364($fp)
jal reduce
sw $v0, 368($fp)
lw $s1, 368($fp)
la $a0, t308
li $v0, 4
syscall
move $a0, $s1
li $v0, 1
syscall
j Label_t287
Label_t299:
li $t0, 52
lw $t1, 324($fp)
sw $t1, 324($fp)
bne $t0, $t1, Label_t310
li $t0, 1000
li $t1, 81
add $t2, $t0, $t1
li $t3, -15
mulo $t4, $t3, 5
add $t5, $t2, $t4
li $t6, 0
sub $t7, $t5, $t6
li $t8, 115
add $t9, $t7, $t8
sw $t0, 372($fp)
subi $t0, $t9, 115
sw $t1, 376($fp)
subi $t1, $t0, 6
sw $t2, 380($fp)
div $t2, $t1, 10
sw $t3, 384($fp)
li $t3, 49
sw $t4, 388($fp)
add $t4, $t2, $t3
sw $t5, 392($fp)
subi $t5, $t4, 49
move $s2, $t5
la $a0, t332
li $v0, 4
syscall
sw $t6, 396($fp)
move $t6, $s2
subi $sp, $sp, 4
sw $t6, 0($sp)
subi $sp, $sp, 4
sw $t7, 400($fp)
li $t7, 50
sw $t7, 0($sp)
sw $t0, 412($fp)
sw $t1, 416($fp)
sw $t2, 420($fp)
sw $t3, 424($fp)
sw $t4, 428($fp)
sw $t5, 432($fp)
sw $t6, 436($fp)
sw $t8, 404($fp)
sw $t9, 408($fp)
jal reduce_to_m
sw $v0, 440($fp)
lw $a0, 440($fp)
li $v0, 1
syscall
j Label_t287
Label_t310:
li $t0, 53
lw $t1, 324($fp)
sw $t1, 324($fp)
bne $t0, $t1, Label_t337
li $s3, 65
Label_t339:
move $t0, $s3
subi $t1, $t0, 65
move $t2, $s3
subi $sp, $sp, 4
sw $t2, 0($sp)
sw $t0, 444($fp)
sw $t1, 448($fp)
sw $t2, 452($fp)
jal Lower
sw $v0, 456($fp)
lw $t0, 456($fp)
lw $t1, 448($fp)
sll $at, $t1, 2
add $at, $at, $fp
sw $t0, 188($at)
move $t2, $s3
subi $t3, $t2, 65
sll $at, $t3, 2
add $at, $at, $fp
lw $t4, 188($at)
move $a0, $t4
li $v0, 11
syscall
addi $s3, $s3, 1
move $t5, $s3
subi $t6, $t5, 90
sw $t0, 456($fp)
sw $t1, 448($fp)
sw $t2, 460($fp)
sw $t3, 464($fp)
sw $t4, 468($fp)
sw $t5, 472($fp)
sw $t6, 476($fp)
blez $t6, Label_t339
addi $a1, $fp, 188
addi $a2, $a1, 104
Array_Print_s5:
lw $a0, 0($a1)
li $v0, 11
syscall
addi $a1, $a1, 4
bne $a1, $a2, Array_Print_s5
j Label_t287
Label_t337:
li $t0, 54
lw $t1, 324($fp)
sw $t1, 324($fp)
bne $t0, $t1, Label_t354
jal testDeclare
la $a0, t355
li $v0, 4
syscall
jal testForAndArray
j Label_t287
Label_t354:
li $t0, 55
lw $t1, 324($fp)
sw $t1, 324($fp)
bne $t0, $t1, Label_t357
jal testExpression
sw $v0, 480($fp)
lw $s2, 480($fp)
la $a0, t359
li $v0, 4
syscall
move $a0, $s2
li $v0, 1
syscall
j Label_t287
Label_t357:
la $a0, t360
li $v0, 4
syscall
move $t0, $s0
subi $sp, $sp, 4
sw $t0, 0($sp)
li $t1, 81
subi $sp, $sp, 4
sw $t1, 0($sp)
li $t2, -15
subi $sp, $sp, 4
sw $t2, 0($sp)
li $t3, 13
subi $sp, $sp, 4
sw $t3, 0($sp)
sw $t0, 484($fp)
sw $t1, 488($fp)
sw $t2, 492($fp)
sw $t3, 496($fp)
jal sq_dist
sw $v0, 500($fp)
lw $a0, 500($fp)
li $v0, 1
syscall
Label_t287:
lw $ra, 532($sp)
addi $sp, $sp, 536
jr $ra
lw $ra, 532($sp)
addi $sp, $sp, 536
jr $ra
