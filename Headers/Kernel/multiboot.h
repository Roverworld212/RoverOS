#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "types.h"
#include "acpi.h"

#define MBOOT2_MAGIC 0x36d76289

extern void parseCStruct(uint32 *s);
#define VBE_INDEXED 0
#define VBE_RGB 1
#define VBE_EGA 2

//Tags and flags

#define MBOOT_ALIGN 8
#define MBOOT_TAG_END 0
#define MBOOT_CMDLINE 1
#define MBOOT_BOOTLOADER 2
#define MBOOT_MOD 3
#define MBOOT_MEMINFO 4
#define MBOOT_BOOTDEV 5
#define MBOOT_MMAP 6
#define MBOOT_VBE 7
#define MBOOT_FBUFFER 8
#define MBOOT_ELF_SECT 9
#define MBOOT_APM 10
#define MBOOT_EFI32 11
#define MBOOT_EFI64 12
#define MBOOT_SMBIOS 13
#define MBOOT_ACPI_OLD 14
#define MBOOT_ACPI_NEW 15
#define MBOOT_NETWORK 16
#define MBOOT_EFI_MMAP 17
#define MBOOT_EFI_BS 18
#define MBOOT_EFI32_IH 19
#define MBOOT_EFI64_IH 20
#define MBOOT_LOAD_BASE_ADDR 21

#define MBOOT_END 0
#define MBOOT_REQ 1
#define MBOOT_ADDR 2
#define MBOOT_ENTRY 3
#define MBOOT_CONSOLE 4
#define MBOOT_HEADER_FBUFFER 5
#define MBOOT_MOD_ALIGN 6
#define MBOOT_HEADER_EFI_BS 7
#define MBOOT_ENTRY_EFI32 8
#define MBOOT_ENTRY_EFI64 9
#define MBOOT_RELOCATABLE 10

#define ARCH_I386 0
#define ARCH_MIPS 4
#define ARCH_OPTIONAL 1

#define MBOOT_PREF_NONE 0
#define MBOOT_PREF_LOW 1
#define MBOOT_PREF_HIGH 2

#define MBOOT_CONSOLE_REQ 1
#define MBOOT_CONSOLE_EGA 2

struct multibootHeader{
  uint32 magic;
  uint32 arch;
  uint32 len;
  uint32 check;
};

struct multibootHeaderTag{
  uint16 type;
  uint16 flags;
  uint32 sz;
}__attribute__((packed));

struct multibootTagRequest{
  uint16 type;
  uint16 flags;
  uint32 sz;
  uint32 req[0];
};

struct multibootTagAddr{
  uint16 type;
  uint16 flags;
  uint32 sz;
  uint32 headerAddr;
  uint32 loadAddr;
  uint32 loadEnd;
  uint32 bssEnd;
};

struct multibootTagEntry{
  uint16 type;
  uint16 flags;
  uint32 sz;
  uint32 entry;
};

struct multibootConsole{
  uint16 type;
  uint16 flags;
  uint32 sz;
  uint32 consoleFlags;
};

struct multibootFramebuffer{
  uint16 type;
  uint16 flags;
  uint32 sz;
  uint32 width;
  uint32 height;
  uint32 depth; //Also know as bpp
};

struct multibootModAlign{
  uint16 type;
  uint16 flags;
  uint32 sz;
};

struct multibootTagRelocatable{
  uint16 type;
  uint16 flags;
  uint32 sz;
  uint32 minAddr;
  uint32 maxAddr;
  uint32 align;
  uint32 pref;
};

struct multibootColor{
  uint8 red;
  uint8 green;
  uint8 blue;
};

#define MEMORY_AVALIBLE 1
#define MEMORY_RESERVED 2
#define MEMORY_ACPI 3
#define MEMORY_NVS 4
#define MEMORY_BADRAM 5

struct multibootMmapEntry{
  uint64 addr;
  uint64 len;
  uint32 type;
  uint32 zero;
};

struct multibootTag{
  uint32 type;
  uint32 sz;
};

struct multibootTagString{
  uint32 type;
  uint32 sz;
  char str[0];
};

struct multibootTagMod{
  uint32 type;
  uint32 sz;
  uint32 start;
  uint32 end;
  char cmdline[0];
};

