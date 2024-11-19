# crcle
CRC checksum C++ library 

## Status

```cpp
namespace poly
{
	const type<16> crc16                 = 0x8005;
	const type<32> inverse               = (type<32>)-1;
	const type<32> neutral               = (type<32>) 0;
	const type<32> crc32                 = 0x04C11DB7;
	const type<32> crc32_ieee            = reflect<32>(crc32);
	const type<32> crc32_iscsi           = 0x1EDC6F41;
	const type<32> crc32k_koopman_1_3_28 = 0x741B8CD7;
	const type<32> crc32k_koopman_1_1_30 = 0x32583499;
	const type<32> crc32q                = 0x814141AB;
};

namespace crc16
{
	namespace name
	{
		const char* arc = "CRC-16/ARC";
	};
	using arc = crc<16, poly::crc16, 0, 0, poly::ref_out | poly::ref_in, 0xBB3D, &name::arc>;
};
namespace crc32
{
	namespace name
	{
		const char* mpeg2 = "CRC-32/MPEG2";
		const char* ccitt = "CRC-32/CCITT";
		const char* iscsi = "CRC-32/ISCSI";
	};
	using mpeg2 = crc<32, poly::crc32, poly::inverse, poly::neutral, poly::ref_none, 0x0376E6E7, &name::mpeg2>;
	using ccitt = crc<32, poly::crc32, poly::inverse, poly::inverse, poly::ref_none, 0xFC891918, &name::ccitt>;
	using iscsi = crc<32, poly::crc32_iscsi, poly::inverse, poly::inverse, poly::ref_out | poly::ref_in, 0xE3069283, &name::iscsi>;
};
```

## Usage

```cpp
#include <cstdlib>
#include <cstdio>
#include <crcle/crcle.hpp>

int main(int argc, char** argv)
{
	exit(crc16::arc::check() && crc32::zip::check() && crc32::iscsi::check() && crc32::mpeg2::check() && crc32::ccitt::check() ? EXIT_SUCCESS : EXIT_FAILURE);
}
```

## References

- [Cyclic redundancy check](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
- [reveng](https://sourceforge.net/projects/reveng/)
- [crccalc.com](https://crccalc.com/?crc=123456789&method=CRC-32/MPEG-2&datatype=0&outtype=0)
- [Understanding CRC](http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html)
- [A Collection of 32-bit CRC Tables and Algorithms](http://www.mrob.com/pub/comp/crc-all.html)
- [Updated makecrc - macutils](https://github.com/jopadan/macutils)
- [sz - lrzsz](https://www.ohse.de/uwe/software/lrzsz.html)
