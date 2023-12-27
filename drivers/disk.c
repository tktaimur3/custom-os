#include "disk.h"
#include "ports.h"
#include "screen.h"
#include "mem.h"

uint8 slavebit;
uint16 temp_buff[256];

void ATA_identify(uint16* buffer)
{
    int status = 0;

    // commands for IDENTIFY
    outb(0x1F6, 0xB0);
    while ((inb(0x1F7)&0x80) == 0x80); // Wait til BSY clears from status port

    outb(0x1F2, 0x0);
    outb(0x1F3, 0x0);
    outb(0x1F4, 0x0);
    outb(0x1F5, 0x0);

    // identify drive
    outb(0x1F7, 0xEC);

    for (int i = 0; i < 15; i++)
        status = inb(0x1F7);

    printf("%d\n", status);

    while ((inb(0x1F7)&0x80) == 0x80); // Wait til BSY clears from status port
    while (inb(0x1F4) != 0x0);
    while (inb(0x1F5) != 0x0);
    printf("write_addrmid and write_addrhi: %d %d\n", inb(0x1F4), inb(0x1F5));
    while ((inb(0x1F7)&0x08) != 0x08); // Wait for DRQ to set
    while ((inb(0x1F7)&0x1) == 0x1); // wait for ERR to clear

    for (int i = 0; i < 256; i++)
    {
        buffer[i] = inw(0x1F0);
    }
}

static void inline ATA_delay()
{
    uint8 status = 0;
    do {
        for (int i = 0; i < 15; i++)
            status = inb(0x1F7);
    } while ((status&0x80) == 0x80);
}

static void ATA28_PIO_read_chunk(uint16* buffer, uint32 lba, uint8 sector)
{
    uint8 status = 0;
    outb(0x1F6, 0xF0 | (slavebit << 4) | (lba >> 24) & 0x0F); // top four bits of LBA are LS 4 bits of MS 8 bits of read_addr
    outb(0x1F2, sector); 
    outb(0x1F3, lba); // first 8 bits of LBA (lowest)
    outb(0x1F4, (unsigned char) (lba >> 8)); // next 8 bits of LBA
    outb(0x1F5, (unsigned char) (lba >> 16)); // next 8 bits of LBA
    
    outb(0x1F7, 0x20); // read command

    // Wait til BSY bit clears   
    while ((inb(0x1F7)&0x80) == 0x80);
    // Wait for DRQ to set
    while ((inb(0x1F7)&0x08) != 0x08);

    // read
    for (int i = 0; i < 256; i++)
    {
        buffer[i] = inw(0x1F0);
    }

    do {
        for (int i = 0; i < 15; i++)
            status = inb(0x1F7);
    } while ((status&0x80) == 0x80);
}

void ATA_PIO_select(uint8 bit)
{
    slavebit = bit;
}

void ATA28_PIO_read(uint8* buffer, uint64 read_addr, uint32 len_bytes)
{
    // divide by 512
    uint32 lba = read_addr >> 9;
    uint8 sector = (read_addr >> 28) & 0xFF;
    uint32 chunks = len_bytes >> 9;
    uint16 index_into_chunk_bytes = read_addr & 0x01FFU;
    uint16 partial_start_chunk_length = 512-index_into_chunk_bytes;

    if (len_bytes <= 512)
    {
        // if requested read crosses into next chunk boundary
        if (index_into_chunk_bytes + len_bytes > 512)
        {
            ATA28_PIO_read_chunk(temp_buff, lba, sector);
            memcpy(buffer, ((uint8*)temp_buff+index_into_chunk_bytes), partial_start_chunk_length);

            ATA28_PIO_read_chunk(temp_buff, lba+1, sector);
            memcpy(buffer+(partial_start_chunk_length), ((uint8*)temp_buff), len_bytes-(partial_start_chunk_length));
        }
        else
        {
            ATA28_PIO_read_chunk(temp_buff, lba, sector);
            memcpy(buffer, ((uint8*)temp_buff+index_into_chunk_bytes), len_bytes);
        }
    }
    else
    {
        // read aligned on 512 byte boundary - easy to read into buffer
        if (index_into_chunk_bytes == 0)
        {
            // TODO: update the sector math to work correctly when length is long enough
            for (int i = 0; i < chunks; i++)
            {
                ATA28_PIO_read_chunk((uint16*)(buffer+512*i), lba+i, sector);
            }

            // length spills into last chunk (hasn't been accounted for in chunks variable)
            if (len_bytes & 0x01FFU > 0)
            {
                ATA28_PIO_read_chunk(temp_buff, lba+chunks, sector);
                memcpy(buffer+(512*(lba+chunks)), ((uint8*)temp_buff), len_bytes & 0x01FFU);
            }
        }
        else
        {
            // may be able to just copy and paste chunks in between the first partial and final partial chunk into buffer if this is >0
            uint32 full_chunks = (len_bytes - partial_start_chunk_length) >> 9;

            // first partial chunk
            ATA28_PIO_read_chunk(temp_buff, lba, sector);
            memcpy(buffer, ((uint8*)temp_buff+index_into_chunk_bytes), partial_start_chunk_length);

            // these chunks can be added exactly where the first partial left off
            if (full_chunks >= 1)
            {
                // TODO: update the sector math to work correctly when length is long enough
                for (int i = 0; i < full_chunks; i++)
                {
                    ATA28_PIO_read_chunk((uint16*)(buffer+(512*i)+partial_start_chunk_length), lba+i+1, sector);
                }
            }

            // last partial chunk - see if it needs to be loaded 
            // last chunk does not exist if the remaining length after the first partial divides evenly by 512
            if (((len_bytes - partial_start_chunk_length) & 0x01FFU) > 0)
            {
                ATA28_PIO_read_chunk(temp_buff, lba+full_chunks+1, sector);
                memcpy(buffer+(512*full_chunks)+partial_start_chunk_length, (uint8*)temp_buff, (len_bytes - partial_start_chunk_length) & 0x01FFU);
            }
        }
    }
}

void ATA28_PIO_write(uint8* buffer, uint64 write_addr, uint32 len_bytes)
{
    uint8 status = 0;

    // we assume our write_addr is 36 bits
    // write_addr: 0hXXXX XXXS SLLL LLLL
    // X is unused, S is sector count bits, L is LBA bits 
    outb(0x1F6, 0xF0 | (slavebit << 4) | (write_addr >> 24) & 0x0F); // top four bits of LBA are LS 4 bits of MS 8 bits of write_addr
    outb(0x1F2, (write_addr >> 28) & 0xFF); // sector count uses MS 8 bits of write_addr
    outb(0x1F3, write_addr); // first 8 bits of LBA (lowest)
    outb(0x1F4, (unsigned char) (write_addr >> 8)); // next 8 bits of LBA
    outb(0x1F5, (unsigned char) (write_addr >> 16)); // next 8 bits of LBA

    outb(0x1F7, 0x30); // write command

    // Wait til BSY bit clears
    while ((inb(0x1F7)&0x80) == 0x80);
    // Wait for DRQ to set
    while ((inb(0x1F7)&0x08) != 0x08); 

    // write
    for (int i = 0; i < 256; i++)
    {
        outw(0x1F0, ('b' << 8) | 'a');
        outb(0x1F7, 0xE7); // flush cache
        // Wait til BSY bit clears
        while ((inb(0x1F7)&0x80) == 0x80);
    }

    do {
        for (int i = 0; i < 15; i++)
            status = inb(0x1F7);
    } while ((status&0x80) == 0x80);
}