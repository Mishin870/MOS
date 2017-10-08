fasm boot.asm boot.bin
fasm kernel.asm kernel.bin
fasm OS.asm OS.bin
dd if=/dev/zero of=/dev/sdb bs=1024 count=1440
dd if=OS.bin of=/dev/sdb conv=notrunc
qemu-system-i386 -localtime -net nic -net user -m 768M -drive file=/dev/sdb,media=disk,format=raw
