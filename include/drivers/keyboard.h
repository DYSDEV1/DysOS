#ifndef __DYSOS__DRIVERS__KEYBOARD_H
#define __DYSOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/port.h>
#include <drivers/driver.h>

using namespace dysos::common;
using namespace dysos::drivers;
using namespace dysos::hardwarecommunication;

namespace dysos{
    namespace drivers{

        class KeyboardEventHandler{
            public: 
                KeyboardEventHandler();
                virtual void onKeyDown(char);
                virtual void onKeyUp(char);
        };
        
        class KeyboardDriver : public InterruptHandler, public Driver
        {
            Port8Bit dataPort; 
            Port8Bit commandPort;
            KeyboardEventHandler* handler;
        
            public: 
                KeyboardDriver(InterruptManager *interruptManager, KeyboardEventHandler * handler);
                ~KeyboardDriver();
                char scancode_to_char(uint8_t scancode);
                const char* scancode_to_key(uint8_t scancode);
                virtual uint32_t HandleInterrupt(uint32_t esp);
                virtual void Activate();
        };
        
    }
}




#endif