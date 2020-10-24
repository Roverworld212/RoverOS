gcc ./ramdisk/ramdisk.c -o ./ramdisk/ramdisk.o -I ./Headers;
chmod +x ./ramdisk/ramdisk.o;
./ramdisk/ramdisk.o;
mv -f ./ramdisk/disk.initrd ./isodir/boot
