#ifndef CRC_H
#define CRC_H

void initCRC();

void resetCRC();

void calcCRC(uint32_t *data, size_t dataSize);

uint32_t getCRC(void);

#endif // CRC_H