#ifndef DISK_H
#define DISK_H

#include "typedefs.h"

void ATA_identify (uint16* buffer);

// 28bit PIO read/write
void ATA_PIO_select(uint8 slavebit);
void ATA28_PIO_write(uint8* buffer, uint64 write_addr, uint32 len_bytes);
void ATA28_PIO_read(uint8* buffer, uint64 read_addr, uint32 len_bytes);

// creating the 400ns delay
static void inline ATA_delay();

// basic 512 byte read
static void ATA28_PIO_read_chunk(uint16* buffer, uint32 lba, uint8 sector);

#endif