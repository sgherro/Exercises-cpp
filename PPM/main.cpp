#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

template <typename T>
void load_ppm_bin(string filename, vector<T>& vect_R, vector<T>& vect_G, vector<T>& vect_B, uint32_t& w, uint32_t& h) {

	ifstream is(filename, ios::binary);
	string header; 
	uint32_t maxval;
	uint8_t val;
	is >> header;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	while (is.peek() == '#') {
		getline(is, header);
	}
	is >> w >> h >> maxval;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	while (is.read(reinterpret_cast<char*>(&val), 1)) {
		vect_R.push_back(val);
		is.read(reinterpret_cast<char*>(&val), 1);
		vect_G.push_back(val);
		is.read(reinterpret_cast<char*>(&val), 1);
		vect_B.push_back(val);
	}
}

template <typename T>
void load_ppm_asc(string filename, vector<T>& vect, uint32_t& w, uint32_t& h) {

	ifstream is(filename, ios::binary);
	string header;
	uint32_t maxval;
	uint32_t val;
	is >> header;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	while (is.peek() == '#') {
		getline(is, header);
	}
	is >> w >> h >> maxval;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	while (is  >> val) {
		vect.push_back(val);
		vect.push_back(0);
		vect.push_back(0);
	}
}

template <typename T>
void load_pgm(string filename, vector<T>& vect, uint32_t& w, uint32_t& h) {

	ifstream is(filename, ios::binary);
	string header;
	uint32_t maxval;
	uint8_t val;
	is >> header;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	while (is.peek() == '#') {
		getline(is, header);
	}
	is >> w >> h >> maxval;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	while (is.read(reinterpret_cast<char*>(&val), 1)) {
		vect.push_back(val);
	}
}
template <typename T>
uint8_t saturation(T n) {
	return uint8_t(n);
}


template <typename T>
void save_ppm_pgm(string filename, vector<T>& vect, uint32_t w, uint32_t h, string head) {

	ofstream os(filename, ios::binary);
	uint32_t i = 0;
	vector<uint8_t> v;

	for (const auto& elem : vect) {
		v.push_back(saturation(elem));
	}

	os << head << endl << w << ' ' << h << endl << "255" << endl;
	os.write(reinterpret_cast<char*>(v.data()), v.size() * sizeof(uint8_t));
}

void save_ppm_asc(string filename, vector<uint32_t>& vect, uint32_t w, uint32_t h) {

	ofstream os(filename, ios::binary);
	int i = 0;
	
	os << "P3" << endl << w << ' ' << h << endl << "255" << endl;

	for (size_t r = 0; r < h; r++) {
		for (size_t c = 0; c < w; c++) {
			
			os << vect.at(r * w + c);
			
			if (i < 2) {
				os << ' ';
			}
			else {
				os << "\t";
				i = -1;
			}
			i++;
		}
		os << endl;
	}

}


template <typename T>
void flip(vector<T>& vect, uint32_t w, uint32_t h) {

	for (size_t r = 0; r < h / 2; r++) {
		for (size_t c = 0; c < w; c++) {
			swap(vect.at(r * w + c), vect.at((h - r - 1) * w + c));
		}
	}

}


void es1() {
	uint32_t i = 0,  k=0;
	vector<uint8_t> vect;

	while (i < 256) {
		while (k < 256) {
			vect.push_back(i);
			vect.push_back(0);
			vect.push_back(0);
			k++;
		}
		i++; k = 0;
	}
	save_ppm_pgm("output_1.ppm", vect, 256, 256,"P6");
}


void es2(string filename) {

	vector<uint8_t> v,R,G,B;
	vector<uint32_t> v_32;
	uint32_t w, h, i=0;

	load_ppm_bin(filename, R, G, B, w, h);

	while (i < R.size()) {
		v.push_back(R.at(i));
		v.push_back(G.at(i));
		v.push_back(B.at(i));
		i++;
	}
	flip(v, w*3, h);
	save_ppm_pgm("output_1_flipped.ppm", v, w, h,"P6");

	R.clear();
	G.clear();
	B.clear();
	i = 0;
	load_ppm_asc("frog_asc.ppm", v_32, w, h);
	flip(v_32, w , h);
	save_ppm_asc("output_1_asc_flipped.ppm", v_32, w, h);
}

void es3(string filename) {

	vector<uint8_t> R, G, B;
	uint32_t w, h;

	load_ppm_bin(filename, R, G, B, w, h);

	save_ppm_pgm("output_2_R.pgm", R, w, h,"P5");
	save_ppm_pgm("output_2_G.pgm", G, w, h,"P5");
	save_ppm_pgm("output_2_B.pgm", B, w, h,"P5");
}

void es4(string filename) {

	vector<uint32_t> R, G, B, v;
	uint32_t w, h, i=0;

	load_pgm(filename + "_R.pgm", R, w, h);
	load_pgm(filename + "_G.pgm", G, w, h);
	load_pgm(filename + "_B.pgm", B, w, h);

	while (i < R.size()) {
		v.push_back(R.at(i));
		v.push_back(G.at(i));
		v.push_back(B.at(i));
		i++;
	}
	save_ppm_pgm("output_3_reconstructed.ppm", v, w, h, "P6");
	
}
int main(int argc, char* argv[]) {

	es1();

	es2("parrots.ppm");

	es3("parrots.ppm");

	es4("output_2");

	return EXIT_SUCCESS;
}