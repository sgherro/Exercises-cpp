#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

struct bitwriter
{
	ostream& os_;
	uint8_t buffer_;
	uint8_t occ_ = 0;

	bitwriter(ostream& os) : os_(os){}

	void write_bit(uint32_t u) {
		buffer_ = buffer_ << 1 | (u & 1);
		if (++occ_ == 8) {
			os_.write(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
			occ_ = 0;
		}
	}

	void write(uint32_t u, uint8_t n) {
		while (n-- > 0) {
			write_bit(u >> n);
		}
	}

	void flush(uint32_t u = 0) {
		while (occ_ > 0) {
			write_bit(u);
		}
	}
}; 

struct bitreader
{
	istream& is_;
	uint8_t buffer_;
	uint8_t occ_ = 0;

	bitreader(istream& is) : is_(is) {}

	uint8_t read_bit() {
		
		if (occ_ == 0) {
			is_.read(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
			occ_ = 8;
		}
		return (buffer_ >> --occ_) & 1;
	}

	bitreader& read(int32_t& u, uint8_t n) {
		u = 0; // dove inserisco il valore letto 
		while (n-- > 0) {
			u = (u<<1) | read_bit();
		}
		return *this;
	}

	explicit operator bool() {
		return !is_.fail();
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

int main(int argc, char *argv[]) {

	string str;

	if (argc != 3) return EXIT_FAILURE;

	

	return EXIT_SUCCESS;

}