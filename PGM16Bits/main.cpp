#include "pgm16.h"

int main(int argc, char* argv[]) {
	uint16_t maxval;
	mat<uint16_t> m;
	load("frog_bin.pgm", m, maxval);

	return EXIT_SUCCESS;
}