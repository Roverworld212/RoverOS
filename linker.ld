ENTRY(start)
SECTIONS{
  . = 0x100000;
  .multiboot : {
  boot = .; _boot = .; __boot = .;
  *(.mheader)
  . = ALIGN(4096);
  }
  .text : {
  code = .; _code = .; __code = .;
  *(.text)
  . = ALIGN(4096);
  }
  .data : {
  data = .; _data = .; __data = .;
  *(.data)
  *(.rodata)
  . = ALIGN(4096);
  }
  .bss : {
  bss = .; _bss = .; __bss = .;
  *(.bss)
  . = ALIGN(4096);
  }
  load = 0x0;
  hhalf = 0x0;
  end = .; _end = .; __end = .;
}
