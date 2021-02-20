#include "base64.h"

using namespace std;

int main(int argc, char* argv[]) {

	ofstream os("output.txt");
	
	os << base64_decode("YWJj");

	return EXIT_SUCCESS;
}