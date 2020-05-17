#pragma once

namespace xf::encrypt
{

    constexpr std::size_t BlockSize(16);
    constexpr std::size_t KeySize(32);

    void aes_encrypt(void* data, std::size_t n, const std::uint8_t (&key)[KeySize], const std::uint8_t (&iv)[BlockSize]);
    void aes_decrypt(void* data, std::size_t n, const std::uint8_t (&key)[KeySize], const std::uint8_t (&iv)[BlockSize]);

}
