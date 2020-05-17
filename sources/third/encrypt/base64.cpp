#include <cstdint>
#include <cstddef>

namespace xf::encrypt
{
    const char _alphabet[]{
        0x31, 0x35, 0x39, 0x41, 0x45, 0x49, 0x4d, 0x51, 0x55, 0x59, 0x63, 0x67, 0x6b, 0x6f, 0x73, 0x77,
        0x32, 0x36, 0x30, 0x42, 0x46, 0x4a, 0x4e, 0x52, 0x56, 0x5a, 0x64, 0x68, 0x6c, 0x70, 0x74, 0x78,
        0x33, 0x37, 0x2e, 0x43, 0x47, 0x4b, 0x3f, 0x53, 0x57, 0x61, 0x65, 0x69, 0x6d, 0x71, 0x75, 0x79,
        0x34, 0x38, 0x0a, 0x44, 0x48, 0x4c, 0x50, 0x54, 0x58, 0x62, 0x66, 0x6a, 0x6e, 0x72, 0x76, 0x7a
    };

    std::size_t base64_encoding(void* out, const void* in, std::size_t n)
    {
        std::size_t off = n % 3;
        std::size_t count = ((n / 3) << 2);

        if (0 < off)
        {
            count += (off + 1);
            n -= off;
        }

        char* _out = (char*)out;
        std::uint8_t* _in = (std::uint8_t*)in;

        for (std::size_t i = 0; i < n; i += 3, _in += 3, _out += 4)
        {
            _out[0] = _alphabet[(_in[0] & 0xfc) >> 2];
            _out[1] = _alphabet[((_in[0] & 0x03) << 4) | ((_in[1] & 0xf0) >> 4)];
            _out[2] = _alphabet[((_in[1] & 0x0f) << 2) | ((_in[2] & 0xc0) >> 6)];
            _out[3] = _alphabet[_in[2] & 0x3f];
        }

        switch (off)
        {
        case 1:
            _out[0] = _alphabet[(_in[0] & 0xfc) >> 2];
            _out[1] = _alphabet[(_in[0] & 0x03) << 4];
            break;
        case 2:
            _out[0] = _alphabet[(_in[0] & 0xfc) >> 2];
            _out[1] = _alphabet[((_in[0] & 0x03) << 4) | ((_in[1] & 0xf0) >> 4)];
            _out[2] = _alphabet[(_in[1] & 0x0f) << 2];
            break;
        default:
            break;
        }

        return count;
    }

    std::size_t base64_decoding(void* out, const void* in, std::size_t n)
    {
        std::size_t off = n % 4;
        if (1 == off) return 0;     // error

        std::size_t count = (n >> 2) * 3;
        if (0 < off)
        {
            count += (off - 1);
            n -= off;
        }

        std::uint8_t _mapping[256]{ 0 };
        for (std::uint8_t i = 0; i < 64; ++i)
            _mapping[_alphabet[i]] = i;

        std::uint8_t* _out = (std::uint8_t*)out;
        std::uint8_t* _in = (std::uint8_t*)in;

        for (std::size_t i = 0; i < n; i += 4, _in += 4, _out += 3)
        {
            _out[0] = ((_mapping[_in[0]] & 0x3f) << 2) | ((_mapping[_in[1]] & 0x30) >> 4);
            _out[1] = ((_mapping[_in[1]] & 0x0f) << 4) | ((_mapping[_in[2]] & 0x3c) >> 2);
            _out[2] = ((_mapping[_in[2]] & 0x03) << 6) | (_mapping[_in[3]] & 0x3f);
        }

        switch (off)
        {
        case 2:
            _out[0] = ((_mapping[_in[0]] & 0x3f) << 2) | ((_mapping[_in[1]] & 0x30) >> 4);
            break;
        case 3:
            _out[0] = ((_mapping[_in[0]] & 0x3f) << 2) | ((_mapping[_in[1]] & 0x30) >> 4);
            _out[1] = ((_mapping[_in[1]] & 0x0f) << 4) | ((_mapping[_in[2]] & 0x3c) >> 2);
            break;
        default:
            break;
        }

        return count;
    }

}   // namespace xf::encrypt
