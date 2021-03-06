fasm boot.asm boot.bin
fasm kernel.asm kernel.bin
fasm writer.asm writer.bin
fasm OS.asm OS.bin
dd if=/dev/zero of=OS.img bs=1024 count=1440
dd if=OS.bin of=OS.img conv=notrunc
qemu-system-i386 -localtime -net nic -net user -m 768M -drive file=OS.img,media=disk,format=raw
