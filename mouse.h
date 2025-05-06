#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"




class MouseEventHandler{
    
    int8_t x, y;

    public: 
        MouseEventHandler();
        virtual void onActivate();
        virtual void onMouseDown(uint8_t button);
        virtual void onMouseUp(uint8_t button);
        virtual void onMouseMove(int xOffset, int yOffset);
};


class MouseDriver : public InterruptHandler, public Driver
{
    Port8Bit dataPort; 
    Port8Bit commandPort;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

    MouseEventHandler *handler;

    public: 
        MouseDriver(InterruptManager *interruptManager, MouseEventHandler* handler);
        ~MouseDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
        virtual void Activate();
};




#endif
