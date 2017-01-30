.section .text
.global tarai
.type tarai @function

tarai:
    push %rbp
    mov %rsp, %rbp

    mov %rsi, %rax    # Move y into RAX for return value in case of jump
    cmp %rdi, %rsi    # Check x, y
    jge taraiend      # if y >= x, jump

    # Callee-saved registers (RBP, RBX, R12-15)
    push %r12
    push %r13
    push %r14

    # Arguments:
    # x) %edi
    # y) %esi
    # z) %edx

    # Tarai calculation

    # tarai(x-1, y, z)
    push %rdi       # Store x
    push %rsi       # Store y
    push %rdx       # Store z

    add $-1, %edi   # x-1
    call tarai      # Recursive call

    pop %rdx          # Restore z
    pop %rsi          # Restore y
    pop %rdi          # Restore x
    mov %rax, %r12    # r12 = result 1


    # tarai(y-1, z, x)
    xchg %rsi, %rdi   # (x, y, z) -> (y, x, z)
    xchg %rdx, %rsi   # (y, x, z) -> (y, z, x)
    push %rdi         # Store x (actually y)
    push %rsi         # Store y (actually z)
    push %rdx         # Store z (actually x)

    add $-1, %edi     # y-1
    call tarai        # Recursive call

    pop %rdx          # Restore z (actually x)
    pop %rsi          # Restore y (actually z)
    pop %rdi          # Restore x (actually y)
    mov %rax, %r13    # r13 = result 2


    # tarai(z-1, x, y)
    xchg %rsi, %rdi   # (y, z, x) -> (z, y, x)
    xchg %rdx, %rsi   # (z, y, x) -> (z, x, y)
    push %rdi         # Store x (actually z)
    push %rsi         # Store y (actually x)
    push %rdx         # Store z (actually y)

    add $-1, %edi     # z-1
    call tarai        # Recursive call

    pop %rdx          # Restore z (actually z)
    pop %rsi          # Restore y (actually x)
    pop %rdi          # Restore x (actually y)
    mov %rax, %r14    # r14 = result 3

    # HERE COMES THE FUN STUFF (outer recursive call)
    mov %r12, %rdi   # Move first result into x (RDI)
    mov %r13, %rsi   # Move second result into y (RSI)
    mov %r14, %rdx   # Move third result into z (RDX)

    call tarai


taraiend:
    # Restore Callee-saved registers (RBP, RBX, R12-15)
    pop %r14
    pop %r13
    pop %r12

    # End of function
    mov %rbp, %rsp
    pop %rbp
    ret
