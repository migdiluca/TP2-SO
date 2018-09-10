GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHour

%macro accessRTC 1
	push rbp
	mov rbp,rsp
	
	mov rax, 0h
	mov al, %1
	out 70h, al
	in al, 71h

	leave
	ret
%endmacro

getSeconds:
	accessRTC 0

getMinutes:
	accessRTC 2

getHour:
	accessRTC 4
