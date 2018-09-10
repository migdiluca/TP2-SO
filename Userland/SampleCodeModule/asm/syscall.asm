GLOBAL _syscall

section .text

_syscall:
	push rbp
	mov rbp, rsp

	mov rax, rdi

	mov rdi, rsi
	mov rsi, rdx
	mov rdx, rcx
	mov r10, r8
	mov r8, r9

	int 80h

	leave
	ret


