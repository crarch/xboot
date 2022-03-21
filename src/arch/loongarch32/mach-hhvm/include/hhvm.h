#include <io.h>
#include <stdint.h>

#define __startup       __attribute__((__section__(".startup.text")))
#define __startup_data  __attribute__((__section__(".startup.data")))
#define STOP_CPU asm(".word 0x80000000")
