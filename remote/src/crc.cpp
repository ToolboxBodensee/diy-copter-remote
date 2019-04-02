#include "crc.h"

static const unsigned int tiny_crc32tab[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};


uint32_t __tiny_crc32(const void *data, unsigned int length, uint32_t crc)
{
   const unsigned char *buf = (const unsigned char *)data;
   unsigned int i;

   for (i = 0; i < length; ++i) {
      crc ^= buf[i];
      crc = tiny_crc32tab[crc & 0x0f] ^ (crc >> 4);
      crc = tiny_crc32tab[crc & 0x0f] ^ (crc >> 4);
   }

   return crc;
}
uint32_t tiny_crc32(const void *data, unsigned int length)
{
    // return value suitable for passing in next time, for final value invert it
    return __tiny_crc32(data, length, 42) ^ 0xffffff;
}
