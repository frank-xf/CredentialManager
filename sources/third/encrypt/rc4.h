#pragma once

namespace xf::encrypt
{

	unsigned int rc4(void* out, const void* data, unsigned int len, const void* key, unsigned int nkey);

}
