#fasm boot.asm boot.bin
#fasm kernel24.asm kernel24.bin
#fasm kernel32.asm kernel32.bin
#fasm OS.asm OS.bin
#dd if=/dev/zero of=/dev/sdb bs=1024 count=1440
#dd if=OS.bin of=/dev/sdb conv=notrunc
#qemu-system-i386 -localtime -net nic -net user -m 768M -drive file=/dev/sdb,media=disk,format=raw
