#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>

using namespace std;

class bitwriter {

	ostream& os_;
	uint8_t buffer_, n_ = 0;


	void write_bit(uint32_t u) {

		buffer_ = (buffer_ << 1) | (u & 1);
		if (++n_ == 8) {
			os_.write(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
			n_ = 0;
		}
	}

public:
	bitwriter(std::ostream& os) : os_(os) {}
	~bitwriter() { flush(); }

	void write(uint32_t u , uint8_t num) {
		while (num	--> 0) {
			write_bit(u >> num);
		}
	}

	void flush(uint32_t u = 0) {
		while (n_ > 0) {
			write_bit(u);
		}
	}
};

class bitreader {
	istream& is_;
	uint8_t buffer_, n_ = 0;

	uint32_t read_bit() {
		if (n_ == 0) {
			is_.read(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
			n_ = 8;
		}
		return (buffer_ >> --n_) & 1;
	}

public:

	bitreader(istream & is) : is_(is) {}
	
	void read(uint32_t& u, uint8_t num) {
		u = 0;
		while (num --> 0) {
			u = (u << 1 ) | read_bit();
		}
	}

	explicit operator bool() {
		return !is_.fail();
	}
};

uint32_t numbits(int32_t max, int32_t min) {

	uint32_t range = max - min + 1;
	uint32_t n_bits = 1;
	while (range /= 2) {
		n_bits++;
	}

	return n_bits;
}

struct frequencies {

	uint32_t counter[256] = { 0 };

	frequencies () {}

	void operator()(uint8_t val) {
			++counter[val];	
	}

	double entropy()
	{
		double tot = 0;
		for (size_t i = 0; i < 256; ++i) {
			tot += counter[i];
		}
	
		double h = 0;
		for (size_t i = 0; i < 256; ++i) {
			double p = counter[i];
			if (p > 0) {
				h += p * log2(p);
			}
		}
		return log2(tot) - h / tot;

	}

};

int write_32bit(string filename, string output) {

	 ifstream is(filename);
	 ofstream os(output, ios::binary);
	 if (!is || !os) return EXIT_FAILURE;
	 int32_t val = 0;
	 while (is >> val) {
		 os.write(reinterpret_cast<const char*>(&val), 4);
	 }
	 return EXIT_SUCCESS;
}

 int read_32bit(string filename, string output) {

	 ifstream is(filename, ios::binary);
	 ofstream os(output);
	 if (!is || !os) return EXIT_FAILURE;
	 int32_t val = 0;
	 while (is.read(reinterpret_cast<char*>(&val),4)) {
		 os << val << endl;
	 }
	 return EXIT_SUCCESS;
 }

 int write_int11(string filename, string output) {

	 ifstream is(filename, ios::binary);
	 ofstream os(output);
	 if (!is || !os) return EXIT_FAILURE;
	 int32_t val = 0;
	 bitwriter bw(os);
	 while (is >> val) {
		 bw.write(val, 11);
	 }
	 bw.flush();

	 return EXIT_SUCCESS;
 }

 int read_int11(string filename, string output) {
	 ifstream is(filename, ios::binary);
	 ofstream os(output);
	 if (!is || !os) return EXIT_FAILURE;
	 int32_t val = 0;
	 bitreader br(is);
	 while (br.read(val,11)) {
		 os << val << endl;
	 }

	 return EXIT_SUCCESS;

 }

int es1(string filename, string output) {

	ifstream is(filename, ios::binary);
	ofstream os(output);

	if (!is || !os) return EXIT_FAILURE;
	uint8_t car;
	frequencies f;
	
	while (is.read(reinterpret_cast<char*>(&car),1)) {
		f(car);
	}

	for (uint32_t i = 0; i < 256; i++) {

		if (f.counter[i] > 0) {
			os << hex << setfill('0') << setw(2) << i << '\t' << dec << f.counter[i] << '\n';
		}
	}

	cout << "Entropia: -" <<dec << f.entropy() << endl;


	return EXIT_SUCCESS;
}

int es2(string option, string filename, string output) {


	if (option == "c") {
		//COMPRESS

		list <uint32_t> numbers;
		ifstream is(filename);
		ofstream os(output, ios::binary);
		if (!is || !os) return EXIT_FAILURE;
		int32_t max, min;
		int32_t val = 0;
		bitwriter bw(os);

		while (is >> val) {
			if (numbers.size() == 0) {
				max = val;
				min = val;
			}
			numbers.push_back(val);

			if (val < min) {
				min = val;
			}
			if (max < val) {
				max = val;
			}
		}
		uint8_t range_bit = numbits(max, min);
		os << "BIN!";
		bw.write(min,32);
		bw.write(max,32);
		bw.write(numbers.size(),32);

		for (const auto& x : numbers) {
			uint32_t num = x - min;
			bw.write(num, range_bit);
		}

	}
	else if (option == "d") {
		//DECOMPRESS

		ifstream is(filename, ios::binary);
		ofstream os(output);
		if (!is || !os) return EXIT_FAILURE;
		
		int32_t max=0,  min=0;
		uint32_t tmp = 0;
		uint32_t num_values=0, n = 0;
		bitreader br(is);
		char special[4];
		is.read(reinterpret_cast<char*>(&special), 4);
		//if (special != "BIN!") return EXIT_FAILURE;
		br.read(tmp, 32);
		min = tmp;
		br.read(tmp, 32);
		max = tmp;
		br.read(num_values, 32);
		n = numbits(max, min);

		while (num_values-- > 0) {
			br.read(tmp, n);
			os << int(tmp) + min << endl;
		}

	}
	else {
		cout << "ERRORE! INSERIRE COME COMANDO c OPPURE d, per specificare \n compressione e decompressione rispettivamente";
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {

	if (argc <3) return EXIT_FAILURE;

	es1(argv[1], argv[2]);

	es2(argv[1], argv[2], argv[3]);

	return EXIT_SUCCESS;
}
