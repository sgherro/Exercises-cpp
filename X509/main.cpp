#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {

	ifstream is(argv[1], ios::binary);
	ofstream os("output.txt");
	vector<uint8_t> content;
	uint8_t identifier, num_bytes;
	uint32_t tag, header_length, length;
	if (!is || !os) return EXIT_FAILURE;
	
	while (is.read(reinterpret_cast<char*>(&identifier),1)) {
		uint8_t classe = identifier >> 6;
		uint8_t tipo = (identifier << 2) >> 7;
		
		header_length = 1;
		length = 0;
		tag = identifier & uint8_t(31);
		if (tag == 31) {
			// long tag
			tag = 0;
			is.read(reinterpret_cast<char*>(&identifier),1);
			header_length++;
			while (identifier >> 7) {
				tag += (identifier >> 1);
				is.read(reinterpret_cast<char*>(&identifier), 1);
				header_length++;
			} 
			tag += (identifier >> 1);
		}
		is.read(reinterpret_cast<char*>(&identifier),1);
		header_length++;
		if (identifier >> 7) {
			num_bytes = identifier & uint8_t(127);
			header_length += num_bytes;
			while (num_bytes-- > 0) {
				length = length << 8;
				is.read(reinterpret_cast<char*>(&length), 1);
			}
		}
		else{
			length = identifier & uint8_t(127);
		}
		os << to_string(classe) << ", " << to_string(tipo) << ", " << to_string(tag) << ", " << to_string(header_length) << "+" << to_string(length);
		if (!tipo) {
			content.resize(length);
			is.read(reinterpret_cast<char*>(content.data()), content.size() * sizeof(uint8_t));
		}
		if (tag == 19) {
			os << ": ";
			os.write(reinterpret_cast<char*>(content.data()), content.size() * sizeof(uint8_t));
		}
			os << endl;
	}
	return EXIT_SUCCESS;
}