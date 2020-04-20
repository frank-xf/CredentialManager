#pragma once

namespace xf::encrypt
{

	inline void _swap(std::uint8_t& a, std::uint8_t& b)
	{
		if (a != b)
			a ^= b ^= a ^= b;
	}

	void memory_xor(std::uint8_t* out, const std::uint8_t* in, std::size_t n);
	void memory_copy(std::uint8_t* out, const std::uint8_t* in, std::size_t n);
	
	void mix_sbox(std::uint8_t* sbox, std::size_t len, const void* key, std::size_t n);

	template<std::size_t len>
	void mix_sbox(std::uint8_t (&sbox)[len], const void* key, std::size_t n)
	{
		mix_sbox(sbox, len, key, n);
	}

}
