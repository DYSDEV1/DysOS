#ifndef __DYSOS__DRIVERS__MOUSE_H
#define __DYSOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/port.h>
#include <drivers/driver.h>

using namespace dysos::common;
using namespace dysos::drivers;
using namespace dysos::hardwarecommunication;

namespace dysos{
    namespace drivers
    {
                
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



    } 
    
}



#endif
