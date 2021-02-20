#include "pbm.h"

using namespace std;

bool BinaryImage::ReadFromPBM(const std::string& filename) {

	ifstream is(filename, ios::binary);
	string header;
	uint32_t w, h, i=0;
	uint8_t val;
	if (!is) return false;

	getline(is, header);
	if (header != "P4") return false;
	while (is.peek() == '#') {
		getline(is, header);
	}
	is >> w >> h;
	if (w < 1 || h < 1 || is.get() !='\n') return false;

	uint32_t rest = w % 8;
	H = h;
	W = w;

	while (is.read(reinterpret_cast<char*>(&val), 1)) {
			ImageData.push_back(val);
	}

	return true;
}

Image BinaryImageToImage(const BinaryImage& bimg) {

	Image img;
	img.H = bimg.H;
	img.W = bimg.W;
	uint8_t val,k = 8, b=8;
	uint8_t rest = bimg.W % 8;
	int W_ceil = (img.W + rest) / 8;

	for (int r = 0; r < img.H; r++) {
		for (int c = 0; c < W_ceil; c++) {
			uint8_t val = bimg.ImageData[c + r * W_ceil];

			while(b-->0){
				uint8_t bits = (val >> b) & 1;
				switch (bits) {
				case 1:
					img.ImageData.push_back(0);
					break;
				case 0:
					img.ImageData.push_back(255);
					break;
				default: break;
				}
				if (b == (8 - rest) && c >= (bimg.W / 8)) {
					b = 0;
				}
			}
			b = 8;
		}
	}

	return img;
}