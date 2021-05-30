
MULTIBOOT_MAGIC:        equ  0xe85250d6
MULTIBOOT_ARCH:         equ  0
MULTIBOOT_LENGTH:       equ  (header_end - header_start)
MULTIBOOT_CHECKSUM:     equ  -(MULTIBOOT_MAGIC + MULTIBOOT_ARCH + MULTIBOOT_LENGTH)


; MULTIBOOT_PAGE_ALIGN:   equ  0x1
; MUTLIBOOT_MEMORY_INFO:  equ  0x2
; MUTIBOOT_VIDEO_MODE:    equ  0x4
; MULTIBOOT_FLAGS:        equ  MULTIBOOT_PAGE_ALIGN | MUTLIBOOT_MEMORY_INFO | MUTIBOOT_VIDEO_MODE


section .multiboot_header
align	8

header_start:
  dd MULTIBOOT_MAGIC
  dd MULTIBOOT_ARCH
  dd MULTIBOOT_LENGTH
  dd MULTIBOOT_CHECKSUM
info_request_start:
  dw 1
  dw 0
  dd info_request_end - info_request_start
  dd 1          ; Request boot command line
  dd 4          ; Request basic memory information
  dd 5          ; Request BIOS boot device
  dd 6          ; Request memory map
  dd 8          ; Request framebuffer info
info_request_end:
  dd 0          ; Padding

  ; Video mode
  dw 5          ; Framebuffer tag type
  dw 0          ; Flags
  dd 20         ; Size (fixed 20)
  dd 1024       ; Width
  dd 768        ; Height
  dd 32         ; Depth
  dd 0          ; Padding

  ; End tag
  dd 0
  dw 0
  dd 8
header_end:


