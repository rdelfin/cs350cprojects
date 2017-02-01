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
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%rsi, %rax
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$200, %rsp
	.cfi_def_cfa_offset 256
	cmpq	%rsi, %rdi
	movq	%rsi, 120(%rsp)
	movq	%rdx, 80(%rsp)
	jbe	.L2
	movq	%rdi, %rbx
	movq	%rsi, 48(%rsp)
.L4:
	movq	80(%rsp), %r15
	movq	120(%rsp), %r14
	movq	%rbx, %rsi
	movq	%r15, %rax
	movq	%r14, %rdx
	subq	$1, %rax
	movq	%rax, %rdi
	movq	%rax, 104(%rsp)
	call	tarai
	movq	%r14, %rdi
	movq	%rbx, %rdx
	movq	%r15, %rsi
	subq	$1, %rdi
	movq	%rax, 152(%rsp)
	call	tarai
	movq	%rax, 120(%rsp)
	leaq	-1(%rbx), %rax
	cmpq	48(%rsp), %rax
	movq	%rax, 88(%rsp)
	jbe	.L143
.L117:
	movq	104(%rsp), %rax
	movq	88(%rsp), %rcx
	cmpq	%rcx, %rax
	movq	%rax, 136(%rsp)
	jbe	.L70
	movq	48(%rsp), %rax
	movq	%rcx, 96(%rsp)
	movq	%rcx, %r14
	movq	%rax, 112(%rsp)
	movq	%rax, %rbx
.L24:
	movq	136(%rsp), %r15
	movq	%rbx, %rax
	movq	%r14, %rdx
	subq	$1, %rax
	movq	%rax, %rdi
	movq	%rax, 176(%rsp)
	movq	%r15, %rsi
	call	tarai
	movq	%rax, 160(%rsp)
	movq	%r14, %rax
	subq	$1, %rax
	movq	%rax, %rcx
	movq	%rax, 144(%rsp)
	movq	%rax, 8(%rsp)
	cmpq	%rbx, %rcx
	jbe	.L71
	movq	%rbx, %r13
	movq	%r15, 128(%rsp)
	movq	%r15, %r14
.L15:
	movq	8(%rsp), %rbx
	movq	%r14, %rax
	movq	%r13, %rdx
	subq	$1, %rax
	movq	%rax, %rdi
	movq	%rax, 16(%rsp)
	movq	%rbx, %rsi
	call	tarai
	movq	%rax, 168(%rsp)
	leaq	-1(%r13), %rax
	movq	%rax, %rcx
	movq	%rax, 24(%rsp)
	movq	%rax, %r12
	cmpq	%rcx, %r14
	jnb	.L72
	movq	%r13, 184(%rsp)
	movq	%r14, %r13
.L11:
	leaq	-1(%rbx), %rax
	movq	%r13, %rdx
	movq	%r12, %rsi
	movq	%rax, %rdi
	movq	%rax, 56(%rsp)
	call	tarai
	movq	%rax, 32(%rsp)
	leaq	-1(%r13), %rax
	cmpq	%rax, %rbx
	movq	%rax, 40(%rsp)
	jnb	.L73
	movq	%rbx, %r15
	movq	%r12, %rbp
	movq	%r12, 64(%rsp)
	movq	%rbx, 72(%rsp)
	movq	%r15, %r12
	movq	%rax, %rbx
	.p2align 4,,10
	.p2align 3
.L9:
	leaq	-1(%rbp), %rdi
	movq	%r12, %rdx
	movq	%rbx, %rsi
	call	tarai
	leaq	-1(%r12), %rdi
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	movq	%rax, %r15
	call	tarai
	leaq	-1(%rbx), %rdi
	movq	%rax, %r14
	movq	%rbp, %rdx
	movq	%r12, %rsi
	movq	%r15, %rbp
	movq	%r14, %r12
	call	tarai
	cmpq	%rax, %r14
	movq	%rax, %rbx
	jb	.L9
	movq	64(%rsp), %r12
	movq	72(%rsp), %rbx
	movq	%r14, (%rsp)
