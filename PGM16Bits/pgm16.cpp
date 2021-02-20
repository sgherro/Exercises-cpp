#include "pgm16.h"

using namespace std;

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue) {

	ifstream is(filename, ios::binary);
	string header;
	uint32_t w, h;
	uint8_t val;
	if (!is) return false;

	getline(is, header);
	if (header != "P5") return false;
	while (is.peek() == '#') {
		getline(is, header);
	}
	is >> w >> h >> maxvalue;
	if (w < 1 || h < 1 || maxvalue < 0 || is.get() !='\n') return false;

	img.resize(h , w);
	
	for (size_t r = 0; r < h; r++) {
		for (size_t c = 0; c < w; c++) {
			is.read(reinterpret_cast<char*>(&val), 1);
			if (maxvalue >= 256) {
				uint16_t v = val << 8;
				is.read(reinterpret_cast<char*>(&v), 1);
				img(r, c) = v;
				v = 0;
			}
			else {
				img(r, c) = val;
			}
		}
	}

	return true;
}