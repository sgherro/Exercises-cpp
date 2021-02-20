#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

uint8_t sat(double val) {

	if (val < 0.) {
		return 0;
	}
	else if(val > 255.){
		return 255;
	}

	return uint8_t(val);
}

int es1(string filename, string output, uint8_t scale) {

	ifstream is(filename, ios::binary);
	ofstream os_Y(output + "_Y.pgm", ios::binary);
	ofstream os_Cb(output + "_Cb.pgm", ios::binary);
	ofstream os_Cr(output + "_Cr.pgm", ios::binary);
	string header;
	int32_t w, h, t, maxval;
	uint8_t val_R, val_G, val_B;
	double y, cb, cr;
	vector<uint8_t> Y, Cr, Cb, Cb_t, Cr_t;
	if (!is || !os_Y || !os_Cb || !os_Cr) return EXIT_FAILURE;

	getline(is, header);
	if (header != "P6") return EXIT_FAILURE;
	while (is.peek() == '#') {
		getline(is, header);
	}
	is >> w >> h >> maxval;
	is.get();

	while (is.read(reinterpret_cast<char*>(&val_R), 1)) {
		is.read(reinterpret_cast<char*>(&val_G), 1);
		is.read(reinterpret_cast<char*>(&val_B), 1);
		y = 16 + 1. / 256. * (65.738 * val_R + 129.057 * val_G + 25.064 * val_B);
		Y.push_back(sat(y));
		cb = 128 +  1. / 256. * (-37.945 * val_R - 74.494 * val_G + 112.439 * val_B);
		Cr.push_back(sat(cb)); 
		cr = 128 + 1. / 256. * (112.439 * val_R - 94.154 * val_G - 18.285 * val_B);
		Cb.push_back(sat(cr));
	}

	for (size_t r = 0; r < h; r+=scale) {
		for (size_t c = 0; c < w; c+=scale) {
			cb = 0;
			cr = 0;
			for (size_t s_r = 0; s_r < scale; s_r++) {
				for (size_t s_c = 0; s_c < scale; s_c++) {
					cb += Cb.at((r + s_r) * w + c + s_c);
					cr += Cr.at((r + s_r) * w + c + s_c);
				}
			}
			Cb_t.push_back(cb/(double(scale*scale)));
			Cr_t.push_back(cr/(double(scale*scale)));
		}
	}

	os_Y << "P5" << endl << w << ' ' << h << endl << maxval<< endl;
	os_Cb << "P5" << endl << w/scale << ' ' << h/scale << endl << maxval << endl;
	os_Cr << "P5" << endl << w/scale << ' ' << h/scale << endl << maxval << endl;
	os_Y.write(reinterpret_cast<char*>(Y.data()), Y.size() * sizeof(uint8_t));
	os_Cb.write(reinterpret_cast<char*>(Cb_t.data()), Cb_t.size() * sizeof(uint8_t));
	os_Cr.write(reinterpret_cast<char*>(Cr_t.data()), Cr_t.size() * sizeof(uint8_t));

	return EXIT_SUCCESS;
}

vector<uint8_t> load_pgm(string filename, uint32_t &w, uint32_t & h, uint32_t &maxval) {
	vector <uint8_t> data;
	string header;

	ifstream is(filename, ios::binary);

	if (!is) return data;

	getline(is, header);
	if (header != "P5") return data;
	is >> w >> h >> maxval;
	if (is.get() != '\n') return data;
	data.resize(w * h);

	is.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(uint8_t));

	return data;
}

vector<uint8_t> calc_C(vector<uint8_t> C, uint8_t scale, uint32_t w, uint32_t h) {

	vector<uint8_t> val(w * h * scale * scale);
		for (size_t r = 0; r < h * scale - 1 ; r ++) {
			for (size_t c = 0; c < w * scale - 1; c ++) {
				for (size_t r_scale = 0; r_scale < scale; r_scale++) {
					for (size_t c_scale = 0; c_scale < scale; c_scale++) {
						
						val[(r + r_scale) * w * scale + (c + c_scale)] = C[r/scale * (w) + c / scale];
					}
				}
			}
		}
	
	return val;
}


int es2(string filename, string output, uint8_t scale) {

	ofstream os(output, ios::binary);
	vector<uint8_t> RGB,Y, Cb, Cr, Cb_t, Cr_t;
	uint32_t w, h, maxval, i=0;
	double r, g, b;
	uint8_t cb, cr;
	ifstream is(filename + "_Y.pgm", ios::binary);
	if (!os || !is) return EXIT_FAILURE;
	
	Y = load_pgm(filename + "_Y.pgm", w, h, maxval);
	Cb = load_pgm(filename + "_Cb.pgm",w, h, maxval);
	Cr = load_pgm(filename + "_Cr.pgm",w, h, maxval);
	
	// risolto lasciando uguali i numeri, senza implementare l'interpolazione del laboratorio
	Cb_t = calc_C(Cb, scale, w, h);
	Cr_t = calc_C(Cr, scale, w, h);

	for (const auto& y : Y) {

		cb = Cb_t[i];
		cr = Cr_t[i];

		r = 1. / 256. *(298.082 * y + 408.583 * cr) - 222.291;
		g = 1. / 256. * (298.082 * y - 100.291 * cb - 208.120 * cr) + 135.576;
		b = 1. / 256. * (298.082 * y + 516.412 * cb) - 276.836;
		
		RGB.push_back(sat(r));
		RGB.push_back(sat(g));
		RGB.push_back(sat(b));
		i++;
	}

	os << "P6" << endl << w*scale << ' ' << h*scale << endl << maxval << endl;
	os.write(reinterpret_cast<char*>(RGB.data()), RGB.size() * sizeof(uint8_t));

	return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {

	//es1(argv[4], argv[2], atoi(argv[1]));

	es2(argv[2], argv[3], atoi(argv[1]));

	return EXIT_SUCCESS;
}