#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void load_asc(string filename, vector<uint32_t>& vect, uint32_t& w, uint32_t& h) {

	ifstream is(filename, ios::binary);
	uint32_t i = 0, k = 0;
	string header;
	if (!is) exit(EXIT_FAILURE);

	is >> header;
	if (is.get() != '\n' || header!="P2") exit(EXIT_FAILURE);

	while (is.peek() == '#') {
		getline(is, header);
	}

	is >> w >> h >> i;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	

	while (is >> skipws >> k) {

		vect.push_back(k);
	}
}


void load_bin(string filename, vector<uint8_t>& vect, uint32_t& w, uint32_t& h) {

	ifstream is(filename, ios::binary);
	uint32_t i = 0;
	uint8_t val;
	string header;
	if (!is) exit(EXIT_FAILURE);

	is >> header;
	if (is.get() != '\n' || header != "P5") exit(EXIT_FAILURE);

	while (is.peek() == '#') {
		getline(is, header);
	}

	is >> w >> h;
	if (is.get() != '\n') exit(EXIT_FAILURE);
	is >> i;

	while (is.read(reinterpret_cast<char*>(&val), sizeof(val))) {
		vect.push_back(val);
	}
}


template <typename T>
void save_pgm(string filename, uint32_t w, uint32_t h, vector<T> vect) {

	vector<uint8_t> v;
	uint32_t i = 0;
	ofstream os("output_" + filename, ios::binary);
	if (!os) exit(EXIT_FAILURE);

	
		while (i < vect.size()) {
			v.push_back(uint8_t(vect[i]));
			i++;
		}

	os << "P5" << endl << w << ' ' << h << endl << "255" << endl;
	os.write(reinterpret_cast<char*>(v.data()), v.size() * sizeof(uint8_t));

}

template <typename T>
void flip(vector<T>& vect, uint32_t w, uint32_t h) {

	for (size_t r = 0; r < h/2; r++) {
		for (size_t c = 0; c < w; c++) {
			swap(vect.at(r * w + c), vect.at((h - r - 1) * w + c));
		}
	}
}


void es1() {

	ofstream os("output_1.pgm", ios::binary);
	vector<uint8_t> vect;
	uint32_t i = 0, k = 0;
	if (!os) exit(EXIT_FAILURE);

	os << "P5" << endl << "256" << ' ' << "256" << endl << "255" << endl;
	
	while (i < 256) {
		while (k < 256) {
			vect.push_back(i);
			k++;
		}
		i++; k = 0;
	}

	os.write(reinterpret_cast<char*>(vect.data()), vect.size() * sizeof(uint8_t));

}

void es2(string filename) {

	vector<uint32_t> vect;
	uint32_t w, h;
	load_asc(filename, vect,w,h);

	flip(vect, w, h);

	save_pgm(filename, w, h, vect);
}


void es3(string filename) {

	vector<uint8_t> vect;
	uint32_t w, h;
	load_bin(filename,vect, w, h);

	flip(vect, w, h);
	save_pgm(filename, w, h, vect);


}

int main(int argc, char* argv[]) {

	es1();

	es2("frog_asc.pgm");

	es3("frog_bin.pgm");



	return EXIT_SUCCESS;
}