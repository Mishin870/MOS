fasm graph.asm graph.bin
dd if=/dev/zero of=/dev/sdb bs=1024 count=1440
dd if=graph.bin of=/dev/sdb conv=notrunc
qemu-system-i386 -localtime -net nic -net user -m 768M -drive file=/dev/sdb,media=disk,format=raw