.L8:
	leaq	-1(%r12), %r15
	cmpq	%r13, %r15
	jbe	.L144
	movq	40(%rsp), %r12
	movq	56(%rsp), %rdi
	jmp	.L118
	.p2align 4,,10
	.p2align 3
.L145:
	leaq	-1(%rbp), %rdi
	leaq	-1(%r12), %r12
.L118:
	movq	%r13, %rdx
	movq	%r15, %rsi
	call	tarai
	movq	%r15, %rdx
	movq	%rbx, %rsi
	movq	%r12, %rdi
	movq	%rax, %rbp
	call	tarai
	leaq	-1(%r15), %rdi
	movq	%rax, %r12
	movq	%rbx, %rdx
	movq	%r13, %rsi
	movq	%rbp, %rbx
	movq	%r12, %r13
	call	tarai
	cmpq	%rax, %r12
	movq	%rax, %r15
	jb	.L145
	cmpq	%r12, (%rsp)
	movq	32(%rsp), %rbx
	jnb	.L146
.L74:
	movq	(%rsp), %r13
	jmp	.L11
.L144:
	movq	%r13, %r12
	cmpq	%r12, (%rsp)
	movq	32(%rsp), %rbx
	jb	.L74
.L146:
	movq	184(%rsp), %r13
	movq	8(%rsp), %rbp
.L7:
	subq	$1, %rbp
	movq	128(%rsp), %rbx
	movq	%r13, %r15
	cmpq	%r13, %rbp
	jbe	.L147
.L119:
	movq	16(%rsp), %rdi
	movq	%r15, %rdx
	movq	%rbp, %rsi
	call	tarai
	movq	%rax, 32(%rsp)
	movq	24(%rsp), %rax
	cmpq	%rax, %rbx
	jnb	.L76
	movq	%rbp, %r12
	movq	%rbp, 40(%rsp)
	movq	%rbx, 8(%rsp)
	movq	%rax, %rbp
	.p2align 4,,10
	.p2align 3
.L17:
	leaq	-1(%r12), %rdi
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	call	tarai
	leaq	-1(%rbx), %rdi
	movq	%rbp, %rdx
	movq	%r12, %rsi
	movq	%rax, %r14
	call	tarai
	leaq	-1(%rbp), %rdi
	movq	%rax, %r13
	movq	%r12, %rdx
	movq	%rbx, %rsi
	movq	%r14, %r12
	movq	%r13, %rbx
	call	tarai
	cmpq	%rax, %r13
	movq	%rax, %rbp
	jb	.L17
	movq	40(%rsp), %rbp
	movq	8(%rsp), %rbx
	movq	%r13, 8(%rsp)
.L16:
	leaq	-1(%rbp), %r13
	cmpq	%r15, %r13
	jbe	.L148
	movq	16(%rsp), %rdi
	movq	24(%rsp), %rbp
	jmp	.L120
	.p2align 4,,10
	.p2align 3
.L149:
	leaq	-1(%r12), %rdi
	leaq	-1(%rbp), %rbp
.L120:
	movq	%r15, %rdx
	movq	%r13, %rsi
	call	tarai
	movq	%r13, %rdx
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	movq	%rax, %r12
	call	tarai
	leaq	-1(%r13), %rdi
	movq	%rax, %rbp
	movq	%rbx, %rdx
	movq	%r15, %rsi
	movq	%r12, %rbx
	movq	%rbp, %r15
	call	tarai
	cmpq	%rax, %rbp
	movq	%rax, %r13
	jb	.L149
.L22:
	cmpq	8(%rsp), %rbp
	movq	32(%rsp), %rbx
	jbe	.L20
	movq	8(%rsp), %r15
	leaq	-1(%rbx), %rax
	movq	%rax, 16(%rsp)
	leaq	-1(%r15), %rax
	movq	%rax, 24(%rsp)
	jmp	.L119
.L147:
	movq	%r13, 8(%rsp)
.L20:
	movq	(%rsp), %rcx
	cmpq	%rcx, 8(%rsp)
	movq	168(%rsp), %rax
	movq	%rax, 128(%rsp)
	jbe	.L150
	movq	%rcx, %r13
	movq	%rax, %r14
	jmp	.L15
