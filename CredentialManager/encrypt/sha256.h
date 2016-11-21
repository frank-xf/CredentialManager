#ifndef _bnb_SHA_256_H_
#define _bnb_SHA_256_H_

namespace bnb
{

    void SHA_256(char(&out_arr)[0x40], const unsigned char* str, unsigned int length);
    void SHA_256(unsigned char(&out_arr)[0x20], const unsigned char* str, unsigned int length);

}

#endif  // _bnb_SHA_256_H_
