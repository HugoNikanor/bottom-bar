#ifndef BATTER_SHADER_HEADER
#define BATTER_SHADER_HEADER

#include "shader_common.h"

extern batteryData batData;

void hsvGradient(byte*, uint, uint, ulong);
void batteryShader(byte*, uint, uint, ulong);

#endif // BATTER_SHADER_HEADER
