GLOBAL beepasm
GLOBAL unbeepasm
section .text

beepasm:
    push rbp
    mov rbp, rsp

    mov rax,0;    seteo el registro de rax todo en 0.
    mov al, 182; preparo al speaker para realizar el beep.
    out 0x43, al
    mov ax, 9121; cargo la frecuencia.
    out 0x42, al; envio la parte baja del byte
    mov al, ah
    out 0x42, al; envio la parte alta
    in al, 0x61
    or al, 0x03; prendo los bits para emitir el beep.
    out 61h, al

    mov rsp, rbp
    pop rbp
    ret

unbeepasm:
    in al, 0x61
    and al, 0xFC; apago los bits para apagar el beep.
    out 0x61, al
    ret
