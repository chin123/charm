#ifndef CRC32_H
#define CRC32_H

#define QUOTIENT  0x04c11db7

extern unsigned int crctab[];

#ifdef __cplusplus
extern "C" {
#endif
  
unsigned int crc32(unsigned char *data, int len);
unsigned int crc32_update(unsigned char *data, int len, unsigned int previous);

#ifdef __cplusplus
}
#endif

#endif /* CRC32_H */
