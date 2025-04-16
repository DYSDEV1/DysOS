# GDB Commands for OS Development

## Basic Control
- `run` or `r` - Start the program
- `continue` or `c` - Continue execution
- `next` or `n` - Step over (execute one line)
- `step` or `s` - Step into (go into function calls)
- `finish` - Run until current function returns
- `kill` - Stop the program
- `quit` or `q` - Exit GDB

## Breakpoints
- `break function_name` or `b function_name` - Set breakpoint at function
- `break *0x12345678` - Set breakpoint at memory address
- `break file.c:123` - Set breakpoint at line
- `break +5` - Set breakpoint 5 lines ahead
- `watch variable` - Stop when variable changes
- `delete 1` - Delete breakpoint number 1
- `disable 1` - Disable breakpoint number 1
- `enable 1` - Enable breakpoint number 1
- `info breakpoints` - List all breakpoints

## Examining State
- `backtrace` or `bt` - Show call stack
- `info registers` - Show all registers
- `info registers eax ebx` - Show specific registers
- `print variable` or `p variable` - Print variable value
- `print/x $eax` - Print register in hex
- `print/d $eax` - Print register in decimal
- `print/t $eax` - Print register in binary
- `x/10x $esp` - Examine 10 words at stack pointer (hex)
- `x/10i $eip` - Disassemble 10 instructions at instruction pointer
- `info frame` - Info about current stack frame
- `info locals` - Print local variables

## Memory Inspection
- `x/10x 0x12345678` - Examine 10 words at address (hex)
- `x/10i 0x12345678` - Disassemble 10 instructions at address
- `x/s 0x12345678` - Print string at address
- `x/b 0x12345678` - Examine byte at address
- `x/w 0x12345678` - Examine word (4 bytes) at address

## OS-Specific
- `info gdt` - Show Global Descriptor Table
- `info idt` - Show Interrupt Descriptor Table
- `info tss` - Show Task State Segment
- `info mem` - Show memory regions
- `info all-registers` - Show all CPU registers

## Miscellaneous
- `set pagination off` - Disable paging (useful for large outputs)
- `set disassembly-flavor intel` - Use Intel syntax for disassembly
- `layout asm` - Show assembly in a window
- `layout src` - Show source in a window
- `layout split` - Show both source and assembly
- `focus cmd` - Focus on command window