.L148:
	movq	%r15, %rbp
	jmp	.L22
.L76:
	movq	%rbx, 8(%rsp)
	jmp	.L16
.L73:
	movq	%rbx, (%rsp)
	jmp	.L8
.L72:
	movq	%r14, (%rsp)
	movq	%rbx, %rbp
	jmp	.L7
.L150:
	movq	136(%rsp), %rbp
	movq	96(%rsp), %rax
.L6:
	subq	$1, %rbp
	movq	%rax, %r12
	movq	%rax, %rcx
	cmpq	%rax, %rbp
	jbe	.L26
	movq	%rax, %rbx
	movq	112(%rsp), %r13
	movq	176(%rsp), %rdi
	jmp	.L121
.L151:
	leaq	-1(%rbx), %rax
	leaq	-1(%r14), %rdi
	movq	%rax, 144(%rsp)
.L121:
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	call	tarai
	movq	144(%rsp), %rdi
	movq	%rbp, %rdx
	movq	%r13, %rsi
	movq	%rax, %r14
	call	tarai
	leaq	-1(%rbp), %rdi
	movq	%rax, %rbx
	movq	%r13, %rdx
	movq	%r12, %rsi
	movq	%r14, %r13
	movq	%rbx, %r12
	call	tarai
	cmpq	%rax, %rbx
	movq	%rax, %rbp
	jb	.L151
	movq	%rbx, 96(%rsp)
	movq	%rbx, %rcx
.L26:
	cmpq	%rcx, (%rsp)
	movq	160(%rsp), %rax
	movq	%rax, 112(%rsp)
	jnb	.L152
	movq	(%rsp), %rax
	movq	%rcx, 136(%rsp)
	movq	112(%rsp), %rbx
	movq	%rax, 96(%rsp)
	movq	%rax, %r14
	jmp	.L24
.L152:
	movq	(%rsp), %rax
	movq	%rax, 136(%rsp)
.L5:
	movq	48(%rsp), %rax
	movq	80(%rsp), %rcx
	subq	$1, %rax
	cmpq	%rcx, %rax
	movq	%rax, 112(%rsp)
	movq	%rax, (%rsp)
	jbe	.L79
	movq	88(%rsp), %rax
	movq	%rcx, 8(%rsp)
	movq	%rcx, %r14
	movq	%rax, 16(%rsp)
	movq	%rax, %rbx
.L36:
	movq	(%rsp), %r12
	movq	%rbx, %rax
	movq	%r14, %rdx
	subq	$1, %rax
	movq	%rax, %rdi
	movq	%rax, 56(%rsp)
	movq	%r12, %rsi
	call	tarai
	movq	%rax, 32(%rsp)
	movq	%r14, %rax
	subq	$1, %rax
	movq	%rax, %rbp
	movq	%rax, 24(%rsp)
	cmpq	%rbx, %rbp
	jbe	.L80
	movq	%rbx, %r14
.L32:
	leaq	-1(%r12), %rax
	movq	%r14, %rdx
	movq	%rbp, %rsi
	movq	%rax, %rdi
	movq	%rax, 72(%rsp)
	call	tarai
	movq	%rax, 40(%rsp)
	leaq	-1(%r14), %rax
	cmpq	%r12, %rax
	movq	%rax, 64(%rsp)
	jbe	.L81
	movq	%r14, 96(%rsp)
	movq	%rbp, %rbx
	movq	%rbp, 128(%rsp)
	movq	%r12, %r14
	movq	%rax, %r13
	movq	%r12, %r15
.L30:
	leaq	-1(%rbx), %rdi
	movq	%r14, %rdx
	movq	%r13, %rsi
	call	tarai
	leaq	-1(%r14), %rdi
	movq	%r13, %rdx
	movq	%rbx, %rsi
	movq	%rax, %rbp
	call	tarai
	leaq	-1(%r13), %rdi
	movq	%rax, %r12
	movq	%rbx, %rdx
	movq	%r14, %rsi
	movq	%rbp, %rbx
	movq	%r12, %r14
	call	tarai
	cmpq	%rax, %r12
	movq	%rax, %r13
	jb	.L30
	movq	128(%rsp), %rbp
	movq	96(%rsp), %r14
	movq	%r12, %r13
	movq	%r15, %r12
