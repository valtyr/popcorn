bits 64

global BIOSWait
global BIOSHalt

BIOSWait:
    push rbp
    mov rbp, rsp

    mov cx, [rbp+8]
    mov dx, [rbp+8]

    mov ah, 86h
    mov al, 0
    int 15h

    pop rbp
    ret

BIOSHalt:
    hlt
