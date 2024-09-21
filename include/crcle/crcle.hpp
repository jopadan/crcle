#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <bit>
#include <algorithm>
#include <experimental/simd>

namespace stdx = std::experimental;
using namespace stdx::parallelism_v2;

namespace poly
{
	template<size_t N>
	using type = std::make_unsigned_t<__int_with_sizeof_t<(N>>3)>>;

	template<size_t N>
	constexpr static type<N> rol(type<N> val, const uint8_t d) { return (val << d | (val >> (N - d))); }
	template<size_t N>
	constexpr static type<N> ror(type<N> val, const uint8_t d) { return ((val >> d) | (val << (N - d))); }

	template<size_t N>
	constexpr static type<N> reflect(type<N> val)
	{
		type<N> bits = 0;
		type<N> mask = val;

		for(size_t i = 0; i < N; i++)
		{
			bits <<= 1;
			if(mask & 1)
				bits |= 1;
			mask >>= 1;
		}

		return bits;
	}

	enum flags
	{
		ref_none = 0 << 0,
		ref_in   = 1 << 0,
		ref_out  = 1 << 1,
	};

	const type<32> inverse               = (type<32>)-1;
	const type<32> neutral               = (type<32>) 0;
	const type<32> crc32                 = 0x04C11DB7;
	const type<32> crc32_ieee            = reflect<32>(crc32);
	const type<32> crc32c_castagnolia    = 0x1EDC6F41;
	const type<32> crc32k_koopman_1_3_28 = 0x741B8CD7;
	const type<32> crc32k_koopman_1_1_30 = 0x32583499;
	const type<32> crc32q                = 0x814141AB;
};

template<size_t N, poly::type<N> P, poly::type<N> xor_in = -1, poly::type<N> xor_out = -1, enum poly::flags reflect = poly::ref_none>
struct crc
{
	constexpr static poly::type<N> compute(const uint8_t* buf, size_t len)
	{
		poly::type<N> msb, crc;

		crc = xor_in;
		while(len--)
		{
			crc ^= ((poly::type<N>)((reflect & poly::ref_in) ? poly::reflect<8>(*buf++) : *buf++)) << (N - 8);
			for(int i=0;i<8;i++)
			{
				msb = crc>>(N-1);
				crc<<=1;
				crc ^= (0-msb) & P;
			}
		}

		return (reflect & poly::ref_out) ? poly::reflect<N>(crc)^xor_out : crc^xor_out;
	}

	constexpr static poly::type<N> check()
	{
		const char src[10] = "123456789";
		return compute((uint8_t*)src, 9);
	}
};

namespace crc32
{
	using mpeg2 = crc<32, poly::crc32, poly::inverse, poly::neutral, poly::ref_none>;
	using ccitt = crc<32, poly::crc32, poly::inverse, poly::inverse, poly::ref_none>;
};
