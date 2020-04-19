
#include "sbox.h"

namespace xf::encrypt
{

	unsigned int rc4(void* out, const void* data, unsigned int len, const void* key, unsigned int nkey)
	{
		if (out && data && 0 < len)
		{
			unsigned char sbox[256]{ 0 };
			mix_sbox(sbox, key, nkey);

		    unsigned char* _out = (unsigned char*)out;
			const unsigned char* _data = (const unsigned char*)data;
			for (unsigned int i = 0, x = 0, y = 0; i < len; ++i)
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
