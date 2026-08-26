#include "storage.h"
#include "mbr.h"
#include "fat32.h"

FilesystemDriver* activeDriver = nullptr;

StorageDevice StorageDevices[32];
uint8_t StorageDeviceAmount = 0;

StorageDevice* selectedStorageDevice;
PartitionInfo* selectedPartition;

static bool fill_partition_infos(StorageDevice &storageDevice) {
    uint8_t buffer[SECTOR_SIZE_BYTES];
    AHCI_READ_DMA_EXT(storageDevice.Port, 0, 1, buffer);
    PartitionInfo partitions[4];

    if (!parse_mbr(buffer, partitions)) return false;

    for (uint8_t i = 0; i < 4; i++) {
        storageDevice.Partitions[i] = partitions[i];
        storageDevice.PartitionCount++;
    }

    return true;
}

static void setup_storage_devices() {
    ReadyPort readyPorts[32];

    find_ready_ports(readyPorts, StorageDeviceAmount);

    for (uint8_t i = 0; i < StorageDeviceAmount; i++) {
        StorageDevices[i].Port = readyPorts[i].PortPointer;
        StorageDevices[i].PortIndex = readyPorts[i].PortIndex;
        StorageDevices[i].Signature = readyPorts[i].PortPointer->SIG;
        StorageDevices[i].SpeedGen = readyPorts[i].PortPointer->SSTS.SPD;

        StorageDevices[i].Ready = false;
        StorageDevices[i].Identified = false;

        fill_partition_infos(StorageDevices[i]);


        const IDENTIFY_Response* DeviceInformations = AHCI_IDENTIFY_DEVICE(StorageDevices[i].Port);
        if (DeviceInformations != nullptr) {
            StorageDevices[i].IdentificationInformation = *DeviceInformations;
            StorageDevices[i].Identified = true;
        }

        StorageDevices[i].Ready = true;
    }
}

static bool search_suitable_storage_partition() {
    for (uint32_t i = 0; i < StorageDeviceAmount; i++) {
        const uint8_t availablePartitions = StorageDevices[i].PartitionCount;

        for (uint8_t j = 0; j < availablePartitions; j++) {
            if (StorageDevices[i].Partitions[j].Type != FilesystemType::FAT32) continue;

            selectedStorageDevice = &StorageDevices[i];
            selectedPartition = &StorageDevices[i].Partitions[j];

            return true;
        }
    }

    return false;
}

bool init_storage() {
    setup_storage_devices();
    if (StorageDeviceAmount == 0) return false;

    if (!search_suitable_storage_partition()) return false;

    switch (selectedPartition->Type) {
        case FilesystemType::FAT32:
            activeDriver = &FAT32_Driver;
            break;
        default:
            return false;
    }

    if (!activeDriver->init(selectedPartition->Start_LBA)) return false;

    return true;
}
