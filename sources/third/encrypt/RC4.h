#pragma once

namespace xf::encrypt
{

	unsigned int rc4(unsigned char* dst, const unsigned char* src, unsigned int nsrc, const unsigned char* key, unsigned int nkey);

}

