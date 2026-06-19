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
        }

    return returnInfo;
}

