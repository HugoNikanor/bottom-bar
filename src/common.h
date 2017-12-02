#ifndef COMMON_HEADER
#define COMMON_HEADER

typedef unsigned int uint;
typedef unsigned long ulong;

typedef unsigned char byte;
//typedef u_int8_t byte;

#define B 0
#define G 1
#define R 2

#define seconds * 1e6

typedef struct {
	void (*init)();
	void (*shader)(byte*, uint, uint, ulong);
} shader;

#endif // COMMON_HEADER
