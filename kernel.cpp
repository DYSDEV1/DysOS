#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

#define HEIGHT 25 
#define WIDTH 80



void printf(const char* str) {
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0; 
    static uint8_t y = 0; 


    for(uint16_t i = 0; str[i] != '\0'; ++i) {

        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;
        
        default:
            VideoMemory[WIDTH*y+x] = (VideoMemory[WIDTH*y+x] & 0xFF00) | str[i];
            x++;
            break;
        }
        if( x >= WIDTH){
            x = 0;
            y++; 
        }
        if(y >= 25){
            for(y = 0;y < HEIGHT;y++){
                for(x = 0; x < WIDTH; x++){
                    VideoMemory[WIDTH*y+x] = (VideoMemory[WIDTH*y+x] & 0xFF00) | ' ';
                }
            }
            x = 0; 
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf("0x");
    printf(foo);
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors(){
    for(constructor* i= (constructor*) &start_ctors; i != (constructor*) &end_ctors;i++){
        (*i)();
    }
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*magicnumber*/ ){
    
    printf("\n");
    printf("===============================================\n");
    printf("   Welcome to DysOS - The Ultimate 32-bit OS!  \n");
    printf("      An operating system by Dys \n");
    printf("===============================================\n");
    printf("\n");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    KeyboardDriver keyboard(&interrupts);
    interrupts.Activate();


    while(1);
}