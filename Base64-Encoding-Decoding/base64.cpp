#include "base64.h"

using namespace std;


std::string base64_decode(const std::string& input) {

	string final_string, curr;
	if (input == "") return "";
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint32_t i = 0;

	while (input.size() > i*4) {
		curr = input.substr(4 * i, 4);
		if (curr.size() == 0) break;
		uint8_t val = 0;
		
		val = (alphabet.find(curr[0]) << 2);
		uint8_t val1 = (alphabet.find(curr[1]) >> 4);
		val = val | val1;
		final_string.push_back(val);
		if (curr[2] != '=') {

			val = (alphabet.find(curr[1]) << 4) | (alphabet.find(curr[2]) >> 2);
			final_string.push_back(val);
			if (curr[3] != '=') {

				val = (alphabet.find(curr[2]) << 6) | (alphabet.find(curr[3]));
				final_string.push_back(val);
			}
		}
		i++;
	}


	return final_string;
}

/*
std::string base64_encode(const std::string& input) {

	string final_string, curr;
	if (input == "") return "";
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint32_t i = 0;
	bool flag = true;

	while (flag) {
		curr = input.substr(3 * i, 3);
		if (curr.size() == 0) break;
		uint8_t val = 0;
		val = curr[0] >> 2;
		final_string.push_back(alphabet[val]);
		if (curr.size() > 1) {
			val = ((curr[0] & 3) << 4);
			uint8_t val1 = (curr[1] >> 4);
			val = val | val1;
			final_string.push_back(alphabet[val]);
			if (curr.size() > 2) {
				val = ((curr[1] & 15) << 2) | (curr[2] >> 6);
				final_string.push_back(alphabet[val]);
				val = (curr[2] & 63);
				final_string.push_back(alphabet[val]);
			}
			else {
				val = ((curr[1] & 15) << 2);
				final_string.push_back(alphabet[val]);
				final_string.push_back('=');
				flag = false;
			}
		}
		else {
			val = ((curr[0] & 3) << 4);
			final_string.push_back(alphabet[val]);
			final_string.push_back('=');
			final_string.push_back('=');
			flag = false;
		}
		i++;
	}


	return final_string;
}

*/