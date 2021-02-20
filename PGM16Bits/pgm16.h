#pragma once
#include <cstdint>
#include <string>
#include "mat.h"
#include <fstream>

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue);