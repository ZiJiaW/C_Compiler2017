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
subi $sp, $sp, 180
sw $ra, 176($sp)
sw $fp, 172($sp)
move $fp, $sp
sw $s0, 168($fp)
sw $s1, 164($fp)
sw $s2, 160($fp)
sw $s3, 156($fp)
sw $s4, 152($fp)
li $s4, 52
li $s1, 99
li $s0, 65
li $s3, 454
li $t0, 12
li $t1, 0
sll $at, $t1, 2
add $at, $at, $fp
sw $t0, 0($at)
li $t2, 2
li $t3, 1
sll $at, $t3, 2
add $at, $at, $fp
sw $t2, 0($at)
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
li $t5, 0
sll $at, $t5, 2
add $at, $at, $fp
lw $t4, 0($at)
move $a0, $t4
li $v0, 1
syscall
la $a0, t26
li $v0, 4
syscall
li $t7, 1
sll $at, $t7, 2
add $at, $at, $fp
lw $t6, 0($at)
move $a0, $t6
li $v0, 1
syscall
lw $ra, 176($sp)
sw $t4, 140($fp)
sw $t6, 144($fp)
lw $fp, 172($sp)
lw $s0, 168($sp)
lw $s1, 164($sp)
lw $s2, 160($sp)
lw $s3, 156($sp)
lw $s4, 152($sp)
addi $sp, $sp, 180
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
li $s2, 97
Label_t30:
subi $t0, $s2, 97
sll $at, $t0, 2
add $at, $at, $gp
sw $s2, 1548($at)
subi $t1, $s2, 97
sll $at, $t1, 2
add $at, $at, $gp
lw $t2, 1548($at)
move $a0, $t2
li $v0, 11
syscall
addi $s2, $s2, 1
subi $t3, $s2, 122
sw $t0, 28($fp)
sw $t1, 44($fp)
sw $t2, 48($fp)
sw $t3, 56($fp)
blez $t3, Label_t30
li $s1, 100
li $s3, 0
li $s0, 50
Label_t46:
move $t0, $s3
move $t1, $s1
move $t2, $s0
add $s3, $t0, $s1
sw $t0, 148($fp)
sw $t1, 72($fp)
sw $t2, 76($fp)
bne $t1, $t2, Label_t51
li $s0, 20
j Label_t55
Label_t51:
subi $s1, $s1, 1
Label_t55:
subi $s1, $s1, 1
sub $t0, $s1, $s0
sw $t0, 96($fp)
bgez $t0, Label_t46
la $a0, t62
li $v0, 4
syscall
move $a0, $s3
li $v0, 1
syscall
li $s1, 0
Label_t64:
subi $t0, $s1, 0
sw $t0, 104($fp)
bgez $t0, Label_t66
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
subi $t0, $s1, 0
sw $t0, 112($fp)
bltz $t0, Label_t64
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
sw $t0, 24($fp)
sw $t1, 20($fp)
sw $t2, 8($fp)
lw $fp, 12($sp)
addi $sp, $sp, 28
jr $ra
lw $ra, 16($sp)
lw $fp, 12($sp)
addi $sp, $sp, 28
jr $ra
testExpression:
subi $sp, $sp, 360
sw $ra, 356($sp)
sw $fp, 352($sp)
move $fp, $sp
sw $s0, 348($fp)
sw $s1, 344($fp)
sw $s2, 340($fp)
li $t0, 2
neg $t1, $t0
li $t2, 480
neg $t3, $t2
li $s2, 0
li $t5, 9
sll $at, $t5, 2
add $at, $at, $gp
lw $t4, 1548($at)
subi $t6, $t4, 97
mulo $t7, $t6, 2
add $t8, $t1, $t7
add $s0, $t3, $t8
move $a0, $s0
li $v0, 1
syscall
li $t9, 480
neg $t0, $t9
sw $t1, 56($fp)
li $t1, 2
neg $t2, $t1
sw $t3, 48($fp)
sw $t4, 60($fp)
li $t4, 9
sll $at, $t4, 2
add $at, $at, $gp
lw $t3, 1548($at)
subi $t5, $t3, 97
sw $t6, 64($fp)
mulo $t6, $t5, 2
sw $t7, 68($fp)
add $t7, $t2, $t6
sw $t8, 72($fp)
add $t8, $t0, $t7
subi $t9, $t8, 18
sw $t0, 116($fp)
sw $t2, 124($fp)
sw $t3, 128($fp)
sw $t5, 132($fp)
sw $t6, 136($fp)
sw $t7, 140($fp)
sw $t8, 144($fp)
sw $t9, 156($fp)
bgtz $t9, Label_t105
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
li $t2, 18
addi $t3, $t1, 0
subi $sp, $sp, 4
sw $t2, 0($sp)
li $t4, 3
neg $t5, $t4
addi $t6, $t5, 0
subi $sp, $sp, 4
sw $t6, 0($sp)
sw $t1, 160($fp)
sw $t2, 172($fp)
sw $t3, 164($fp)
sw $t5, 188($fp)
sw $t6, 192($fp)
jal sum
sw $v0, 196($fp)
lw $t0, 164($fp)
lw $t1, 196($fp)
add $s1, $t0, $t1
sw $t0, 164($fp)
sw $t1, 196($fp)
Label_t156:
move $t0, $s2
move $t1, $s1
addi $s1, $t1, 1
addi $t2, $t1, 1
add $s2, $t0, $t1
li $t4, 9
sll $at, $t4, 2
add $at, $at, $gp
lw $t3, 1548($at)
subi $t5, $t3, 97
mulo $t6, $t5, 2
addi $t7, $t6, 15
subi $sp, $sp, 4
sw $t7, 0($sp)
li $t8, 2
neg $t9, $t8
sw $t0, 332($fp)
subi $t0, $t9, 8
subi $sp, $sp, 4
sw $t0, 0($sp)
sw $t0, 252($fp)
sw $t1, 336($fp)
sw $t2, 216($fp)
sw $t3, 220($fp)
sw $t5, 224($fp)
sw $t6, 228($fp)
sw $t7, 232($fp)
sw $t9, 244($fp)
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
li $t0, 480
neg $t1, $t0
li $t2, 2
neg $t3, $t2
li $t5, 9
sll $at, $t5, 2
add $at, $at, $gp
lw $t4, 1548($at)
subi $t6, $t4, 97
mulo $t7, $t6, 2
add $t8, $t3, $t7
add $t9, $t1, $t8
move $v0, $t9
lw $ra, 356($sp)
sw $t1, 300($fp)
sw $t3, 308($fp)
sw $t4, 312($fp)
sw $t6, 316($fp)
sw $t7, 320($fp)
sw $t8, 324($fp)
sw $t9, 328($fp)
lw $fp, 352($sp)
lw $s0, 348($sp)
lw $s1, 344($sp)
lw $s2, 340($sp)
addi $sp, $sp, 360
jr $ra
lw $ra, 356($sp)
lw $fp, 352($sp)
lw $s0, 348($sp)
lw $s1, 344($sp)
lw $s2, 340($sp)
addi $sp, $sp, 360
jr $ra
sq_dist:
subi $sp, $sp, 96
sw $ra, 92($sp)
sw $fp, 88($sp)
move $fp, $sp
sw $s0, 84($fp)
lw $t0, 96($fp)
lw $t1, 100($fp)
sub $t2, $t0, $t1
lw $t3, 104($fp)
lw $t4, 108($fp)
sub $t5, $t3, $t4
mulo $t6, $t2, $t2
mulo $t7, $t5, $t5
add $s0, $t7, $t6
addi $t8, $s0, 1
mulo $t9, $t8, 2
sw $t0, 96($fp)
div $t0, $t9, 2
sw $t1, 100($fp)
subi $t1, $t0, 1
move $v0, $t1
lw $ra, 92($sp)
sw $t0, 76($fp)
sw $t1, 80($fp)
sw $t2, 52($fp)
sw $t3, 104($fp)
sw $t4, 108($fp)
sw $t5, 24($fp)
sw $t6, 56($fp)
sw $t7, 28($fp)
sw $t8, 68($fp)
sw $t9, 72($fp)
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
sw $t1, 32($fp)
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
sw $t1, 32($fp)
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
lw $t0, 48($fp)
add $s0, $s0, $t0
subi $t0, $t0, 1
lw $t1, 44($fp)
sub $t2, $t0, $t1
sw $t0, 48($fp)
sw $t1, 44($fp)
sw $t2, 24($fp)
bgez $t2, Label_t256
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
sw $t0, 16($fp)
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
subi $t0, $s0, 0
sw $t0, 300($fp)
bgez $t0, Label_t268
la $a0, t272
li $v0, 4
syscall
lw $ra, 532($sp)
addi $sp, $sp, 536
jr $ra
j Label_t273
Label_t268:
Label_t273:
li $t0, 10
move $t1, $s0
sw $t0, 320($fp)
sw $t1, 304($fp)
beq $t1, $t0, Label_t274
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
addi $t0, $s0, 6
div $t1, $t0, 2
subi $sp, $sp, 4
sw $t1, 0($sp)
sw $t0, 332($fp)
sw $t1, 336($fp)
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
div $t1, $t0, $s0
add $t2, $t1, $s3
subi $t3, $t2, 48
subi $sp, $sp, 4
sw $t3, 0($sp)
sw $t1, 352($fp)
sw $t2, 360($fp)
sw $t3, 364($fp)
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
li $s2, 100
la $a0, t332
li $v0, 4
syscall
move $t0, $s2
subi $sp, $sp, 4
sw $t0, 0($sp)
subi $sp, $sp, 4
li $t1, 50
sw $t1, 0($sp)
sw $t0, 436($fp)
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
subi $t1, $s3, 65
subi $sp, $sp, 4
sw $t0, 0($sp)
sw $t0, 452($fp)
sw $t1, 448($fp)
jal Lower
sw $v0, 456($fp)
lw $t0, 456($fp)
lw $t1, 448($fp)
sll $at, $t1, 2
add $at, $at, $fp
sw $t0, 188($at)
subi $t2, $s3, 65
sll $at, $t2, 2
add $at, $at, $fp
lw $t3, 188($at)
move $a0, $t3
li $v0, 11
syscall
addi $s3, $s3, 1
subi $t4, $s3, 90
sw $t0, 456($fp)
sw $t1, 448($fp)
sw $t2, 464($fp)
sw $t3, 468($fp)
sw $t4, 476($fp)
blez $t4, Label_t339
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
