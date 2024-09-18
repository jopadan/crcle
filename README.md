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
    printf("[CHK] .8X\n", crc32::mpeg2::compute("123456789", 9));
}
```

