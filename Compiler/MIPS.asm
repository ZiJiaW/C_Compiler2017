.data
t35: .asciiz " "
t40: .asciiz "you input a!"
t43: .asciiz "you input b!"
t44: .asciiz "not a or b!"
.text
jal main
li $v0, 10
syscall
cal:
subi $sp, $sp, 36
sw $ra, 32($sp)
sw $fp, 28($sp)
move $fp, $sp
lw $t0, 36($fp)
subi $t1, $t0, 0
sw $t0, 0($fp)
sw $t1, 4($fp)
bgtz $t1, Label_t0
li $v0, 0
lw $ra, 32($sp)
lw $fp, 28($sp)
addi $sp, $sp, 44
jr $ra
j Label_t5
Label_t0:
lw $t0, 40($fp)
subi $sp, $sp, 4
sw $t0, 0($sp)
lw $t1, 36($fp)
subi $t2, $t1, 1
subi $sp, $sp, 4
sw $t2, 0($sp)
sw $t0, 8($fp)
sw $t1, 12($fp)
sw $t2, 16($fp)
jal cal
sw $v0, 20($fp)
lw $t0, 20($fp)
addi $t1, $t0, 1
move $v0, $t1
lw $ra, 32($sp)
sw $t0, 20($fp)
sw $t1, 24($fp)
lw $fp, 28($sp)
addi $sp, $sp, 44
jr $ra
Label_t5:
lw $ra, 32($sp)
lw $fp, 28($sp)
addi $sp, $sp, 44
jr $ra
main:
subi $sp, $sp, 100
sw $ra, 96($sp)
move $fp, $sp
li $t0, 1
li $s0, 2
li $s1, 3
move $t1, $s1
neg $t2, $t1
move $t3, $s0
mulo $t4, $t2, $t3
move $t5, $s1
neg $t6, $t5
mulo $t7, $t4, $t6
move $t8, $s0
add $t9, $t7, $t8
move $t0, $t9
move $a0, $t0
li $v0, 1
syscall
sw $t0, 0($gp)
move $t0, $s0
sw $t1, 12($fp)
move $t1, $s1
sw $t2, 16($fp)
add $t2, $t0, $t1
sw $t3, 20($fp)
move $t3, $t2
sw $t4, 24($fp)
move $t4, $s0
sw $t5, 28($fp)
move $t5, $s1
sw $t6, 32($fp)
add $t6, $t4, $t5
subi $sp, $sp, 4
sw $t7, 36($fp)
li $t7, 2
sw $t7, 0($sp)
subi $sp, $sp, 4
li $t7, 3
sw $t7, 0($sp)
sw $t0, 48($fp)
sw $t1, 52($fp)
sw $t2, 56($fp)
sw $t3, 0($gp)
sw $t4, 60($fp)
sw $t5, 64($fp)
sw $t6, 68($fp)
sw $t8, 40($fp)
sw $t9, 44($fp)
jal cal
sw $v0, 72($fp)
lw $t0, 68($fp)
lw $t1, 72($fp)
add $t2, $t0, $t1
move $t3, $t2
la $a0, t35
li $v0, 4
syscall
move $a0, $t3
li $v0, 1
syscall
li $v0, 12
syscall
move $s2, $v0
move $t4, $s2
li $t5, 97
sw $t0, 68($fp)
sw $t1, 72($fp)
sw $t2, 76($fp)
sw $t3, 0($gp)
sw $t4, 80($fp)
bne $t5, $t4, Label_t39
la $a0, t40
li $v0, 4
syscall
j Label_t36
Label_t39:
li $t0, 98
lw $t1, 80($fp)
sw $t1, 80($fp)
bne $t0, $t1, Label_t42
la $a0, t43
li $v0, 4
syscall
j Label_t36
Label_t42:
la $a0, t44
li $v0, 4
syscall
Label_t36:
lw $ra, 96($sp)
addi $sp, $sp, 100
jr $ra
lw $ra, 96($sp)
addi $sp, $sp, 100
jr $ra
