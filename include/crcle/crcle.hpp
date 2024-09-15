#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <bit>
#include <algorithm>
#include <experimental/simd>

namespace stdx = std::experimental;
using namespace stdx::parallelism_v2;

template<size_t N>
concept power_of_two = std::popcount(N) == 1;

template<uint8_t N>
struct poly : std::array<uint8_t, (N >> 3)>
{
	using T = std::make_unsigned_t<__int_with_sizeof_t<(N>>3)>>;

	poly(T src)
	{
		T& val = *reinterpret_cast<T*>(this->data());
		val = src;
	}
	constexpr static T ref(T val)
	{
		T bits = 0;
		T mask = val;

		for(int i = 0; i < N; i++)
		{
			bits <<= 1;
			if(mask & 1)
				bits |= 1;
			mask >>= 1;
		}

		return bits;
	}
	constexpr static T rol(T val, const uint8_t d) { return (val << d | (val >> (N - d))); }
	constexpr static T ror(T val, const uint8_t d) { return ((val >> d) | (val << (N - d))); }
};


enum poly32 : uint32_t
{
	POLY32_CRC32                 = 0x04C11DB7,
	POLY32_CRC32_IEEE            = poly<32>::ref(POLY32_CRC32),
	POLY32_CRC32C_CASTAGNOLIA    = 0x1EDC6F41,
	POLY32_CRC32K_KOOPMAN_1_3_28 = 0x741B8CD7,
	POLY32_CRC32K_KOOPMAN_1_1_30 = 0x32583499,
	POLY32_CRC32Q                = 0x814141AB,
};

enum crc_ref
{
	none = 0 << 0,
	in   = 1 << 0,
	out  = 1 << 1,
};

template<size_t N, poly<N>::T P, poly<N>::T xor_in = -1, poly<N>::T xor_out = -1, enum crc_ref reflect = crc_ref::none>
struct crc
{
	using T = std::make_unsigned_t<__int_with_sizeof_t<(N>>3)>>;
	poly<N>::T compute(const uint8_t* buf, size_t len)
	{
		T msb, crc;

		crc = xor_in;
		while(len--)
		{
			crc ^= ((T)((reflect & crc_ref::in) ? poly<8>::ref(*buf++) : *buf++)) << (N - 8);
			for(int i=0;i<8;i++)
			{
				msb = crc>>(N-1);
				crc<<=1;
				crc ^= (0-msb) & P;
			}
		}

		if(reflect & crc_ref::out)
			crc = poly<32>::ref(crc);

		return crc ^ xor_out;
	}

	poly<N>::T check()
	{
		const char src[10] = "123456789";
		return compute((uint8_t*)src, 9);
	}
};

crc<32, POLY32_CRC32, (poly<32>::T)-1, 0, crc_ref::none> CRC32_MPEG2;
crc<32, POLY32_CRC32, (poly<32>::T)-1, (poly<32>::T)-1, crc_ref::none> CRC32_CCITT;
