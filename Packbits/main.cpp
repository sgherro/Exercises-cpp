#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void comrpess(string filename, string output) {

	ifstream is(filename, ios::binary);
	ofstream os(output,ios::binary);
	uint8_t curr, prec = 0, run=1;
	vector<uint8_t> cp;
	bool flag = true;

	while (flag) {
		if (is.read(reinterpret_cast<char*>(&curr), sizeof(uint8_t))) {
			flag = true;
		}
		else {
			flag = false;
		}
		if (curr == prec && flag) {
			run++;
			if (run == 2 && cp.size()>1) {
				uint8_t s = uint8_t(cp.size()) - 1;
				os << s;
				os.write(reinterpret_cast<char*>(cp.data()), sizeof(uint8_t) * cp.size());
				cp.clear();
			}
		}
		else {
			
			if (run > 1) {
				run = 257 - run;
				os << uint8_t(run) << uint8_t(prec);
				run = 1;
				cp.clear();
			}
			else {
				cp.push_back(prec);
			}
		}
		prec = curr;
	}
	os << uint8_t(128);
}

void decompress(string filename, string output) {

	ifstream is(filename, ios::binary);
	ofstream os(output);
	uint8_t val, run, cp;

	while (is.read(reinterpret_cast<char*>(&val), 1)) {

		if (val > 128) {
			run = 257 - val;
			is.read(reinterpret_cast<char*>(&val), 1);
			while (run-- > 0) {
				os << val;
			}
		}
		else if(val < 128){
			cp = val + 1;
			while (cp-- > 0) {
				is.read(reinterpret_cast<char*>(&val), 1);
				os << val;
			}
		}
		else {
			break;
		}
	}
}


int main(int argc, char* argv[]) {

	switch (*argv[1])
	{
	case('c'):
		comrpess("input.txt","output_compress.bin");
		break;
	case('d'):
		decompress("output_compress.bin", "output.txt");
		break;
	default:
		std::cout << "ERRORE";
		break;
	}
	return EXIT_SUCCESS;
}