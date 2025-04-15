#include "port.h"


void operator delete(void* p, unsigned int) {}
void operator delete(void* p) {}


Port::Port(uint16_t portnumber){
    this->portnumber = portnumber;
}
Port::~Port(){}


//PORT8_BIT

Port8Bit::Port8Bit(uint16_t portnumber)
:Port(portnumber){}

Port8Bit::~Port8Bit(){}

void Port8Bit::Write(uint8_t data){
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8_t Port8Bit::Read(){
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}


//POR16_BIT


Port16Bit::Port16Bit(uint16_t portnumber)
:Port(portnumber){}

Port16Bit::~Port16Bit(){}

void Port16Bit::Write(uint16_t data){
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint16_t Port16Bit::Read(){
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

//PORT32_BIT


Port32Bit::Port32Bit(uint16_t portnumber)
:Port(portnumber){}

Port32Bit::~Port32Bit(){}

void Port32Bit::Write(uint32_t data){
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint32_t Port32Bit::Read(){
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}


//PORT8_BIT_SLOW

Port8BitSlow::Port8BitSlow(uint16_t portnumber)
:Port(portnumber){}

Port8BitSlow::~Port8BitSlow(){}

void Port8BitSlow::Write(uint8_t data){
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnumber));
}










