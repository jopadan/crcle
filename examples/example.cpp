#include <cstdlib>
#include <cstdio>
#include <crcle/crcle.hpp>

int main(int argc, char** argv)
{
	printf("[CHK] 0x%.8X CRC32_MPEG2 \"123456789\"\n", crc32::mpeg2::compute((const uint8_t*)"123456789", 9));
	printf("[CHK] 0x%.8X CRC32_MPEG2 \"123456789\"\n", crc32::mpeg2::check());
	printf("[CHK] 0x%.8X CRC32_CCITT \"123456789\"\n", crc32::ccitt::compute((const uint8_t*)"123456789", 9));
	printf("[CHK] 0x%.8X CRC32_CCITT \"123456789\"\n", crc32::ccitt::check());
	exit(EXIT_SUCCESS);
}
