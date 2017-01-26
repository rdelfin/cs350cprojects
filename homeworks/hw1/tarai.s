.section .text
.global tarai
.type tarai @function

tarai:
    push %rbp
    mov %rsp, %rbp

    # Callee-saved registers
    push %r12
    push %r13

    # Arguments:
    # x) %edi
    # y) %esi
    # z) %edx 

    mov %rdi, %r9    # Copy x to r9
    mov %rsi, %r10   # Copy y to r10
    mov %rdx, %r11   # Copy z to r11

    # Tarai calculation

    # tarai(x-1, y, z)
    # TODO: Change this to copy from r9-11 to rdi, rsi and rdx instead of moving
    mov %rdi, %r12   # copy of x into %r12
    add $-1, %edi    # x = x - 1
    push %rdi        # Caller-saved register
    push %rsi        # Caller-saved register
    push %rdx        # Caller-saved register

    call tarai       # Call recursively
    mov %rax, %r13   # Result of tarai(x-1, y, z)

    pop %rdx
    pop %rsi
    pop %rdi
    mov %r12, %rdi   # Restore %edi (x)


    # tarai(y-1, z, x)
    mov %rsi, %r14  # Move y -> r13
    mov %rdx, %rsi  # Move z -> y
    mov %rdi, %rdx  # Move x -> z
    mov %r13, %rdi  # Move y -> x
    add $-1, %edi   # Add to y
    push %rdi       # Caller-saved register
    push %rsi       # Caller-saved register
    push %rdx       # Caller-saved register
    
    call tarai      # Recursive call
    mov %rax, %r14  # Result of tarai(y-1, z, x) 

    pop %rdx
    pop %rsi
    pop %rdi
    

    # End of function
    mov %rsp, %rbp
    pop %rbp
    ret

