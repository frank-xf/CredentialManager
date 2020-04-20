#pragma once

namespace xf::encrypt
{
    void sha_256(unsigned char(&signature)[0x20], const void* data, std::size_t n);

}
