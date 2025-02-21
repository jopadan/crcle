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

	enum flags : int
	{
		ref_none = 0 << 0,
		ref_in   = 1 << 0,
		ref_out  = 1 << 1,
	};

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


template<size_t N, poly::type<N> P, poly::type<N> xor_in = -1, poly::type<N> xor_out = -1, int reflect = poly::ref_none, poly::type<N> chk = poly::inverse, const char** model = nullptr>
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

	constexpr static inline const char* name()
	{
		static char str[256] = "";
		if(sprintf(str,"CRC-%zu/%s", N, *model) == -1)
			return nullptr;
		return str;
	}
	constexpr static inline poly::type<N> check_value(const char* str = "123456789")
	{
		return compute((const uint8_t*)str, strlen(str));
	}
	constexpr static inline bool check()
	{
		poly::type<N> check = check_value();
		printf("[CHK] %s %.8X/%.8X/%.8X/%.8X\n", name(), P, chk, check, magic());
		return check == chk;
	}
	constexpr static inline poly::type<N> magic()
	{
		poly::type<N> magic  = xor_out;
		if(reflect & poly::ref_out)
			magic = poly::reflect<N>(magic);
		crc<N, P, 0, 0, reflect, 0> magic_poly;
		magic = magic_poly.compute((const uint8_t*)&magic, N);
		if(reflect & poly::ref_in)
			magic = poly::reflect<N>(magic);
		return magic;
	}
};

namespace model
{
	static const char* arc   = "ARC";
	static const char* zip   = "ZIP";
	static const char* mpeg2 = "MPEG2";
	static const char* ccitt = "CCITT";
	static const char* iscsi = "ISCSI";
};

namespace crc16
{
	using arc = crc<16, poly::crc16, 0, 0, poly::ref_out | poly::ref_in, 0xBB3D, &model::arc>;
};

namespace crc32
{
	using zip   = crc<32, poly::crc32, poly::inverse, poly::inverse, poly::ref_none, 0xFC891918, &model::zip>;
	using mpeg2 = crc<32, poly::crc32, poly::inverse, poly::neutral, poly::ref_none, 0x0376E6E7, &model::mpeg2>;
	using ccitt = crc<32, poly::crc32, poly::inverse, poly::inverse, poly::ref_none, 0xFC891918, &model::ccitt>;
	using iscsi = crc<32, poly::crc32_iscsi, poly::inverse, poly::inverse, poly::ref_out | poly::ref_in, 0xE3069283, &model::iscsi>;
};
