#include <drivers/keyboard.h>

using namespace dysos::common;
using namespace dysos::drivers;
using namespace dysos::hardwarecommunication;


void printf(const char* str);
void printfHex(uint8_t key);

KeyboardEventHandler::KeyboardEventHandler(){}

void KeyboardEventHandler::onKeyDown(char ch){
    char* foo = " ";
    foo[0] = ch;
    printf((const char*)foo);
}

void KeyboardEventHandler::onKeyUp(char ch){
    
}



KeyboardDriver::KeyboardDriver(InterruptManager *interruptManager, KeyboardEventHandler *handler)
:InterruptHandler(interruptManager, 0x21),dataPort(0x60), commandPort(0x64)
{
    this->handler = handler;
}

void KeyboardDriver::Activate(){
    while(commandPort.Read() & 0x1)//cleanup the buffer from previous keystroke or garbage data
        dataPort.Read();
    commandPort.Write(0xAE); //activate interrupts
    commandPort.Write(0x20); //get current state
    uint8_t status = (dataPort.Read() | 1) & ~0x10; //set the bit 1 to 1 and the bit 4 to 0 
    commandPort.Write(0x60); //set state
    dataPort.Write(status);

    dataPort.Write(0xF4); //activate keyboard
}


KeyboardDriver::~KeyboardDriver(){

}


char KeyboardDriver::scancode_to_char(uint8_t scancode) {
    switch (scancode) {
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case 0x10: return 'a';
        case 0x11: return 'z';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1E: return 'q';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x27: return 'm';
        case 0x2C: return 'w';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return ',';
        case 0x33: return ';';
        case 0x34: return ':';
        case 0x35: return '!';
        case 0x39: return ' ';
        case 0x1C: return '\n';
        case 0x0E: return '\b';
        default: return 0;  // 0 = not printable
    }
}


const char* KeyboardDriver::scancode_to_key(uint8_t scancode){

    switch (scancode) {
        // Special keys
        case 0x01: return "ESC";
        case 0x0E: return "BACKSPACE";
        case 0x0F: return "TAB";
        case 0x1C: return "ENTER";
        case 0x1D: return "CTRL";
        case 0x2A: return "LEFT_SHIFT";
        case 0x36: return "RIGHT_SHIFT";
        case 0x38: return "ALT";
        case 0x3A: return "CAPS_LOCK";
        case 0x45: return "NUM_LOCK";
        case 0x46: return "SCROLL_LOCK";

        // Function keys
        case 0x3B: return "F1";
        case 0x3C: return "F2";
        case 0x3D: return "F3";
        case 0x3E: return "F4";
        case 0x3F: return "F5";
        case 0x40: return "F6";
        case 0x41: return "F7";
        case 0x42: return "F8";
        case 0x43: return "F9";
        case 0x44: return "F10";
        case 0x57: return "F11";
        case 0x58: return "F12";

        // Numpad keys
        case 0x47: return "NUMPAD_7";
        case 0x48: return "NUMPAD_8";
        case 0x49: return "NUMPAD_9";
        case 0x4A: return "NUMPAD_MINUS";
        case 0x4B: return "NUMPAD_4";
        case 0x4C: return "NUMPAD_5";
        case 0x4D: return "NUMPAD_6";
        case 0x4E: return "NUMPAD_PLUS";
        case 0x4F: return "NUMPAD_1";
        case 0x50: return "NUMPAD_2";
        case 0x51: return "NUMPAD_3";
        case 0x52: return "NUMPAD_0";
        case 0x53: return "NUMPAD_DOT";

        default: return "UNKNOWN";
    }
    
}





uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp){
    uint8_t key = dataPort.Read();

    if(handler == 0)
        return esp;

    if(key < 0x80){
        char ch = scancode_to_char(key);
        if( ch != 0){
            handler->onKeyDown(ch);
        }
        else{
            printf("Key: ");
            printf(scancode_to_key(key));
        }
    }
    
    return esp;
}