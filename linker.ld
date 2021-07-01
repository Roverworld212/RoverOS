ENTRY(bootstrap_start)
SECTIONS{
  . = 0x100000;
  .multiboot : {
    boot = .; _boot = .; __boot = .;
    *(.mheader)
    . = ALIGN(4096);
  }
  _kernel_start = .;
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
  linker_end = .; _linker_end = .; __linker_end = .;
}
