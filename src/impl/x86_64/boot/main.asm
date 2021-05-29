global start
extern kernel_main

section .text
bits 32

start:
  ; Set stack pointer to top of stack
  mov esp, stack_top

  ; Call the c kernel main function
  call kernel_main


; Reserve space for the stack
section .bss
align 4096

stack_bottom:
  resb 4096 * 4
stack_top:
