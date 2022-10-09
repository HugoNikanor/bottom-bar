#ifndef SHADER_COMMON_HEADER
#define SHADER_COMMON_HEADER

#include "common.h"

extern unsigned int LINES;
extern unsigned int HEIGHT;
extern unsigned int WIDTH;
extern unsigned int FONT_HEIGHT;
extern unsigned int USABLE_LINES;
extern unsigned int DATA_SIZE;


typedef struct shader shader;

typedef void (*init_t)(shader*);
typedef void (*cleanup_t)(shader*);
typedef void (*run_t)(shader*, byte[4], uint, uint, ulong);

struct shader {
	init_t init;
	cleanup_t cleanup;
	run_t shader;
};

#endif // SHADER_COMMON_HEADER
