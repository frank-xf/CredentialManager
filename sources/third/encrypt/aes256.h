#pragma once

#include <stdint.h>

namespace xf::encrypt
{
    void aes_encrypt(void* data, uint32_t n, const uint8_t (&key)[32], const uint8_t (&iv)[16]);
    void aes_decrypt(void* data, uint32_t n, const uint8_t (&key)[32], const uint8_t (&iv)[16]);

}
