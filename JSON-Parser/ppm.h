#ifndef PPM_H
#define PPM_H

#include <string>
#include <array>
#include "mat.h"

using vec3b = std::array<uint8_t, 3>;

bool save_ppm(const std::string& filename, const mat<vec3b>& img, bool ascii = false);
bool load_ppm(const std::string& filename, mat<vec3b>& img);

#endif // PPM_H