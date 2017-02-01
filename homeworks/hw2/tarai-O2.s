	.file	"tarai.c"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
	.globl	tarai
	.type	tarai, @function
tarai:
.LFB0:
	.cfi_startproc
	cmpq	%rsi, %rdi
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	jbe	.L4
	movq	%rdi, %rbx
	movq	%rdx, %r12
	.p2align 4,,10
	.p2align 3
.L3:
	leaq	-1(%r12), %rdi
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	call	tarai
	leaq	-1(%rbp), %rdi
	movq	%rbx, %rdx
	movq	%r12, %rsi
	movq	%rax, %r14
	call	tarai
	leaq	-1(%rbx), %rdi
	movq	%rax, %r13
	movq	%r12, %rdx
	movq	%rbp, %rsi
	movq	%r14, %r12
	movq	%r13, %rbp
	call	tarai
	cmpq	%rax, %r13
	movq	%rax, %rbx
	jb	.L3
.L2:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%r13, %rax
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L4:
	.cfi_restore_state
	movq	%rsi, %r13
	jmp	.L2
	.cfi_endproc
.LFE0:
	.size	tarai, .-tarai
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
