#include <cstdint>
#include <cstddef>

#include "sbox.h"

namespace xf::encrypt
{

	std::size_t rc4(void* out, const void* data, std::size_t len, const void* key, std::size_t nkey)
	{
		if (out && data && 0 < len)
		{
			std::uint8_t sbox[256]{ 0 };
			mix_sbox(sbox, key, nkey);

			std::uint8_t* _out = (std::uint8_t*)out;
			const std::uint8_t* _data = (const std::uint8_t*)data;
			for (std::size_t i = 0, x = 0, y = 0; i < len; ++i)
			{
				x = ((x + 1) & 0xff);
				y = ((y + sbox[x]) & 0xff);
				_swap(sbox[x], sbox[y]);
				_out[i] = (_data[i] ^ sbox[(sbox[x] + sbox[y]) & 0xff]);
			}

			return len;
		}

		return 0;
	}

}	// namespace xf::encrypt
