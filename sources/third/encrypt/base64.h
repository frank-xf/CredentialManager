#pragma once

namespace xf::encrypt
{
    unsigned int base64_encoding(unsigned char* dst, const void* src, std::size_t n);
    unsigned int base64_decoding(unsigned char* dst, const void* src, std::size_t n);

}
