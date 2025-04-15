.section .multiboot
.set MAGIC, 0x1BADB002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.long MAGIC
.long FLAGS
.long CHECKSUM

.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader:

    movl $kernel_stack, %esp

    call callConstructors

    pushl %eax 
    pushl %ebx
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop

.section .bss
.lcomm kernel_stack, 2*1024*1024