#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

uint8_t sat(double num) {
	if (num < 0) {
		num = 0;
	}
	else if (num > 255) {
		num = 255;
	}
	return uint8_t(num);
}


void save_pgm(string filename, vector<uint8_t> vect, uint32_t w, uint32_t h){

	ofstream os(filename, ios::binary);
	os << "P5" << endl << w << ' ' << h << endl << "255" << endl;
	os.write(reinterpret_cast<char*>(vect.data()), vect.size() * sizeof(uint8_t));
}

void load_ppm(string filename, vector<uint8_t>& vect_R, vector<uint8_t>& vect_G, vector<uint8_t>& vect_B, uint32_t& w, uint32_t& h) {
	string head;
	ifstream is(filename, ios::binary);
	uint32_t maxval;
	uint8_t val;
	is >> head;;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	if (head != "P6") exit(EXIT_FAILURE);
	while (is.peek() == '#') {
		getline(is, head);
	}
	is >> w >> h >> maxval;
	if (w < 1 || h < 1 || maxval != 255) exit(EXIT_FAILURE);
	if (is.get() != '\n') exit(EXIT_FAILURE);

	while (is.read(reinterpret_cast<char*>(&val), 1)) {
		vect_R.push_back(val);
		is.read(reinterpret_cast<char*>(&val), 1);
		vect_G.push_back(val);
		is.read(reinterpret_cast<char*>(&val), 1);
		vect_B.push_back(val);
	}
}

void save_ppm(string filename, vector<uint8_t> vect, uint32_t w, uint32_t h) {
	ofstream os(filename, ios::binary);
	os << "P6" << endl << w << ' ' << h << endl << "255" << endl;
	os.write(reinterpret_cast<char*>(vect.data()), vect.size() * sizeof(uint8_t));
}

void load_pgm(string filename, vector<uint8_t>& vect, uint32_t& w, uint32_t& h) {
	string head;
	ifstream is(filename, ios::binary);
	uint32_t maxval;
	uint8_t val;
	is >> head;;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	if (head != "P5") exit(EXIT_FAILURE);
	while (is.peek() == '#') {
		getline(is, head);
	}
	is >> w >> h >> maxval;
	if (w < 1 || h < 1 || maxval != 255) exit(EXIT_FAILURE);
	if (is.get() != '\n') exit(EXIT_FAILURE);

	while (is.read(reinterpret_cast<char*>(&val), 1)) {
		vect.push_back(val);
	}
}

vector<uint8_t> scaling(vector<uint8_t> vect, uint32_t scale, uint32_t w, uint32_t h) {

	vector<uint8_t> ret;
	for (size_t r = 0; r < h; r+=scale) {
		for (size_t c = 0; c < w; c+=scale) {
			uint32_t v = 0;
			for (size_t sr = 0; sr < scale; sr++) {
				for (size_t sc = 0; sc < scale; sc++) {
					v += vect.at((r + sr) * w + c + sc);
				}
			}
			ret.push_back(v/pow(scale,2));
		}
	}
	return ret;
}


int es1(string filename, string output, uint32_t scale) {
	vector<uint8_t> vect_R, vect_G, vect_B, vect_Y, vect_Cb, vect_Cr;
	uint32_t w, h, i=0;

	load_ppm(filename, vect_R, vect_G, vect_B, w, h);

	if (vect_R.size() != (w*h)) return EXIT_FAILURE;

	while (i < vect_R.size()) {
		double Y = 16. + (1. / 256.) * (65.738 * vect_R.at(i) + 129.057 * vect_G.at(i) + 25.064 * vect_B.at(i));
		double Cb = 128. + (1. / 256.) * (-37.945 * vect_R.at(i) - 74.494 * vect_G.at(i) + 112.439 * vect_B.at(i));
		double Cr = 128. + (1. / 256.) * (112.439 * vect_R.at(i) - 94.154 * vect_G.at(i) - 18.285 * vect_B.at(i));

		vect_Y.push_back(sat(Y));
		vect_Cb.push_back(sat(Cb));
		vect_Cr.push_back(sat(Cr));
		i++;
	}

		vect_Cb = scaling(vect_Cb, scale, w,h);
		vect_Cr = scaling(vect_Cr, scale, w,h);

	save_pgm(output + "_Y.pgm", vect_Y, w, h);
	save_pgm(output + "_Cb.pgm", vect_Cb, w/scale, h/scale);
	save_pgm(output + "_Cr.pgm", vect_Cr, w/scale, h/scale);
	return EXIT_SUCCESS;
}


vector<uint8_t> rescaling(vector<uint8_t> vect, uint32_t scale, uint32_t w, uint32_t h) {

	vector<uint8_t> ret(w*h*pow(scale,2));
	for (size_t r = 0; r < h; r ++) {
		for (size_t c = 0; c < w; c ++) {
			for (size_t sr = 0; sr < scale; sr++) {
				for (size_t sc = 0; sc < scale; sc++) {
					ret[(r*scale+sr)*w*scale + c*scale +sc] = vect.at(r * w + c);
				}
			}
		}
	}
	return ret;
}


int es2(uint32_t scale, string filename, string output) {

	vector<uint8_t> vect_RGB, vect_Y, vect_Cb, vect_Cr;
	uint32_t w, h, i = 0;

	load_pgm(filename + "_Y.pgm", vect_Y, w, h);
	load_pgm(filename + "_Cb.pgm", vect_Cb, w, h);
	load_pgm(filename + "_Cr.pgm", vect_Cr, w, h);
	
	if (scale == 2) {
		vect_Cb = rescaling(vect_Cb, scale, w, h);
		vect_Cr = rescaling(vect_Cr, scale, w, h);
	}
	
	while (i < vect_Y.size()) {
		double R = (1. / 256.) * (298.02 * vect_Y.at(i) + 408.583 * vect_Cr.at(i)) - 222.291;
		double G =(1. / 256.) * (298.02 * vect_Y.at(i) - 100.291 * vect_Cb.at(i) - 208.12 * vect_Cr.at(i)) + 135.576;
		double B =(1. / 256.) * (298.02 * vect_Y.at(i) + 516.412 * vect_Cb.at(i)) - 276.836;

		vect_RGB.push_back(sat(R));
		vect_RGB.push_back(sat(G));
		vect_RGB.push_back(sat(B));
		i++;
	}
	
	save_ppm(output + "_Y.pgm", vect_RGB, w * scale, h * scale);

	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {

	//es1("parrots.ppm", "primo_scala_1", 1);

	//es1("parrots.ppm", "primo_scala_2", 2);

	es2(1, "primo_scala_1", "secondo_1");

	es2(2, "primo_scala_2", "secondo_2");

	return EXIT_SUCCESS;
}