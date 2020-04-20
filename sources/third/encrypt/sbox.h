#pragma once

#include <cstdint>
#include <cstddef>

namespace xf::encrypt
{

	inline void _swap(unsigned char& a, unsigned char& b)
	{
		if (a != b)
			a ^= b ^= a ^= b;
	}

	void memory_xor(unsigned char* out, const unsigned char* in, std::size_t n);
	void memory_copy(unsigned char* out, const unsigned char* in, std::size_t n);
	
	void mix_sbox(unsigned char* sbox, std::size_t len, const void* key, std::size_t n);

	template<std::size_t len>
	void mix_sbox(unsigned char(&sbox)[len], const void* key, std::size_t n)
	{
		mix_sbox(sbox, len, key, n);
	}

}
