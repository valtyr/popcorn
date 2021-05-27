bits 16

section .data

image incbin "bios-graphics/bin/splash.png.bin"

section .text

bits 32

; enter_v86:
;    mov ebp, esp               ; save stack pointer
;    mov esp, v86_stack_top

;    push dword  [ebp+4]        ; ss
;    push dword  [ebp+8]        ; esp
;    pushfd                     ; eflags
;    or dword [esp], (1 << 17)  ; set VM flags
;    push dword [ebp+12]        ; cs
;    push dword  [ebp+16]       ; eip
;    iret


; enter_graphics_mode:
;     pusha
;     mov ah, 00h
;     mov al, 13h
;     int 10h
;     popa
;     ret


; return_to_text_mode:
;     pusha
;     mov ah, 00h
;     mov al, 03h
;     int 10h
;     popa
;     ret

copy_splash_image:
    pusha

    mov esi, image
    mov edi, 0xA0000
    cld
    mov ecx, 64000
    rep movsb

    popa

    ret


global intro_main
intro_main:
    call copy_splash_image