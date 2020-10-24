#include "types.h"
#include "multiboot.h"
#include "libcom.h"
#include "kheaders.h"
#include "paging.h"
#include "libbin.h"

uint32 mc = 0;

void cmdline(struct multibootTag *tag){
  printq("--Command Line--\n");
  struct multibootTagString *cline = (struct multibootTagString*)tag;
  if(cline->str[0] != NULL){
    printq(cline->str);
    printq("\n");
  }
  else{
    printq("None\n");
  }
}

void bootloader(struct multibootTag *tag){
  printq("--Boot Loader--\n");
  printq("%s\n", ((struct multibootTagString*)tag)->str);
}

void module(struct multibootTag *tag){
  printq("--Module--\n");
  struct multibootTagMod *mod = (struct multibootTagMod*)tag;
  printq("Mod at: %x - %x\n", mod->start, mod->end);
  printq("Cmdline: %x\n", mod->cmdline[0]);
  if(mc == 0){
    kvinf.kvfs.rdStart = mod->start;
  }
  curAddr = mod->end;
  ++mc;
}

void meminfo(struct multibootTag *tag){
  printq("--Meminfo Basic--\n");  
  struct multibootBasicMem *mem = ((struct multibootBasicMem*)tag);
  printq("Memlower: %x\nMemUpper: %x\n", mem->memLower, mem->memUpper);
}

void bootdev(struct multibootTag *tag){
  printq("--Bootdev--\n");
  struct multibootBootdev *boot = (struct multibootBootdev*)tag;
  printq("Boot device: %x\n", boot->biosdev);
}

void mmap(struct multibootTag *tag){
  printq("--Mem Map--\n");
  struct multibootMmapEntry *map;
  uint64 Cbase;
  uint64 Clen;
  for(map = (((struct multibootMmap*)tag)->entries); (uint8*)map < (uint8*)tag+tag->sz; map = (struct multibootMmapEntry*)((uint64)map+((struct multibootMmap*)tag)->entrysz)){
    Cbase = (uint64)((map->addr>>32)|(map->addr&0xffffffff));
    Clen = (uint64)((map->len>>32)|(map->len&0xffffffff));
    printq("Base: %x\n", Cbase);
    printq("Len: 0x%X%X\n", (unsigned)(map->len>>32), (unsigned)(map->len&0xffffffff));
    switch(map->type){
      case MEMORY_AVALIBLE:
      printq("Avalible\n");
      kvinf.kmem.free += Clen;
      break;
      case MEMORY_RESERVED:
      printq("Reserved\n");
      kvinf.kmem.reserved += Clen;
      break;
      case MEMORY_ACPI:
      printq("ACPI\n");
      kvinf.kmem.acpi += Clen;
      break;
      case MEMORY_NVS:
      printq("NVS\n");
      kvinf.kmem.nvs += Clen;
      break;
      case MEMORY_BADRAM:
      printq("Bad Ram\n");
      kvinf.kmem.badram += Clen;
      break;
      default:
      printq("Unknown\n");
      kvinf.kmem.unknown += Clen;
      break;
    }
    printq("========\n");
  }
}

void framebuffer(struct multibootTag *tag){
  printq("--Framebuffer--\n");
  struct multibootFramebufferReal *fb = (struct multibootFramebufferReal*)tag;
  printq("Phys: %x\n", fb->common.phys);
  printq("Pitch: %x\n", fb->common.pitch);
  printq("Width: %x\n", fb->common.width);
  printq("Height: %x\n", fb->common.height);
  printq("Bpp: %x\n", fb->common.bpp, fb->common.bpp);
  printq("Bypp: %x\n", (fb->common.bpp/8), (fb->common.bpp/8));
  printq("Palette: %x\n", fb->palette);
  printq("Red: %x Blue: %x Green: %x\n", fb->fbPalette.red, fb->fbPalette.blue, fb->fbPalette.green);
  printq("RedPos: %x RedMask: %x\n", fb->redPos, fb->redMask);
  printq("BluePos: %x BlueMask: %x\n", fb->bluePos, fb->blueMask);
  printq("GreenPos: %x GreenMask: %x\n", fb->greenPos, fb->greenMask);
  switch(fb->common.ftype){
    case VBE_INDEXED:
    printq("Using indexed mode\n");
    kernelPanic("Bad Mode");
    break;
    case VBE_EGA:
    printq("Using EGA mode\n");
    kernelPanic("Bad Mode");
    break;
    case VBE_RGB:
    printq("Using RGB mode\n");
    kvinf.kui.mode = VBE_RGB;
    kvinf.kui.xsz = fb->common.width;
    kvinf.kui.ysz = fb->common.height;
    kvinf.kui.bpp = fb->common.bpp;
    kvinf.kui.pitch = fb->common.pitch;
    kvinf.kui.buffer = fb->common.phys;
    break;
    default:
    printq("Unknown mode: %x\n", fb->common.ftype);
    kernelPanic("Unknown Mode");
    break;
  }
}

