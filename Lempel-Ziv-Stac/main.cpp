#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class bitreader {
	uint8_t buffer_, n_ = 0;
	istream& is_;

	uint32_t read_bit() {
		if (n_ == 0) {
			is_.read(reinterpret_cast<char*>(&buffer_), 1);
			n_ = 8;
		}
		return (buffer_ >> --n_) & 1;
	}
public:
	bitreader(istream& is) : is_(is){}

	void read(uint32_t& u, uint8_t k) {
		u = 0;
		while (k-- > 0) {
			u = (u<<1) | read_bit();
		}
	}
	explicit operator bool() {
		return !is_.fail();
	}
};

class bitwriter {

	ostream& os_;
	uint8_t buffer_, n_ = 0;

	void write_bit(uint32_t u) {
		buffer_ = buffer_ << 1 | (u & 1);
		if (++n_ == 8) {
			os_.write(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
			n_ = 0;
		}
	}

public:
	bitwriter(ostream&  os) : os_(os) {}
	~bitwriter() {
		uint32_t u = 0;
		while (n_-- > 0) {
			write_bit(u);
		}
	}
	void write(uint32_t k, uint8_t n) {
		while (k-- > 0) {
			write_bit(k>> n);
		 }
	}
};

//Decompression

void lzs_decompress(std::istream& is, std::ostream& os) {

	bitreader br(is);
	vector<uint8_t> final;
	uint32_t length, N, val=0, offset=0;
	bool flag = true;

	while (flag) {

		br.read(val, 1);
		if (val) {
			br.read(val, 1);
			if (val) {
				br.read(offset, 7);
			}
			else {
				br.read(offset, 11);
			}
			if (!offset) break;
			
			br.read(val, 2);
			if (val < 3) {
				length = val + 2;
			}
			else {
				br.read(val, 2);
				if (val < 3) {
					length = val + 5;
				}
				else {
					br.read(val, 4);
					N = 1;
					while (val == 15) {
						br.read(val, 4);
						N++;
					}
					length = (N * 15 - 7) + val;
				}
			}
			while(length-- > 0) {
				final.push_back(final.at(final.size()-offset));
			}
		}
		else {
			br.read(val, 8);
			final.push_back(uint8_t(val));
		}
	}
	os.write(reinterpret_cast<char*>(final.data()), final.size() * sizeof(uint8_t));
}

void lzs_compress(std::istream& is, std::ostream& os) {
	char val, prec = 0;
	uint32_t length = 0, offset = 0, lun_length,xxxx;
	bitwriter bw(os);
	string occ, str = "";

	while (is.read(reinterpret_cast<char*>(&val), 1)) {

		str += val;
		
		if (occ.find(str)!=string::npos) {
			length++;
			prec = val;
		}
		else {
			if (length < 1) {			
				bw.write(0, 1);
				bw.write(val, 8);
				length = 0;
				prec = 0;
			}
			else if(length==1){
				bw.write(0, 1);
				bw.write(prec, 8);
				length = 0;
				bw.write(0, 1);
				bw.write(val, 8);
				length = 0;
				prec = 0;
			}
			else {

				offset = occ.find(val) + length;
				bw.write(1, 1);
				if (offset < 128) {
					bw.write(1, 1);
					bw.write(offset, 7);
				}
				else {
					bw.write(0, 1);
					bw.write(offset, 11);
				}
				if (length < 5) {
					bw.write(length - 2, 2);
				}
				else if (length < 8) {
					bw.write(length - 2, 4);
				}
				else {
					lun_length = (length + 7) / 15;
					while (lun_length-- > 0) {
						bw.write(15, 4);
					}
					xxxx = length - (lun_length * 15 - 7);
					bw.write(xxxx,4);
				}
				length = 0;
			}
			occ.append(str);
			if (str.size() > 2) {
				str = str.back();
			}
			else {
				str = "";
			}
		}
	}
	bw.write(384, 9);
}

int main(int argc, char* argv[]) {

	ifstream is("input.txt", ios::binary);
	ofstream os("output.lsz", ios::binary);
	
	//do something
	
	return EXIT_SUCCESS;
}
