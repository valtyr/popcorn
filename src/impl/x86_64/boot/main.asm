global start
global video_back_buffer
global popcorn_logo
global popcorn_chime
global popcorn_chime_length

extern kernel_main

section .data

popcorn_logo:
  incbin "bios-graphics/fbi/test-image.fbi"

popcorn_chime:
  incbin "bios-graphics/popcorn.pcm16"
popcorn_chime_length:
  dd (popcorn_chime_length-popcorn_chime)

section .text
bits 32

start:
  ; Set stack pointer to top of stack
  mov esp, stack_top

  ; Reset EFLAGS
  push 0x00000000
  popf
  
  push ebx ; Pointer to Multiboot info structure
  push eax ; Magic value

  ; Call the c kernel main function
  call kernel_main


; Reserve space for the stack
section .bss
align 4096

stack_bottom:
  resb 4096 * 4
stack_top:

video_back_buffer:
  resb 1024 * 768 * 4
