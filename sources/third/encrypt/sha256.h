#pragma once

namespace xf::encrypt
{

    constexpr std::size_t SignatureSize = 0x20;

    void sha_256(std::uint8_t (&signature)[SignatureSize], const void* data, std::size_t n);

}
