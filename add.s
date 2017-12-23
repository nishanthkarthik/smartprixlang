	.text
	.file	"add.ll"
	.globl	xadd                    # -- Begin function xadd
	.p2align	4, 0x90
	.type	xadd,@function
xadd:                                   # @xadd
	.cfi_startproc
# BB#0:
                                        # kill: %ESI<def> %ESI<kill> %RSI<def>
                                        # kill: %EDI<def> %EDI<kill> %RDI<def>
	leal	(%rdi,%rsi), %eax
	retq
.Lfunc_end0:
	.size	xadd, .Lfunc_end0-xadd
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	pushq	%rax
.Lcfi0:
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	movl	$10, %esi
	callq	xadd
	movl	$.Lstr, %edi
	movl	$7, %esi
	xorl	%eax, %eax
	callq	printf
	xorl	%eax, %eax
	popq	%rcx
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.Lstr,@object           # @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"%d"
	.size	.Lstr, 3


	.section	".note.GNU-stack","",@progbits
