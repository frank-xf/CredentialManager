#pragma once

namespace xf::encrypt
{

    void sha_256(char(&out_arr)[0x40], const unsigned char* str, unsigned int length);
    void sha_256(unsigned char(&out_arr)[0x20], const unsigned char* str, unsigned int length);

}
