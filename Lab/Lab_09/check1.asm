.data
    newline: .asciiz "\n"
   
.text
.globl main

main:
    sub $sp, $sp, 4   # Allocate stack space to store $ra (return address pointer)
    sw  $ra, 0($sp)   # Store $ra onto the stack ($sp is the stack pointer)
    
    li $a0, -3
    li $a1, 5
    jal mul
    
    #print outcome
    move $a0, $v0  
    li $v0, 1      
    syscall

    #print new line
    li $v0, 4      
    la $a0, newline
    syscall

    # restore stuff
    lw  $ra, 0($sp) # Load $ra from the stack
    add $sp, $sp, 4  # Restore the stack pointer
    
    # restore $ra, free stack space, and return
    #lw  $ra, 0($sp)
    #add $sp, $sp, 4
    #jr  $ra

    #exit
    li $v0, 10     # Exit program code
    syscall
    

   

mul:
    
    move $t0, $a1 #move to temp variable
    

    bgez, $a1, loop #negate the second value to run the loop
    sub $a1, $zero, $a1

    loop:
        beqz $a1, negate
        add $v0, $v0, $a0
        subi $a1, $a1, 1
        j loop
    
    negate: #if the value was negative, negate the result and return
        bgez, $t0, done
        sub $v0, $zero, $v0
        sub $t0, $zero, $t0
    
    done:
    jr $ra
