ENTRY(start)

SECTIONS
{

  /* . = 512K;

  .intro :
  {
    *(.intro)
  } */

  . = 1M;

  .boot :
  {
    KEEP(*(.multiboot_header))
  }

  .text :
  {
    *(.text)
  }
}
