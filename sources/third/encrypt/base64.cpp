
#include <cstdint>
#include <cstddef>


namespace xf::encrypt
{
    unsigned int base64_encoding(unsigned char* dst, const void* src, std::size_t n)
    {
        return 0;
    }

    unsigned int base64_decoding(unsigned char* dst, const void* src, std::size_t n)
    {
        return 0;
    }

    /*
    unsigned int base64_encoding(unsigned char* dst, const unsigned char* src, unsigned int n)
    {
        unsigned int off = n % 3;
        unsigned int count = (n / 3) << 2;
        if (0 < off)
        {
            count += (off + 1);
            n -= off;
        }

        if (nullptr != dst && nullptr != src)
        {
            for (unsigned int i = 0; i < n; i += 3, src += 3, dst += 4)
            {
                dst[0] = _alphabet[(src[0] & 0xfc) >> 2];
                dst[1] = _alphabet[((src[0] & 0x03) << 4) | ((src[1] & 0xf0) >> 4)];
                dst[2] = _alphabet[((src[1] & 0x0f) << 2) | ((src[2] & 0xc0) >> 6)];
                dst[3] = _alphabet[src[2] & 0x3f];
            }

            switch (off)
            {
            case 1:
                dst[0] = _alphabet[(src[0] & 0xfc) >> 2];
                dst[1] = _alphabet[(src[0] & 0x03) << 4];
                break;
            case 2:
                dst[0] = _alphabet[(src[0] & 0xfc) >> 2];
                dst[1] = _alphabet[((src[0] & 0x03) << 4) | ((src[1] & 0xf0) >> 4)];
                dst[2] = _alphabet[(src[1] & 0x0f) << 2];
                break;
            default:
                break;
            }
        }

        return count;
    }

    unsigned int base64_decoding(unsigned char* dst, const unsigned char* src, unsigned int n)
    {
        if (0 == n)
            return 0;

        unsigned int off = n % 4;
        if (1 == off)
            return 0;

        unsigned int count = (n >> 2) * 3;
        if (0 < off)
        {
            n -= off;
            count += (off - 1);
        }

        if (nullptr != dst && nullptr != src)
        {
            for (unsigned int i = 0; i < n; i += 4, src += 4, dst += 3)
            {
                dst[0] = ((_mapping[src[0]] & 0x3f) << 2) | ((_mapping[src[1]] & 0x30) >> 4);
                dst[1] = ((_mapping[src[1]] & 0x0f) << 4) | ((_mapping[src[2]] & 0x3c) >> 2);
                dst[2] = ((_mapping[src[2]] & 0x03) << 6) | (_mapping[src[3]] & 0x3f);
            }

            switch (off)
            {
            case 2:
                dst[0] = ((_mapping[src[0]] & 0x3f) << 2) | ((_mapping[src[1]] & 0x30) >> 4);
                break;
            case 3:
                dst[0] = ((_mapping[src[0]] & 0x3f) << 2) | ((_mapping[src[1]] & 0x30) >> 4);
                dst[1] = ((_mapping[src[1]] & 0x0f) << 4) | ((_mapping[src[2]] & 0x3c) >> 2);
                break;
            default:
                break;
            }
        }

        return count;
    }
    */

}
