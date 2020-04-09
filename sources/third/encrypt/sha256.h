#pragma once

namespace xf::encrypt
{

    using byte_type = unsigned char;

    template<unsigned int n>
    inline void signature_text(char(&text)[n << 1], const byte_type(&s)[n])
    {
        const char _c[]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

        for (unsigned int i = 0; i < n; ++i)
        {
            text[i << 1] = _c[s[i] >> 4];
            text[(i << 1) + 1] = _c[s[i] & 0x0f];
        }
    }

    void sha_256(byte_type (&out)[0x20], const void* str, unsigned int n);

}
