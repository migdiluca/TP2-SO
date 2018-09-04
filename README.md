# TPE-Arqui

char * v = (char*)0xB8000;

void writeInScreen(char * text, int pos, char format){
	int i=0;
	v+=pos*2;
	while(text[i/2]){
		v[i] = text[i/2];
		v[i+1] = format;
		i+=2;
	}
}


GLOBAL clock

section .text

clock:
	pop rbp
	mov rbp,rsp
	mov al, BYTE[ebp+8]
	out 70h, al
	in ax, 71h
	leave
