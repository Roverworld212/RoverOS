#include "libstd.h"
#include "libstdio.h"
#include "libio.h"
#include "libksystem.h"
#include "kvinf.h"
#include "acpi.h"
#include "paging.h"

void readACPI(){
    printq("%qReading ACPI\n", QINFO);
    if(kvinf.kernel.multiboot->acpi.new == 0x01){
        printq("%qACPI 2.0 is not supported!\n", QWARNING);
        return;
    }
    struct rdsp *acpi = (struct rdsp*)kvinf.kernel.multiboot->acpi.address;
    printq("%qAddress: 0x%llx\n", QINFO, kvinf.kernel.multiboot->acpi.address);
    printq("%qSignature: %s\n", QINFO, (uint8*)acpi->signature);
    printq("%qChecksum 0x%hx\n", QINFO, acpi->check);
    printq("%qOEM: %s\n", QINFO, &acpi->OEM);
    printq("%qRevision: 0x%x\n", QINFO, acpi->revision);
    printq("%qAddress: 0x%lx\n", QINFO, acpi->address);
    printq("%qDone\n", QINFO);
}