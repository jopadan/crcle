# crcle
CRC checksum C++ library 

## Status

```cpp
namespace poly
{
	const type<32> inverse               = (type<32>)-1;
	const type<32> neutral               = (type<32>) 0;
	const type<32> crc32                 = 0x04C11DB7;
	const type<32> crc32_ieee            = reflect<32>(crc32);
	const type<32> crc32c_castagnolia    = 0x1EDC6F41;
	const type<32> crc32k_koopman_1_3_28 = 0x741B8CD7;
	const type<32> crc32k_koopman_1_1_30 = 0x32583499;
	const type<32> crc32q                = 0x814141AB;
};

namespace crc32
{
	using mpeg2 = crc<32, poly::crc32, poly::inverse, poly::neutral, poly::ref_none>;
	using ccitt = crc<32, poly::crc32, poly::inverse, poly::inverse, poly::ref_none>;
};
```

## Usage

```cpp
#include <cstdlib>
#include <cstdio>
#include <crcle/crcle.hpp>

int main(int argc, char** argv)
{
	printf("[CHK] 0x%.8X CRC32_MPEG2 \"123456789\"\n",
        crc32::mpeg2::compute((const uint8_t*)"123456789", 9));
	printf("[CHK] 0x%.8X CRC32_MPEG2 \"123456789\"\n",
        crc32::mpeg2::check());
	printf("[CHK] 0x%.8X CRC32_CCITT \"123456789\"\n",
        crc32::ccitt::compute((const uint8_t*)"123456789", 9));
	printf("[CHK] 0x%.8X CRC32_CCITT \"123456789\"\n",
        crc32::ccitt::check());
	exit(EXIT_SUCCESS);
}
```

## References

- [Cyclic redundancy check](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
- [crccalc.com](https://crccalc.com/?crc=123456789&method=CRC-32/MPEG-2&datatype=0&outtype=0)
- [Understanding CRC](http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html)
- [A Collection of 32-bit CRC Tables and Algorithms](http://www.mrob.com/pub/comp/crc-all.html)
- [Updated macutils](https://github.com/jopadan/macutils)
