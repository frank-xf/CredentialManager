#pragma once

namespace xf::encrypt
{
    void aes_encrypt(void* data, unsigned int length, const unsigned char* key, const unsigned char* iv);
    void aes_decrypt(void* data, unsigned int length, const unsigned char* key, const unsigned char* iv);

}
