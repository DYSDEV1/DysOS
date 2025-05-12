#include <hardwareCommunication/pci.h>

using namespace dysos::common;
using namespace dysos::hardwarecommunication;

void printf(const char* str);
void printfHex(uint8_t key);

PCIController::PCIController()
: dataPort(0xCFC), commandPort(0xCF8){

}

PCIController::~PCIController(){

}

PCIDeviceDescriptor::PCIDeviceDescriptor(){

}

PCIDeviceDescriptor::~PCIDeviceDescriptor(){}


uint32_t PCIController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset){
    uint32_t id = //6 bit reserved
    0x1 << 31 
    | ((bus & 0xFF) << 16) //8 bit 
    | ((device & 0x1F) << 11)//5 bit 
    | ((function & 0x07) << 8)//3 bit 
    | ((registeroffset & 0xFC)); //6 bit 
    //need to be aligned to 4 bit as it is a 32 bit address
    commandPort.Write(id);
    uint32_t result = dataPort.Read();

    return result >> (8* (registeroffset % 4));

}


void PCIController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value){
    uint32_t id = 
    0x1 << 31
    | ((bus & 0xFF) << 16)
    | ((device & 0x1F) << 11)
    | ((function & 0x07) << 8)
    | ((registeroffset & 0xFC));

    commandPort.Write(id);
    dataPort.Write(value);

}
 
bool PCIController::DeviceHasFunctions(uint16_t bus, uint16_t device){
    return Read(bus, device, 0, 0x0E) & (1<<7);
    
}

BaseAddressRegister PCIController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
    BaseAddressRegister result;

    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBars = 6 - (4*headertype);
    if(bar >= maxBars)
        return result;

    uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;

    uint32_t temp;

    if(result.type == MemoryMapping){
        switch ((bar_value >> 1) & 0x3)
        {
        case 0://32bit mode
        case 1://20bit mode 
        case 2://64bit mode
            break;
        
        default:
            break;
        }
    }else{ //InputOutput
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }
    return result;
}

Driver* PCIController::GetDriver(PCIDeviceDescriptor dev, InterruptManager *interrupts){
    switch(dev.vendor_id){
        case 0x1022: //AMD
            switch(dev.device_id){
                case 0x2000: //am79c973
                    break;
            }
            break;
        case 0x8086: //Intel
            break;

    }
    switch(dev.class_id){
        case 0x03: //graphics
            switch(dev.subclass_id){
                case 0x00: //VGA
                    break;
            }
            break;
    }

    return 0;
}

void PCIController::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts){
    for(int bus = 0; bus < 8; bus++){
        for(int device = 0; device < 32; device++){
            int numFunc = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int func = 0; func < numFunc; func++){
                PCIDeviceDescriptor dev = GetDeviceDescriptor(bus, device, func);
                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) //no more function for this device 
                    continue;

                for(int barNum = 0; barNum < 0;barNum++){
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, func, barNum);
                    if(bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;

                    Driver* driver = GetDriver(dev, interrupts);
                    if(driver != 0)
                        driverManager->AddDriver(driver);
                }

                printf("PCI BUS: ");
                printfHex(bus & 0xFF);

                printf(", DEVICE: ");
                printfHex(device & 0xFF);
                
                printf(", FUNCTION: ");
                printfHex(func & 0xFF);


                printf(" = VENDOR: ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);

                printf(", DEVICE: ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}

PCIDeviceDescriptor PCIController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function){
    PCIDeviceDescriptor result; 

    result.bus = bus;
    result.device = device; 
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3C);

    return result;
}