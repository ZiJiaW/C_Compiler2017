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
sw $t0, 36($fp)
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
sw $t1, 36($fp)
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
li $s1, 3
li $t0, 3
neg $t1, $t0
li $s0, 2
mulo $t2, $t1, 2
mulo $t3, $t2, $t1
addi $t4, $t3, 2
move $a0, $t4
li $v0, 1
syscall
add $t4, $s0, $s1
add $t5, $s0, $s1
subi $sp, $sp, 4
li $t6, 2
sw $t6, 0($sp)
subi $sp, $sp, 4
li $t6, 3
sw $t6, 0($sp)
sw $t1, 32($fp)
sw $t2, 24($fp)
sw $t3, 36($fp)
sw $t4, 0($gp)
sw $t5, 68($fp)
jal cal
sw $v0, 72($fp)
lw $t0, 68($fp)
lw $t1, 72($fp)
add $t2, $t0, $t1
la $a0, t35
li $v0, 4
syscall
move $a0, $t2
li $v0, 1
syscall
li $v0, 12
syscall
move $s2, $v0
move $t3, $s2
li $t4, 97
sw $t0, 68($fp)
sw $t1, 72($fp)
sw $t2, 0($gp)
sw $t3, 80($fp)
bne $t4, $t3, Label_t39
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
