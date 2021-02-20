#include "pbm.h"

int main(int argc, char* argv[]) {

	BinaryImage img, img1;
	img.ReadFromPBM("im1.pbm");
	img1.ReadFromPBM("im2.pbm");
	Image i = BinaryImageToImage(img1);

	return EXIT_SUCCESS;
}