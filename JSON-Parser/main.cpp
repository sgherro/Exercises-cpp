#include <cstdlib>
#include <string>
#include <fstream>
#include <map>
#include "ppm.h"


using namespace std;

int es(string filename, string output) {

	ifstream is(filename, ios::binary);
	ofstream os(output, ios::binary);
	uint8_t val;
	uint32_t number;
	string str, key;
	map<string, string> m, mElem;
	vector<map<string, string>> elements;

	uint8_t flag_brackets = 0, flag_elements = 0;
	if (!is || !os) return EXIT_FAILURE;

	while (is >> skipws >> val) {

		switch (val) {

		case '"':
			getline(is, key, '"');
			break;

		case '{':
			flag_brackets++;
			break;

		case ':':

			is >> skipws >> val;
			switch (val) {
			case '[':
				flag_elements++;
				break;

			case ']':
				flag_elements--;
				break;

			case ('"'):
				getline(is, str, '"');
				if (flag_elements) {
					mElem[key] = str;
				}
				else {
					m[key] = str;
				}
				break;
			default:
				is.putback(val);
				is >> number;
				if (flag_elements) {
					mElem[key] = to_string(number);
				}
				else {
					m[key] = to_string(number);
				}
				break;

			}
			break;

		case '}':
			if (flag_elements && flag_brackets > 1)
				elements.push_back(mElem);
			flag_brackets--;
			break;

		}
		if (flag_brackets == 0 && flag_elements == 0)
			break;
	}

	int32_t w, h, x, y, w_elem, h_elem;
	w = stoi(m["width"]);
	h = stoi(m["height"]);
	mat<vec3b> img(w, h);
	val = 0;
	for (auto& v : img) {
		v[0] = 255;
		v[1] = 255;
		v[2] = 255;
	}

	for ( auto& e : elements) {
		w_elem = stoi(e["width"]);
		h_elem = stoi(e["height"]);
		x = stoi(e["x"]);
		y = stoi(e["y"]);
		for (size_t r = 0; r < h_elem; r++) {
			for (size_t c = 0; c < w_elem; c++) {
				img(r + y, c + x)[0] = 80 + val;
				img(r + y, c + x)[1] = 80 - val;
				img(r + y, c + x)[2] = 8 * val;
			}
		}
		val += 20;
	}

	save_ppm(output, img, false);

	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {

	es(argv[1], argv[2]);

	return EXIT_SUCCESS;
}
