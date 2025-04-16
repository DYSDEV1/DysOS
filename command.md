qemu-system-i386 -kernel kernel.bin -d int -no-reboot -no-shutdown

objdump -D build/kernel.bin > debug.txt