.L29:
	leaq	-1(%rbp), %r15
	cmpq	%r14, %r15
	jbe	.L153
	movq	64(%rsp), %rbp
	movq	72(%rsp), %rdi
	jmp	.L122
.L154:
	leaq	-1(%rbx), %rdi
	leaq	-1(%rbp), %rbp
.L122:
	movq	%r14, %rdx
	movq	%r15, %rsi
	call	tarai
	movq	%r15, %rdx
	movq	%r12, %rsi
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	tarai
	leaq	-1(%r15), %rdi
	movq	%rax, %rbp
	movq	%r12, %rdx
	movq	%r14, %rsi
	movq	%rbx, %r12
	movq	%rbp, %r14
	call	tarai
	cmpq	%rax, %rbp
	movq	%rax, %r15
	jb	.L154
	cmpq	%r13, %rbp
	movq	40(%rsp), %r12
	jbe	.L155
.L82:
	movq	%r13, %r14
	jmp	.L32
.L153:
	movq	%r14, %rbp
	movq	40(%rsp), %r12
	cmpq	%r13, %rbp
	ja	.L82
.L155:
	movq	(%rsp), %rbp
.L28:
	subq	$1, %rbp
	cmpq	8(%rsp), %rbp
	jbe	.L156
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	movq	56(%rsp), %rdi
	jmp	.L123
.L157:
	leaq	-1(%r15), %rax
	leaq	-1(%r14), %rdi
	movq	%rax, 24(%rsp)
.L123:
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	call	tarai
	movq	24(%rsp), %rdi
	movq	%rbp, %rdx
	movq	%r12, %rsi
	movq	%rax, %r14
	call	tarai
	leaq	-1(%rbp), %rdi
	movq	%rax, %r15
	movq	%r12, %rdx
	movq	%rbx, %rsi
	movq	%rax, (%rsp)
	movq	%r15, %rbx
	call	tarai
	cmpq	%rax, %r15
	movq	%rax, %rbp
	movq	%r14, %r12
	jb	.L157
	cmpq	%r13, (%rsp)
	movq	32(%rsp), %rax
	movq	%rax, 16(%rsp)
	jbe	.L158
.L83:
	movq	%r13, 8(%rsp)
	movq	%rax, %rbx
	movq	%r13, %r14
	jmp	.L36
.L81:
	movq	%r12, %r13
	jmp	.L29
.L156:
	movq	8(%rsp), %rax
	movq	%rax, (%rsp)
	cmpq	%r13, (%rsp)
	movq	32(%rsp), %rax
	movq	%rax, 16(%rsp)
	ja	.L83
.L158:
	movq	%r13, 128(%rsp)
.L27:
	movq	88(%rsp), %rax
	subq	$1, %rax
	cmpq	48(%rsp), %rax
	movq	%rax, 64(%rsp)
	jbe	.L159
.L124:
	movq	104(%rsp), %rax
	movq	64(%rsp), %rcx
	cmpq	%rcx, %rax
	movq	%rax, 72(%rsp)
	jbe	.L85
	movq	48(%rsp), %rax
	movq	%rcx, 96(%rsp)
	movq	%rax, 88(%rsp)
.L56:
	movq	72(%rsp), %rcx
	subq	$1, %rax
	movq	%rax, 24(%rsp)
	cmpq	%rcx, %rax
	jbe	.L86
	movq	96(%rsp), %rax
	movq	%rcx, 32(%rsp)
	movq	%rax, 40(%rsp)
.L51:
	movq	24(%rsp), %rcx
	subq	$1, %rax
	movq	%rax, 56(%rsp)
	movq	%rax, (%rsp)
	cmpq	%rcx, %rax
	jbe	.L87
	movq	32(%rsp), %r14
	movq	%rcx, 8(%rsp)
	movq	%rcx, %r15
	movq	%rax, %rbx
	.p2align 4,,10
	.p2align 3
