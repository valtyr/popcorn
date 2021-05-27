section .multiboot_header

header_start:
  ; Magic number for multiboot 2
  dd 0xe85250d6
  
  ; Protected mode i386
  dd 0;
  
  ; Header length
  dd header_end - header_start

  ; Checksum
  dd 0x100000000 - (0xe85250d6 + 0 + header_end - header_start)

  ; ; Framebuffer tag
  ; dd 5
  ; dd 0x0000
  ; dw 20
  ; dw 320
  ; dw 200
  ; dw 256
  ; dd 8

  ; End tag
  dd 0
  dw 0
  dd 8

  
header_end:


; framebuffer_tag_start:  
;         .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
;         .short MULTIBOOT_HEADER_TAG_OPTIONAL
;         .long framebuffer_tag_end - framebuffer_tag_start
;         .long 1024
;         .long 768
;         .long 32