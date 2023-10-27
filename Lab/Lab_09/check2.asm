.data
    newline: .asciiz "\n"
    matrix:       .word   6, 3, 2, 2
   
.text
.globl main

main:
    sub $sp, $sp, 4   # Allocate stack space to store $ra (return address pointer)
    sw  $ra, 0($sp)   # Store $ra onto the stack ($sp is the stack pointer) taken from lecture notes
    
    la $a0, matrix
    jal det
    
    # print outcome
    move $a0, $v0  
    li $v0, 1      
    syscall

    # print new line
    li $v0, 4      
    la $a0, newline
    syscall

    # restore stack stuff
    lw  $ra, 0($sp) # Load $ra from the stack
    add $sp, $sp, 4  # Restore the stack pointer

    # exit
    li $v0, 10     # Exit program code
    syscall
   
mul:

    move $t0, $a2

    loop:
        beqz $t0, done
        add $v0, $v0, $a1
        subi $t0, $t0, 1
        j loop
    
    done:
    jr $ra

det:
    move $t9, $ra
    
    lw $a1, 0($a0)    # a
    lw $a2, 12($a0)   # d
    
    jal mul # multiply a d
    move $t1, $v0 # move outcome to $t1


    lw $a1, 4($a0)    # b
    lw $a2, 8($a0)    # c
   
    li $v0, 0
   
    jal mul #multiply b c
    move $t2, $v0 # move outcome to t2
    
    sub $v0, $t1, $t2 #subtract ad-cb . problem HAS to be here
    
    move $ra, $t9
    jr $ra
    