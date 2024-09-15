#include <cstring>
#include <filesystem>
#include <crcle/crcle.hpp>

int main(int argc, char** argv)
{
	printf("CRC32_MPEG2: \"123456789\" check: 0x%.8X\n", CRC32_MPEG2.check());
	printf("CRC32_CCITT: \"123456789\" check: 0x%.8X\n", CRC32_CCITT.check());
	exit(EXIT_SUCCESS);
}
