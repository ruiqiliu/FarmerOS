#ifndef __SERIAL_H__
#define __SERIAL_H__

void init_serial(void);
void putchar(char);
int printf(const char * format, ...);
#define SERIAL_PORT  0x3F8

#endif
