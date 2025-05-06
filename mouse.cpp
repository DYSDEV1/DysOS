#include "mouse.h"

#define HEIGHT 25 
#define WIDTH 80


void printf(const char* str);
void printfHex(uint8_t key);


MouseEventHandler::MouseEventHandler(){
}

void MouseEventHandler::onActivate(){
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    x = 40;
    y = 12;
    VideoMemory[WIDTH*y+x] = (( VideoMemory[WIDTH*y+x] & 0xF000) >> 4) | (( VideoMemory[WIDTH*y+x] & 0x0F00) << 4) | ( VideoMemory[WIDTH*y+x] & 0x00FF);
}

void MouseEventHandler::onMouseDown(uint8_t button){

}

void MouseEventHandler::onMouseUp(uint8_t button){

}


void MouseEventHandler::onMouseMove(int xOffset, int yOffset){
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    VideoMemory[WIDTH*y+x] = (( VideoMemory[WIDTH*y+x] & 0xF000) >> 4) | (( VideoMemory[WIDTH*y+x] & 0x0F00) << 4) | ( VideoMemory[WIDTH*y+x] & 0x00FF);

    x += xOffset;

    if(x < 0 ) x = 0;
    if(x >= WIDTH) x = WIDTH - 1;

    y += yOffset;

    if(y < 0) y = 0;
    if(y >= HEIGHT) y = HEIGHT - 1;

    //reverse color on the cursor
    VideoMemory[WIDTH*y+x] = (( VideoMemory[WIDTH*y+x] & 0xF000) >> 4) | (( VideoMemory[WIDTH*y+x] & 0x0F00) << 4) | ( VideoMemory[WIDTH*y+x] & 0x00FF);
}




MouseDriver::MouseDriver(InterruptManager *interruptManager, MouseEventHandler * handler)
:InterruptHandler(interruptManager, 0x2c),dataPort(0x60), commandPort(0x64)
{
    this->handler = handler;
}


void MouseDriver::Activate(){
    offset = 2;
    buttons = 0;

    if(handler != 0)
        handler->onActivate();
    
    commandPort.Write(0xA8); //enable the mouse 
    commandPort.Write(0x20); //get current state
    uint8_t status = dataPort.Read() | 2;
    commandPort.Write(0x60); //set state
    dataPort.Write(status);

    commandPort.Write(0xD4);
    dataPort.Write(0xF4); //activate
}

MouseDriver::~MouseDriver(){}




uint32_t MouseDriver::HandleInterrupt(uint32_t esp){

    uint8_t status = commandPort.Read();
    if(!(status & 0x20) || (handler == 0) )
        return esp;

    static int8_t x = 40, y = 12;

    buffer[offset] = dataPort.Read();
    offset = (offset + 1) %3;


    if(offset == 0){ 
        if(buffer[1] != 0 || buffer[2] != 0)
            handler->onMouseMove(buffer[1], -buffer[2]);
    }

    return esp;
}