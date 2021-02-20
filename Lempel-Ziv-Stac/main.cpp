#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

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


int main(int argc, char* argv[]) {

	ifstream is("input.txt", ios::binary);
	ofstream os("output.lsz", ios::binary);
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

	return EXIT_SUCCESS;
}