#pragma once

namespace xf::encrypt
{

    void aes_encrypt(void* data, std::size_t n, const std::uint8_t (&key)[32], const std::uint8_t (&iv)[16]);
    void aes_decrypt(void* data, std::size_t n, const std::uint8_t (&key)[32], const std::uint8_t (&iv)[16]);

}
