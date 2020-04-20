#pragma once

namespace xf::encrypt
{

    void sha_256(std::uint8_t (&signature)[0x20], const void* data, std::size_t n);

}