struct multibootBasicMem{
  uint32 type;
  uint32 sz;
  uint32 memLower;
  uint32 memUpper;
};

struct multibootBootdev{
  uint32 type;
  uint32 sz;
  uint32 biosdev;
  uint32 slice;
  uint32 part;
};

struct multibootMmap{
  uint32 type;
  uint32 sz;
  uint32 entrysz;
  uint32 entryversion;
  struct multibootMmapEntry entries[0];
};

struct multibootVBEBlock{
  uint8 externalSpec[512];
};

struct multibootVBEMode{
  uint8 externalSpec[256];
};

struct multibootVBE{
  uint32 type;
  uint32 sz;
  uint16 mode;
  uint16 seg;
  uint16 off;
  uint16 len;
  struct multibootVBEBlock controlInfo;
  struct multibootVBEMode modeInfo;
};

struct multibootBufferCommon{
  uint32 type;
  uint32 sz;
  uint64 phys;
  uint32 pitch;
  uint32 width;
  uint32 height;
  uint8 bpp;
  uint8 ftype;
  uint16 reserved;
};

struct multibootFramebufferReal{
  struct multibootBufferCommon common;
  union{
    struct{
      uint16 palette;
      struct multibootColor fbPalette;
    };
    struct{
      uint8 redPos;
      uint8 redMask;
      uint8 greenPos;
      uint8 greenMask;
      uint8 bluePos;
      uint8 blueMask;
    };
  };
};

struct multibootElf{
  uint32 type;
  uint32 sz;
  uint32 num;
  uint32 entsz;
  uint32 shndx;
  char sections[0];
};

struct multibootApm{
  uint32 type;
  uint32 sz;
  uint16 version;
  uint16 cseg;
  uint32 offset;
  uint16 cseg16;
  uint16 dseg;
  uint16 flags;
  uint16 cseglen;
  uint16 cseg16len;
  uint16 dseglen;
};

struct multibootEfi32{
  uint32 type;
  uint32 sz;
  uint32 pointer;
};

struct multibootEfi64{
  uint32 type;
  uint32 sz;
  uint64 pointer;
};

struct multibootSmbios{
  uint32 type;
  uint32 sz;
  uint8 major;
  uint8 minor;
  uint8 reserved[6];
  uint8 tables[0];
};

struct multibootAcpi{
  uint32 type;
  uint32 sz;
  uint8 rsdp[0];
};

struct multibootNetwork{
  uint32 type;
  uint32 sz;
  uint8 dhcpack[0];
};

struct multibootEfimmap{
  uint32 type;
  uint32 sz;
  uint32 descsz;
  uint32 vers;
  uint8 mmap[0];
};

struct multibootEfi32ih{
  uint32 type;
  uint32 sz;
  uint32 pointer;
};

struct multibootEfi64ih{
  uint32 type;
  uint32 sz;
  uint64 pointer;
};

struct multibootBaseLoad{
  uint32 type;
  uint32 sz;
  uint32 baseAddr;
};

//My multiboot.asm struct

extern void readMultiboot(uint64 mbs);

struct mstruct{
  struct{
    uint64 phys;
    uint32 pitch;
    uint32 width;
    uint32 height;
    uint8 bpp;
    uint8 type;
  }vesa;
  struct{
    uint16 mode;
    uint16 seg;
    uint16 off;
    uint16 len;
    uint64 externalSpecVBlock; //Pointer
    uint64 externalSpecVMode; //Pointer
  }vbe;
  struct{
    uint32 base;
  }lba;
  struct{
    uint8 major;
    uint8 minor;
    uint16 reserved[3];
  }smbios;
  struct{
    uint32 esz;
    uint32 evr;
    uint32 entries; //Pointer
  }mmap;
  struct{
    uint32 pointer;
  }efi32;
  struct{
    uint64 pointer;
  }efi64;
  struct{
    uint16 version;
    uint16 cseg;
    uint32 offset;
    uint16 cseg16;
    uint16 desg;
    uint16 flags;
    uint16 cseglen;
    uint16 dseglen;
  }apm;
  struct{
    uint8 dhc;
  }network;
  struct{
    uint32 mods[20];
  }modules;
  struct{
    uint8 new; //0 if old
    uint32 *address;
  }acpi;
}__attribute__((packed));

#endif
