#ifndef __DYSOS_HARDWARECOMMUNICATION__PCI_H
#define __DYSOS_HARDWARECOMMUNICATION__PCI_H

#include <hardwareCommunication/port.h>
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwareCommunication/interrupts.h>

using namespace dysos::common;
using namespace dysos::hardwarecommunication;
using namespace dysos::drivers;


namespace dysos{
    namespace hardwarecommunication{

        class PCIDeviceDescriptor{
            public: 
                uint32_t portBase;
                uint32_t interrupt;

                uint16_t bus;
                uint16_t device;
                uint16_t function;

                uint16_t vendor_id;
                uint16_t device_id;
                
                uint8_t class_id;
                uint8_t subclass_id;
                uint8_t interface_id;

                uint8_t revision;

                PCIDeviceDescriptor();
                ~PCIDeviceDescriptor();

        };

        class PCI{

            Port32Bit dataPort; 
            Port32Bit commandPort;

            public: 
                PCI();
                ~PCI();

                uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
                void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
                bool DeviceHasFunctions(uint16_t bus, uint16_t device);
                void SelectDrivers(DriverManager* driverManager);
                PCIDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);

        };
    }
}



#endif