.L47:
	leaq	-1(%r14), %rdi
	leaq	-1(%r15), %r13
	movq	%r15, %rdx
	movq	%rbx, %rsi
	call	tarai
	cmpq	%r14, %r13
	movq	%rax, 16(%rsp)
	jbe	.L88
	movq	%r14, %r15
	.p2align 4,,10
	.p2align 3
.L46:
	leaq	-1(%rbx), %rdi
	movq	%r15, %rdx
	movq	%r13, %rsi
	call	tarai
	leaq	-1(%r15), %rdi
	movq	%r13, %rdx
	movq	%rbx, %rsi
	movq	%rax, %r12
	call	tarai
	leaq	-1(%r13), %rdi
	movq	%rax, %rbp
	movq	%rbx, %rdx
	movq	%r15, %rsi
	movq	%r12, %rbx
	movq	%rbp, %r15
	call	tarai
	cmpq	%rax, %rbp
	movq	%rax, %r13
	jb	.L46
	movq	(%rsp), %rdi
	movq	8(%rsp), %rsi
.L45:
	subq	$1, %rdi
	movq	%r14, %rdx
	call	tarai
	cmpq	%rbp, %rax
	movq	%rax, (%rsp)
	movq	16(%rsp), %r14
	jbe	.L44
	movq	%rbp, 8(%rsp)
	movq	%rbp, %r15
	movq	%rax, %rbx
	jmp	.L47
.L87:
	movq	%rcx, %rbp
.L44:
	movq	32(%rsp), %rax
	leaq	-1(%rax), %r12
	movq	40(%rsp), %rax
	movq	%r12, %r15
	cmpq	%rax, %r12
	jbe	.L90
	movq	24(%rsp), %rbx
	movq	%rbp, (%rsp)
	movq	%rax, %rbp
	.p2align 4,,10
	.p2align 3
.L49:
	leaq	-1(%rbx), %rdi
	movq	%rbp, %rdx
	movq	%r15, %rsi
	call	tarai
	leaq	-1(%rbp), %rdi
	movq	%r15, %rdx
	movq	%rbx, %rsi
	movq	%rax, %r13
	call	tarai
	leaq	-1(%r15), %rdi
	movq	%rax, %r14
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	movq	%r13, %rbx
	movq	%r14, %rbp
	call	tarai
	cmpq	%rax, %r14
	movq	%rax, %r15
	jb	.L49
	movq	(%rsp), %rbp
