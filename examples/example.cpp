#include <cstdlib>
#include <cstdio>
#include <crcle/crcle.hpp>
#include <immintrin.h>

int main(int argc, char** argv)
{
	exit(crc32::iscsi::check() && crc32::mpeg2::check() && crc32::ccitt::check() ? EXIT_SUCCESS : EXIT_FAILURE);

}
