#include <stdint.h>
#include "string.h"
#include "pci_class_names.h"

PCIClassInfo resolve_class_name(const uint8_t ClassCode, const uint8_t SubClass) {
    PCIClassInfo returnInfo;
    returnInfo.className = "";
    returnInfo.subClassName = "";

    switch (ClassCode) {
        case 0x0:
            returnInfo.className = "Unclassified";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Non-VGA-Compatible Unclassified Device";
                    break;
                case 0x1:
                    returnInfo.subClassName = "VGA-Compatible Unclassified Device";
                    break;
            }
            break;
        case 0x1:
            returnInfo.className = "Mass Storage Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "SCSI Bus Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "IDE Controller";
                    break;
                case 0x2:
                    returnInfo.subClassName = "Floppy Disk Controller";
                    break;
                case 0x3:
                    returnInfo.subClassName = "IPI Bus Controller";
                    break;
                case 0x4:
                    returnInfo.subClassName = "RAID Controller";
                    break;
                case 0x5:
                    returnInfo.subClassName = "ATA Controller";
                    break;
                case 0x6:
                    returnInfo.subClassName = "Serial ATA Controller";
                    break;
                case 0x7:
                    returnInfo.subClassName = "Serial Attached SCSI Controller";
                    break;
                case 0x8:
                    returnInfo.subClassName = "Non-Volatile Memory Controller";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x2:
            returnInfo.className = "Network Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Ethernet Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "Token Ring Controller";
                    break;
                case 0x2:
                    returnInfo.subClassName = "FDDI Controller";
                    break;
                case 0x3:
                    returnInfo.subClassName = "ATM Controller";
                    break;
                case 0x4:
                    returnInfo.subClassName = "ISDN Controller";
                    break;
                case 0x5:
                    returnInfo.subClassName = "WorldFip Controller";
                    break;
                case 0x6:
                    returnInfo.subClassName = "PICMG 2.14 Multi Computing Controller";
                    break;
                case 0x7:
                    returnInfo.subClassName = "Infiniband Controller";
                    break;
                case 0x8:
                    returnInfo.subClassName = "Fabric Controller";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x3:
            returnInfo.className = "Display Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "VGA Compatible Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "XGA Controller";
                    break;
                case 0x2:
                    returnInfo.subClassName = "3D Controller (Not VGA-Compatible)";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x4:
            returnInfo.className = "Multimedia Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Multimedia Video Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "Multimedia Audio Controller";
                    break;
                case 0x2:
                        returnInfo.subClassName = "Computer Telephony Device";
                        break;
                case 0x3:
                    returnInfo.subClassName = "Audio Device";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x5:
            returnInfo.className = "Memory Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "RAM Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "Flash Controller";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x6:
            returnInfo.className = "Multimedia Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Host Bridge";
                    break;
                case 0x1:
                    returnInfo.subClassName = "ISA Bridge";
                    break;
                case 0x2:
                    returnInfo.subClassName = "EISA Bridge";
                    break;
                case 0x3:
                    returnInfo.subClassName = "MCA Bridge";
                    break;
                case 0x4:
                    returnInfo.subClassName = "PCI-to-PCI Bridge";
                    break;
                case 0x5:
                    returnInfo.subClassName = "PCMCIA Bridge";
                    break;
                case 0x6:
                    returnInfo.subClassName = "NuBus Bridge";
                    break;
                case 0x7:
                    returnInfo.subClassName = "CardBus Bridge";
                    break;
                case 0x8:
                    returnInfo.subClassName = "RACEway Bridge";
                    break;
                case 0x9:
                    returnInfo.subClassName = "PCI-to-PCI Bridge";
                    break;
                case 0x0A:
                    returnInfo.subClassName = "InfiniBand-to-PCI Host Bridge";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x7:
            returnInfo.className = "Simple Communication Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Serial Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "Parallel Controller";
                    break;
                case 0x2:
                    returnInfo.subClassName = "Multiport Serial Controller";
                    break;
                case 0x3:
                    returnInfo.subClassName = "Modem";
                    break;
                case 0x4:
                    returnInfo.subClassName = "IEEE 488.1/2 (GPIB) Controller";
                    break;
                case 0x5:
                    returnInfo.subClassName = "Smart Card Controller";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x8:
            returnInfo.className = "Base System Peripheral";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "PIC";
                    break;
                case 0x1:
                    returnInfo.subClassName = "DMA Controller";
                    break;
                case 0x2:
                    returnInfo.subClassName = "Timer";
                    break;
                case 0x3:
                    returnInfo.subClassName = "RTC Controller";
                    break;
                case 0x4:
                    returnInfo.subClassName = "PCI Hot-Plug Controller";
                    break;
                case 0x5:
                    returnInfo.subClassName = "SD Host controller";
                    break;
                case 0x6:
                    returnInfo.subClassName = "IOMMU";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x9:
            returnInfo.className = "Input Device Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Keyboard Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "Digitizer Pen";
                    break;
                case 0x2:
                    returnInfo.subClassName = "Mouse Controller";
                    break;
                case 0x3:
                    returnInfo.subClassName = "Scanner Controller";
                    break;
                case 0x4:
                    returnInfo.subClassName = "Gameport Controller";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0xA:
            returnInfo.className = "Docking Station";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Generic";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0xB:
            returnInfo.className = "Processor";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "386";
                    break;
                case 0x1:
                    returnInfo.subClassName = "486";
                    break;
                case 0x2:
                    returnInfo.subClassName = "Pentium";
                    break;
                case 0x3:
                    returnInfo.subClassName = "Pentium Pro";
                    break;
                case 0x10:
                    returnInfo.subClassName = "Alpha";
                    break;
                case 0x20:
                    returnInfo.subClassName = "PowerPC";
                    break;
                case 0x30:
                    returnInfo.subClassName = "MIPS";
                    break;
                case 0x40:
                    returnInfo.subClassName = "Co-Processor";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0xC:
            returnInfo.className = "Serial Bus Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "FireWire (IEEE 1394) Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "ACCESS Bus Controller";
                    break;
                case 0x2:
                    returnInfo.subClassName = "SSA";
                    break;
                case 0x3:
                    returnInfo.subClassName = "USB Controller";
                    break;
                case 0x4:
                    returnInfo.subClassName = "Fibre Channel";
                    break;
                case 0x5:
                    returnInfo.subClassName = "SMBus Controller";
                    break;
                case 0x6:
                    returnInfo.subClassName = "InfiniBand Controller";
                    break;
                case 0x7:
                    returnInfo.subClassName = "IPMI Interface";
                    break;
                case 0x8:
                    returnInfo.subClassName = "SERCOS Interface (IEC 61491)";
                    break;
                case 0x9:
                    returnInfo.subClassName = "CANbus Controller";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0xD:
            returnInfo.className = "Wireless Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "iRDA Compatible Controller";
                    break;
                case 0x1:
                    returnInfo.subClassName = "Consumer IR Controller";
                    break;
                case 0x10:
                    returnInfo.subClassName = "RF Controller";
                    break;
                case 0x11:
                    returnInfo.subClassName = "Bluetooth Controller";
                    break;
                case 0x12:
                    returnInfo.subClassName = "Broadband Controller";
                    break;
                case 0x20:
                    returnInfo.subClassName = "Ethernet Controller (802.1a)";
                    break;
                case 0x21:
                    returnInfo.subClassName = "Ethernet Controller (802.1b)";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0xE:
            returnInfo.className = "Intelligent Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "I20";
                    break;
            }
            break;
        case 0xF:
            returnInfo.className = "Satellite Communication Controller";
            switch (SubClass) {
                case 0x1:
                    returnInfo.subClassName = "Satellite TV Controller";
                    break;
                case 0x2:
                    returnInfo.subClassName = "Satellite Audio Controller";
                    break;
                case 0x3:
                    returnInfo.subClassName = "Satellite Voice Controller";
                    break;
                case 0x4:
                    returnInfo.subClassName = "Satellite Data Controller";
                    break;
            }
            break;
        case 0x10:
            returnInfo.className = "Encryption Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "Network and Computing Encrpytion/Decryption";
                    break;
                case 0x10:
                    returnInfo.subClassName = "Entertainment Encryption/Decryption";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x11:
            returnInfo.className = "Signal Processing Controller";
            switch (SubClass) {
                case 0x0:
                    returnInfo.subClassName = "DPIO Modules";
                    break;
                case 0x1:
                    returnInfo.subClassName = "Performance Counters";
                    break;
                case 0x10:
                    returnInfo.subClassName = "Communication Synchronizer";
                    break;
                case 0x20:
                    returnInfo.subClassName = "Signal Processing Management";
                    break;
                case 0x80:
                    returnInfo.subClassName = "Other";
                    break;
            }
            break;
        case 0x12:
            returnInfo.className = "Processing Accelerator";
            break;
        case 0x13:
            returnInfo.className = "Non-Essential Instrumentation";
            break;
        // case 0x14 to 0x3F // reserved
        case 0x40:
            returnInfo.className = "Co-Processor";
            break;
        // case 0x41 to 0xFE // reserved
        case 0xFF:
            returnInfo.className = "Unassigned Class (Vendor specific)";
            break;
        }

    return returnInfo;
}