.L48:
	movq	24(%rsp), %r13
	subq	$1, %r13
	cmpq	32(%rsp), %r13
	jbe	.L160
	movq	%rbp, (%rsp)
	movq	40(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	56(%rsp), %rdi
	jmp	.L125
	.p2align 4,,10
	.p2align 3
.L161:
	leaq	-1(%r15), %rdi
	leaq	-1(%r12), %r12
.L125:
	movq	%rbp, %rdx
	movq	%r13, %rsi
	call	tarai
	movq	%r13, %rdx
	movq	%rbx, %rsi
	movq	%r12, %rdi
	movq	%rax, %r15
	call	tarai
	leaq	-1(%r13), %rdi
	movq	%rax, %r12
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	movq	%r15, %rbx
	movq	%r12, %rbp
	call	tarai
	cmpq	%rax, %r12
	movq	%rax, %r13
	jb	.L161
	movq	(%rsp), %rbp
	movq	%r12, 24(%rsp)
.L53:
	cmpq	%r14, 24(%rsp)
	jbe	.L43
	movq	%rbp, 40(%rsp)
	movq	%r14, 32(%rsp)
	movq	%rbp, %rax
	jmp	.L51
.L88:
	movq	%r14, %rbp
	movq	%rbx, %rdi
	movq	%r15, %rsi
	jmp	.L45
.L86:
	movq	%rcx, %r14
.L43:
	movq	96(%rsp), %rax
	leaq	-1(%rax), %r13
	movq	88(%rsp), %rax
	cmpq	%rax, %r13
	jbe	.L92
	movq	72(%rsp), %rbx
	movq	%rax, %r15
.L55:
	leaq	-1(%rbx), %rdi
	movq	%r15, %rdx
	movq	%r13, %rsi
	call	tarai
	leaq	-1(%r15), %rdi
	movq	%r13, %rdx
	movq	%rbx, %rsi
	movq	%rax, %r12
	call	tarai
	leaq	-1(%r13), %rdi
	movq	%rax, %rbp
	movq	%rbx, %rdx
	movq	%r15, %rsi
	movq	%r12, %rbx
	movq	%rbp, %r15
	call	tarai
	cmpq	%rax, %rbp
	movq	%rax, %r13
	jb	.L55
	movq	88(%rsp), %rdx
.L54:
	movq	72(%rsp), %rdi
	movq	96(%rsp), %rsi
	subq	$1, %rdi
	call	tarai
	cmpq	%rbp, %rax
	movq	%rax, 72(%rsp)
	jbe	.L162
	movq	%r14, 88(%rsp)
	movq	%rbp, 96(%rsp)
	movq	%r14, %rax
	jmp	.L56
.L160:
	movq	32(%rsp), %rax
	movq	%rax, 24(%rsp)
	jmp	.L53
.L90:
	movq	%rax, %r14
	jmp	.L48
.L162:
	movq	%rbp, 40(%rsp)
.L42:
	movq	112(%rsp), %rax
	movq	80(%rsp), %rcx
	cmpq	%rcx, %rax
	movq	%rax, %rbp
	jbe	.L94
	movq	64(%rsp), %r13
	movq	%rcx, %r12
.L58:
	leaq	-1(%r13), %rdi
	movq	%r12, %rdx
	movq	%rbp, %rsi
	call	tarai
	leaq	-1(%r12), %rdi
	movq	%rbp, %rdx
	movq	%r13, %rsi
	movq	%rax, %rbx
	call	tarai
	leaq	-1(%rbp), %rdi
	movq	%rax, %r14
	movq	%r13, %rdx
	movq	%r12, %rsi
	movq	%rax, 32(%rsp)
	movq	%r14, %r12
	call	tarai
	cmpq	%rax, %r14
	movq	%rax, %rbp
	movq	%rbx, %r13
	jb	.L58
.L57:
	movq	64(%rsp), %rax
	subq	$1, %rax
	cmpq	48(%rsp), %rax
	movq	%rax, 24(%rsp)
	jbe	.L163
.L126:
	movq	104(%rsp), %rax
	movq	24(%rsp), %rcx
	cmpq	%rcx, %rax
	movq	%rax, (%rsp)
	jbe	.L96
	movq	48(%rsp), %r14
	movq	%rcx, 8(%rsp)
	movq	%rcx, %r15
	movq	%rax, %rbx
.L65:
	leaq	-1(%r14), %rdi
	leaq	-1(%r15), %r13
	movq	%r15, %rdx
	movq	%rbx, %rsi
	call	tarai
	cmpq	%r14, %r13
	movq	%rax, 16(%rsp)
	jbe	.L97
	movq	%r14, %r15
	.p2align 4,,10
	.p2align 3
.L64:
	leaq	-1(%rbx), %rdi
	movq	%r15, %rdx
	movq	%r13, %rsi
	call	tarai
	leaq	-1(%r15), %rdi
	movq	%r13, %rdx
	movq	%rbx, %rsi
	movq	%rax, %r12
	call	tarai
	leaq	-1(%r13), %rdi
	movq	%rax, %rbp
	movq	%rbx, %rdx
	movq	%r15, %rsi
	movq	%r12, %rbx
	movq	%rbp, %r15
	call	tarai
	cmpq	%rax, %rbp
	movq	%rax, %r13
	jb	.L64
	movq	(%rsp), %rdi
	movq	8(%rsp), %rsi
.L63:
	subq	$1, %rdi
	movq	%r14, %rdx
	call	tarai
	cmpq	%rbp, %rax
	movq	%rax, (%rsp)
	movq	16(%rsp), %r14
	jbe	.L62
	movq	%rbp, 8(%rsp)
	movq	%rbp, %r15
	movq	%rax, %rbx
	jmp	.L65
.L96:
	movq	%rcx, %rbp
.L62:
	movq	112(%rsp), %r14
	movq	80(%rsp), %rax
	cmpq	%rax, %r14
	jbe	.L99
	movq	24(%rsp), %rbx
	movq	%rax, %r15
.L67:
	leaq	-1(%rbx), %rdi
	movq	%r15, %rdx
	movq	%r14, %rsi
	call	tarai
	leaq	-1(%r15), %rdi
	movq	%r14, %rdx
	movq	%rbx, %rsi
	movq	%rax, %r13
	call	tarai
	leaq	-1(%r14), %rdi
	movq	%rax, %r12
	movq	%rbx, %rdx
	movq	%r15, %rsi
	movq	%r13, %rbx
	movq	%r12, %r15
	call	tarai
	cmpq	%rax, %r12
	movq	%rax, %r14
	jb	.L67
	movq	80(%rsp), %rdx
.L66:
	movq	24(%rsp), %rdi
	movq	48(%rsp), %rsi
	subq	$1, %rdi
	call	tarai
	cmpq	%r12, %rax
	movq	%rax, 24(%rsp)
	movq	%r12, 48(%rsp)
	jbe	.L164
	movq	%rbp, %rax
	movq	%rbp, 80(%rsp)
	subq	$1, %rax
	movq	%rax, 104(%rsp)
	movq	48(%rsp), %rax
	subq	$1, %rax
	movq	%rax, 112(%rsp)
	jmp	.L126
.L97:
	movq	%r14, %rbp
	movq	%rbx, %rdi
	movq	%r15, %rsi
	jmp	.L63
.L99:
	movq	%rax, %r12
	movq	%rax, %rdx
	jmp	.L66
.L164:
	movq	%r12, 64(%rsp)
.L69:
	movq	32(%rsp), %rax
	cmpq	%rax, 64(%rsp)
	movq	%rax, 48(%rsp)
	jbe	.L141
	movq	40(%rsp), %rax
	movq	%rax, 80(%rsp)
	subq	$1, %rax
	movq	%rax, 104(%rsp)
	movq	48(%rsp), %rax
	subq	$1, %rax
	movq	%rax, 112(%rsp)
	jmp	.L124
.L92:
	movq	%rax, %rbp
	movq	%rax, %rdx
	jmp	.L54
.L159:
	movq	48(%rsp), %rax
.L141:
	movq	%rax, 88(%rsp)
	movq	128(%rsp), %rax
	cmpq	88(%rsp), %rax
	movq	%rax, 48(%rsp)
	jnb	.L165
	movq	136(%rsp), %rax
	movq	%rax, 80(%rsp)
	subq	$1, %rax
	movq	%rax, 104(%rsp)
	jmp	.L117
.L163:
	movq	48(%rsp), %rax
	movq	%rax, 64(%rsp)
	jmp	.L69
.L94:
	movq	%rcx, 32(%rsp)
	jmp	.L57
.L165:
	movq	%rax, %rbx
.L41:
	movq	120(%rsp), %rax
	movq	152(%rsp), %rcx
	cmpq	%rbx, %rax
	movq	%rax, 48(%rsp)
	movq	%rcx, 80(%rsp)
	jb	.L4
.L2:
	addq	$200, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L85:
	.cfi_restore_state
	movq	%rcx, 40(%rsp)
	jmp	.L42
.L80:
	movq	%rbx, %r13
	movq	%r12, %rbp
	jmp	.L28
.L79:
	movq	%rcx, 128(%rsp)
	jmp	.L27
.L71:
	movq	%rbx, (%rsp)
	movq	%r15, %rbp
	movq	%r14, %rax
	jmp	.L6
.L70:
	movq	%rcx, 136(%rsp)
	jmp	.L5
.L143:
	movq	48(%rsp), %rbx
	jmp	.L41
	.cfi_endproc
.LFE0:
	.size	tarai, .-tarai
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
