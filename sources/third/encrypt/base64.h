#pragma once

#include <cstdint>

namespace xf::encrypt
{
    unsigned int base64_encoding(unsigned char* dst, const unsigned char* src, unsigned int n);
    unsigned int base64_decoding(unsigned char* dst, const unsigned char* src, unsigned int n);

}
