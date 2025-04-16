#include "keyboard.h"


void printf(const char* str);
void printfHex(uint8_t key);

KeyboardDriver::KeyboardDriver(InterruptManager *interruptManager)
:InterruptHandler(interruptManager, 0x21),dataPort(0x60), commandPort(0x64)
{
    while(commandPort.Read() & 0x1)
        dataPort.Read();
    commandPort.Write(0xAE); //activate interrupts
    commandPort.Write(0x20); //get current state
    uint8_t status = (dataPort.Read() | 1) & ~0x10;
    commandPort.Write(0x60); //set state
    dataPort.Write(status);

    dataPort.Write(0xF4); //activate keyboard
}

KeyboardDriver::~KeyboardDriver(){

}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp){
    uint8_t key = dataPort.Read();
    printf("KEYBOARD: ");
    printfHex(key);
    return esp;
}