void acpiOld(struct multibootTag *tag){
  struct multibootAcpi *acpi = (struct multibootAcpi*)tag;
  printq("--ACPI 1.0--\n");
  printq("Root Pointer: %x\n", acpi->rsdp);
  parseACPI(0, acpi->rsdp);
}

void acpiNew(struct multibootTag *tag){
  struct multibootAcpi *acpi = (struct multibootAcpi*)tag;
  printq("--ACPI 2.0--\n");
  printq("Root Pointer: %x\n", acpi->rsdp);
  parseACPI(1, acpi->rsdp);
}

void mbootNetwork(struct multibootTag *tag){
  struct multibootNetwork *net = (struct multibootNetwork*)tag;
  printq("--Networking--\n");
  printq("dhcpack[0]: %x\n", net->dhcpack[0]);
}

void mbootApm(struct multibootTag *tag){
  struct multibootApm *apm = (struct multibootApm*)tag;
  printq("--APM--\n");
  printq("Version: %x\nCseg: %x\nCsegLen: %x\n", apm->version, apm->cseg, apm->cseglen);
  printq("Cseg16: %x\nCseg16Len: %x\nDseg: %x\nDsegLen: %x\n", apm->cseg16, apm->cseg16len, apm->dseg, apm->dseglen);
  printq("Offset: %x\nFlags: ", apm->offset);
  for(uint32 i = 0; i < 32; ++i){
    if(0x40000000>>i & apm->flags){
      printq("1");
    }
    else{
      printq("0");
    }
  }
  printq("\n");
}

void efi32(struct multibootTag *tag){
  struct multibootEfi32 *efi = (struct multibootEfi32*)tag;
  printq("--EFI 32--\n");
  printq("Pointer: %x\n", efi->pointer);
}

void mbootSmbios(struct multibootTag *tag){
  struct multibootSmbios *smb = (struct multibootSmbios*)tag;
  printq("--SMBIOS--\n");
  printq("Version: %i.%i\n", smb->major, smb->minor);
  printq("Tables[0]: %x\n", smb->tables[0]);
}

void mbootLBA(struct multibootTag *tag){
  struct multibootBaseLoad *mbl = (struct multibootBaseLoad*)tag;
  printq("--Load Base Address--\n");
  printq("Base: %x\n", mbl->baseAddr);
}

void readStruct(uint64 ptr){
  printq("================================MULTIBOOT STRUCT================================\n");
  struct multibootTag *tag = (struct multibootTag*)(ptr+8);
  uint32 sz = (unsigned*)ptr;
  printq("Header size: %x\n", sz);
  for(; tag->type != MBOOT_TAG_END; tag = NEXT_TAG(tag)){
    //printq("Tag - Type: %x Size: %x\n", tag->type, tag->sz);
    switch((uint32)tag->type){
      case MBOOT_CMDLINE:
      cmdline(tag);
      break;
      case MBOOT_BOOTLOADER:
      bootloader(tag);
      break;
      case MBOOT_MOD:
      module(tag);
      break;
      case MBOOT_MEMINFO:
      meminfo(tag);
      break;
      case MBOOT_BOOTDEV:
      bootdev(tag);
      break;
      case MBOOT_MMAP:
      mmap(tag);
      break;
      case MBOOT_FBUFFER:
      framebuffer(tag);
      break;
      case MBOOT_ACPI_NEW:
      acpiNew(tag);
      break;
      case MBOOT_ACPI_OLD:
      acpiOld(tag);
      break;
      case MBOOT_NETWORK:
      mbootNetwork(tag);
      break;
      case MBOOT_APM:
      mbootApm(tag);
      break;
      case MBOOT_EFI32:
      efi32(tag);
      break;
      case MBOOT_SMBIOS:
      mbootSmbios(tag);
      break;
      case MBOOT_LOAD_BASE_ADDR:
      mbootLBA(tag);
      break;
    }
  }
  printq("================================================================================\n");
}
