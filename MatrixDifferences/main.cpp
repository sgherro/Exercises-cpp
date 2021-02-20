#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

template <typename T>
class freq {

public:

	unordered_map<T, uint32_t> counter;
	uint32_t tot = 0;
	
	freq(){}

	void operator()(const T& num){
		++counter[num];
		++tot;
	}

	double entropy() {
		double s = 0.;

		for (const auto& v : counter) {
			s += v.second * log2(v.second);
		}

		return log2(tot) - s / tot;
	}

};

vector<uint8_t> load_pgm(string filename, uint32_t& w, uint32_t& h) {

	ifstream is(filename, ios::binary);
	vector<uint8_t> ret;
	string head;
	uint8_t val;
	uint32_t maxval;
	if (!is) return ret;

	getline(is, head);
	if (head != "P5") return ret;

	while (is.peek() == '#') {
		getline(is, head);
	}

	is >> w >> h >> maxval;

	if (is.get() != 10) return ret;

	while (is.read(reinterpret_cast<char*>(&val), 1)) {
		ret.push_back(val);
	}

	return ret;
}

int es1(string filename) {

	ofstream os("output_freq.txt");
	uint8_t val;
	uint32_t w, h, i = 0;
	vector<uint8_t> vect;
	freq<uint8_t> f;
	if (!os) return EXIT_FAILURE;;
	vect = load_pgm(filename, w, h);
	for (const auto& elem : vect) {
		f(elem);
	}

	while (i++ < 512) {
		os << dec << i << "\t" << f.counter[i] << endl;
	}

	os << "Entropia: " << f.entropy();

	return EXIT_SUCCESS;
}


int es2(string filename) {


	ofstream os("output_freq_diff.txt");
	ofstream os_img("output.pgm", ios::binary);
	uint8_t val;
	uint32_t w, h;
	int i = -255;
	vector<uint8_t> vect, vect_diff;
	freq<uint8_t> f;
	if (!os) return EXIT_FAILURE;;
	vect = load_pgm(filename, w, h);

	for (size_t r = 0; r < h; r++) {
		for (size_t c = 0; c < w; c++) {
			int diff;
			if (c) {
				diff = vect.at(r*w+c) - vect.at(r * w + c - 1);
			}
			else {
				if (r != 0) {
					diff = vect.at(r * w) - vect.at((r - 1) * w);
				}
				else {
					diff = vect.at(0);
				}
			}
			f(diff);
			diff = diff / 2 + 128;
			vect_diff.push_back(uint8_t(diff));
		}
	}

	os_img << "P5" << endl << w << ' ' << h << endl << "255" << endl;
	os_img.write(reinterpret_cast<char*>(vect_diff.data()), vect_diff.size() * sizeof(uint8_t));


	while (i < 255) {
		os << dec << i << "\t" << f.counter[i] << endl;
		i++;
	}

	os << "Entropia: " << f.entropy();

	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {

	es2("frog_bin.pgm");

	return EXIT_SUCCESS;
}