#pragma once

namespace xf::encrypt
{
    void aes_encrypt(void* data, unsigned int n, const unsigned char (&key)[32], const unsigned char (&iv)[16]);
    void aes_decrypt(void* data, unsigned int n, const unsigned char (&key)[32], const unsigned char (&iv)[16]);

}
