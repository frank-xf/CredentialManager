#ifndef _bnb_RC4_H_
#define _bnb_RC4_H_

namespace bnb
{

	unsigned int RC4Encoding(unsigned char* dst, const unsigned char* src, unsigned int nsrc, const unsigned char* key, unsigned int nkey);

}

#endif  // _bnb_RC4_H_
