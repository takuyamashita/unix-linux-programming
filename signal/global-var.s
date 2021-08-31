	.file	"global-var.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%lld\n"
	.text
	.globl	handler
	.type	handler, @function
handler:
.LFB14:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	counter(%rip), %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE14:
	.size	handler, .-handler
	.section	.rodata.str1.1
.LC1:
	.string	"counter = %lld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB15:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$handler, %esi
	movl	$2, %edi
	call	signal
	movq	counter(%rip), %rsi
	movl	$1000000000, %eax
.L5:
	subq	$1, %rax
	jne	.L5
	addq	$1000000000, %rsi
	movq	%rsi, counter(%rip)
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE15:
	.size	main, .-main
	.local	counter
	.comm	counter,8,8
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-44)"
	.section	.note.GNU-stack,"",@progbits
