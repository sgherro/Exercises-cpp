# PGM 16 bits version Example

Create files pgm16.h and pgm16.cpp that allow to use the following function declaration:

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue);
The definition of template class mat<T> is given in the attached mat.h file.

The function should open the PGM file specified by filename, read its data and store it in the 16 bit per pixel matrix. Moreover it should set maxvalue to the value stored in the file.

The raw PGM file consists of:
<pre>
A magic number to identify the type of file. The magic number of a PGM image is the sequence of two characters P5.
The character '\n', that is the Line Feed (LF), that is the character 10 (0x0A), that is a C new line. Beware that this cannot be the pair "\r\n".
An optional comment identified by a '#' character followed by any sequence of characters and ending with the '\n' character. During reading, it is possible to verify if the '#' character is present, otherwise this field is not present.
The width of the image (W), formatted as a sequence of ASCII characters in decimal.
The character ' ', that is a space.
The height of the image (H), formatted as a sequence of ASCII characters in decimal (i.e. the number of rows).
The character '\n'.
A string indicating the maximum value (maxvalue) that a pixel can take. This string can range from 0 to 65535.
The character '\n'.
A sequence of H rows, in order from top to bottom. Each row consists of W gray levels, in order left to right. Each gray level is a number form 0 to maxvalue. If maxvalue is lower than 256 than each gray level is stored with one byte, otherwise it is stored in big endian with two bytes.
In both cases 0 is black and maxvalue is white.
</pre>
