.data
arg1_max: .word 12
arg1_min: .word -12
arg2_max: .word 7
arg2_min: .word -7
space: .asciiz " "
newline: .asciiz "\n"
error: .asciiz " Error!"

.text
.globl main

main:
    la $t0, arg1_max
    la $t1, arg1_min
    la $t2, arg2_max
    la $t3, arg2_min

    lw $t4, 0($t0)
    lw $t5, 0($t1)
    lw $t6, 0($t2)
    lw $t7, 0($t3)

    move $s4, $t5  # Initialize $s4 to arg1_min

outer_loop:

    bgt $s4, $t4, exit_program

    move $s5, $t7  # Initialize $s5 to arg2_min

inner_loop:

    bgt $s5, $t6, next_outer

    move $a0, $s4
    move $a1, $s5


    jal test_mul
    move $s2, $v0  # $s2 now contains the product

    mul $s3, $s4, $s5

    li $v0, 1
    move $a0, $s4  # s4 contains first number
    syscall
    li $v0, 4
    la $a0, space
    syscall
    li $v0, 1
    move $a0, $s5  # s5 contains second number
    syscall
    li $v0, 4
    la $a0, space
    syscall
    li $v0, 1
    move $a0, $s3  # s3 contains expected product
    syscall
    li $v0, 4
    la $a0, space
    syscall
    li $v0, 1
    move $a0, $s2  # s2 contains actual product from test_mul
    syscall
    li $v0, 4
    la $a0, newline
    syscall


    addi $s5, $s5, 1  # increment arg2
    j inner_loop

next_outer:
    addi $s4, $s4, 1  # increment arg1
    j outer_loop

print_error:
    li $v0, 4
    la $a0, error
    syscall
    j inner_loop

exit_program:
    lw $ra, 0($sp)  # Restore $ra
    addi $sp, $sp, 4

    li $v0, 10
    syscall

test_mul:
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    li $t8, 0  # Initialize accumulator
    
    # Check if either operand is zero
    beq $a0, $zero, mul_done
    beq $a1, $zero, mul_done

    # Determine the sign of the result
    li $t9, 1
    bltz $a0, change_sign_a0
    j check_a1

change_sign_a0:
    negu $a0, $a0
    li $t9, -1

check_a1:
    bltz $a1, change_sign_a1
    j mul_loop

change_sign_a1:
    negu $a1, $a1
    mul $t9, $t9, -1

mul_loop:
    add $t8, $t8, $a0  # Add $a0 to accumulator
    addi $a1, $a1, -1   # Decrement $a1
    bgtz $a1, mul_loop  # Continue looping if $a1 > 0

mul_done:
    mul $v0, $t8, $t9  # Apply the sign to the result

    lw $ra, 0($sp)
    addi $sp, $sp, 8
    jr $ra


