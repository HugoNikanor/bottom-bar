#ifndef SHADER_COMMON_HEADER
#define SHADER_COMMON_HEADER

#include "common.h"

typedef struct shader shader;

typedef void (*init_t)(shader*, unsigned int width, unsigned int height);
typedef void (*cleanup_t)(shader*);
typedef void (*run_t)(shader*, byte[4], uint, uint, ulong);

struct shader {
	init_t init;
	cleanup_t cleanup;
	run_t shader;
	unsigned int width, height;
};

#endif // SHADER_COMMON_HEADER
