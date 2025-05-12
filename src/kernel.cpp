#include <common/types.h>
#include <gdt.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace dysos;
using namespace dysos::common;
using namespace dysos::drivers;
using namespace dysos::hardwarecommunication;

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
        case ' ': 
            x++;
            break; 

        case '\b':
            if(x > 0){
                x--;
                VideoMemory[WIDTH*y+x] = (VideoMemory[WIDTH*y+x] & 0xFF00) | ' ';
            }
            
            break;
        case '\r':
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
    //printf("0x");
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
    printf("   Welcome to DysOS - 32-bit OS!  \n");
    printf("      An operating system by Dys \n");
    printf("===============================================\n");
    printf("\n");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    printf("[i] Initializing Drivers.\n");
    DriverManager DrvManager;

    KeyboardEventHandler kbHandler;
    KeyboardDriver keyboard(&interrupts, &kbHandler);
    DrvManager.AddDriver(&keyboard);
    /*
    MouseEventHandler MsHandler;
    MouseDriver mouse(&interrupts, &MsHandler);
    DrvManager.AddDriver(&mouse);
    */
    PCI PCIController;
    PCIController.SelectDrivers(&DrvManager, &interrupts);


    printf("[i] Activating Drivers. \n");

    DrvManager.ActivateAll();

    printf("[i] Activating Interrupts.\n");

    interrupts.Activate();


    while(1);
}