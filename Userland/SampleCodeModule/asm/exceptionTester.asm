GLOBAL invOp
GLOBAL div0

div0:
	mov rdx, 5
	mov al,1
	mov bl,0
	div bl
	ret

invOp:
	ud2
	ret