qemu-system-i386 -kernel kernel.bin -d int -no-reboot -no-shutdown

objdump -D build/kernel.bin > debug.txt


Debugging with gdb: 

qemu-system-i386 -kernel build/kernel.bin -S -s


gdb 

file build/kernel.bin      
target remote localhost:1234