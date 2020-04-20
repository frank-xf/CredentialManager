#pragma once

namespace xf::encrypt
{

    std::size_t base64_encoding(void* out, const void* in, std::size_t n);
    std::size_t base64_decoding(void* out, const void* in, std::size_t n);

}
