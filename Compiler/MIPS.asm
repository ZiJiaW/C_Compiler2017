.data
t74: .asciiz "The number is too large!"
t118: .asciiz "Please enter the number and arrangement of ways, u said ascending order, d said descending order!"
t133: .asciiz "Input does not match the syntax, exit!"
t134: .asciiz "Input does not match the syntax, exit!"
t138: .asciiz "Whether or not to sum and mul these number, 0 or 1:"
t144: .asciiz "The program is over!"
t146: .asciiz "Thanks for trying, Bye!"
t65: .asciiz " "
t57: .asciiz " "
t103: .asciiz " "
.text
jal main
li $v0, 10
syscall
recor_printU:
subi $sp, $sp, 160
sw $ra, 156($sp)
sw $fp, 152($sp)
move $fp, $sp
sw $s0, 148($fp)
sw $s1, 144($fp)
li $t0, 99
li $t1, 99
add $t2, $t0, $t1
move $t3, $t2
li $t4, 6
li $t5, 3
li $t6, 7
addi $t7, $t6, 5
li $t8, 56
mulo $t9, $t8, 9
sw $t0, 8($fp)
subi $t0, $t9, 23
sw $t1, 12($fp)
mulo $t1, $t0, 6
sw $t2, 16($fp)
add $t2, $t7, $t1
sw $t3, 164($gp)
mulo $t3, $t2, 34
sw $t4, 20($fp)
add $t4, $t5, $t3
sw $t5, 24($fp)
subi $t5, $t4, 21
sw $t6, 28($fp)
mulo $t6, $t5, 4
sw $t7, 32($fp)
mulo $t7, $t6, 14
sw $t8, 36($fp)
lw $t8, 20($fp)
sw $t9, 40($fp)
sub $t9, $t8, $t7
move $s1, $t9
sw $t0, 44($fp)
li $t0, -12
sw $t1, 48($fp)
neg $t1, $t0
sw $t2, 52($fp)
li $t2, 2
sw $t3, 56($fp)
sub $t3, $t1, $t2
sw $t4, 60($fp)
move $t4, $t3
sw $t5, 64($fp)
li $t5, -2
sw $t6, 68($fp)
neg $t6, $t5
sw $t7, 72($fp)
li $t7, -3
sw $t8, 20($fp)
sub $t8, $t6, $t7
sw $t9, 76($fp)
move $t9, $t8
li $s0, 0
sw $t0, 80($fp)
sw $t1, 84($fp)
sw $t2, 88($fp)
sw $t3, 92($fp)
sw $t4, 568($gp)
sw $t5, 96($fp)
sw $t6, 100($fp)
sw $t7, 104($fp)
sw $t8, 108($fp)
sw $t9, 572($gp)
Label_t51:
move $t0, $s0
move $t1, $s0
sll $at, $t0, 2
add $at, $at, $gp
sw $t1, 168($at)
move $t2, $s0
sll $at, $t2, 2
add $at, $at, $gp
lw $t3, 168($at)
move $t4, $t3
la $a0, t57
li $v0, 4
syscall
move $t5, $s0
sll $at, $t5, 2
add $at, $at, $gp
lw $t6, 168($at)
move $a0, $t6
li $v0, 1
syscall
addi $s0, $s0, 1
move $t7, $s0
lw $t8, 160($fp)
sw $t0, 112($fp)
sw $t1, 116($fp)
sw $t2, 120($fp)
sw $t3, 124($fp)
sw $t4, 568($gp)
sw $t5, 128($fp)
sw $t6, 132($fp)
sw $t7, 136($fp)
sw $t8, 140($fp)
bne $t7, $t8, Label_t51
lw $ra, 156($sp)
lw $fp, 152($sp)
lw $s0, 148($sp)
lw $s1, 144($sp)
addi $sp, $sp, 164
jr $ra
recor_printD:
subi $sp, $sp, 20
sw $ra, 16($sp)
sw $fp, 12($sp)
move $fp, $sp
lw $t0, 20($fp)
li $t1, 0
sw $t0, 0($fp)
beq $t0, $t1, Label_t62
la $a0, t65
li $v0, 4
syscall
lw $a0, 20($fp)
li $v0, 1
syscall
lw $t0, 20($fp)
subi $t1, $t0, 1
subi $sp, $sp, 4
sw $t1, 0($sp)
sw $t0, 4($fp)
sw $t1, 8($fp)
jal recor_printD
j Label_t69
Label_t62:
Label_t69:
lw $ra, 16($sp)
lw $fp, 12($sp)
addi $sp, $sp, 24
jr $ra
Isright:
subi $sp, $sp, 32
sw $ra, 28($sp)
sw $fp, 24($sp)
move $fp, $sp
lw $t0, 36($fp)
subi $t1, $t0, 100
sw $t0, 0($fp)
sw $t1, 4($fp)
bltz $t1, Label_t70
la $a0, t74
li $v0, 4
syscall
li $v0, 0
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 40
jr $ra
j Label_t76
Label_t70:
Label_t76:
lw $t0, 36($fp)
subi $t1, $t0, 0
sw $t0, 8($fp)
sw $t1, 12($fp)
blez $t1, Label_t77
lw $t0, 32($fp)
li $t1, 117
sw $t0, 16($fp)
bne $t0, $t1, Label_t81
li $v0, 1
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 40
jr $ra
j Label_t85
Label_t81:
Label_t85:
lw $t0, 32($fp)
li $t1, 100
sw $t0, 20($fp)
bne $t0, $t1, Label_t86
li $v0, 2
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 40
jr $ra
j Label_t90
Label_t86:
Label_t90:
j Label_t91
Label_t77:
Label_t91:
li $v0, 0
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 40
jr $ra
lw $ra, 28($sp)
lw $fp, 24($sp)
addi $sp, $sp, 40
jr $ra
sumandmul:
subi $sp, $sp, 84
sw $ra, 80($sp)
sw $fp, 76($sp)
move $fp, $sp
sw $s0, 72($fp)
sw $s1, 68($fp)
sw $s2, 64($fp)
li $s2, 0
li $s1, 1
li $s0, 0
Label_t96:
move $t0, $s2
move $t1, $s0
add $t2, $t0, $t1
move $s2, $t2
addi $s0, $s0, 1
move $t3, $s0
lw $t4, 84($fp)
sw $t0, 12($fp)
sw $t1, 16($fp)
sw $t2, 20($fp)
sw $t3, 24($fp)
sw $t4, 28($fp)
bne $t3, $t4, Label_t96
move $a0, $s2
li $v0, 1
syscall
la $a0, t103
li $v0, 4
syscall
li $s0, 0
Label_t105:
move $t0, $s0
sll $at, $t0, 2
add $at, $at, $gp
lw $t1, 168($at)
li $t2, 0
sw $t0, 32($fp)
sw $t1, 36($fp)
beq $t1, $t2, Label_t107
move $t0, $s1
move $t1, $s0
sll $at, $t1, 2
add $at, $at, $gp
lw $t2, 168($at)
mulo $t3, $t0, $t2
move $s1, $t3
sw $t0, 40($fp)
sw $t1, 44($fp)
sw $t2, 48($fp)
sw $t3, 52($fp)
j Label_t115
Label_t107:
Label_t115:
addi $s0, $s0, 1
move $t0, $s0
lw $t1, 84($fp)
sw $t0, 56($fp)
sw $t1, 60($fp)
bne $t0, $t1, Label_t105
move $a0, $s1
li $v0, 1
syscall
lw $ra, 80($sp)
lw $fp, 76($sp)
lw $s0, 72($sp)
lw $s1, 68($sp)
lw $s2, 64($sp)
addi $sp, $sp, 88
jr $ra
main:
subi $sp, $sp, 64
sw $ra, 60($sp)
move $fp, $sp
la $a0, t118
li $v0, 4
syscall
li $v0, 5
syscall
move $s0, $v0
li $v0, 12
syscall
move $s2, $v0
move $t0, $s0
subi $sp, $sp, 4
sw $t0, 0($sp)
move $t1, $s2
subi $sp, $sp, 4
sw $t1, 0($sp)
sw $t0, 12($fp)
sw $t1, 16($fp)
jal Isright
sw $v0, 20($fp)
lw $s1, 20($fp)
move $t0, $s1
li $t1, 1
sw $t0, 24($fp)
bne $t1, $t0, Label_t125
move $t0, $s0
subi $sp, $sp, 4
sw $t0, 0($sp)
sw $t0, 28($fp)
jal recor_printU
j Label_t122
Label_t125:
li $t0, 2
lw $t1, 24($fp)
sw $t1, 24($fp)
bne $t0, $t1, Label_t128
li $t0, 0
move $t1, $s0
subi $sp, $sp, 4
sw $t1, 0($sp)
sw $t0, 164($gp)
sw $t1, 32($fp)
jal recor_printD
j Label_t122
Label_t128:
li $t0, 0
lw $t1, 24($fp)
sw $t1, 24($fp)
bne $t0, $t1, Label_t132
la $a0, t133
li $v0, 4
syscall
j Label_t122
Label_t132:
la $a0, t134
li $v0, 4
syscall
Label_t122:
move $t0, $s1
li $t1, 0
sw $t0, 36($fp)
beq $t0, $t1, Label_t135
la $a0, t138
li $v0, 4
syscall
li $v0, 5
syscall
move $s1, $v0
move $t0, $s1
li $t1, 1
sw $t0, 40($fp)
bne $t0, $t1, Label_t139
move $t0, $s0
subi $sp, $sp, 4
sw $t0, 0($sp)
sw $t0, 44($fp)
jal sumandmul
j Label_t143
Label_t139:
la $a0, t144
li $v0, 4
syscall
Label_t143:
j Label_t145
Label_t135:
Label_t145:
la $a0, t146
li $v0, 4
syscall
lw $ra, 60($sp)
addi $sp, $sp, 64
jr $ra
