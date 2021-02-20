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

int main(int argc, char* argv[]) {

	if (argc <3) return EXIT_FAILURE;
	
	// do something

	return EXIT_SUCCESS;
}
