#pragma once

namespace xf::encrypt
{

	std::size_t rc4(void* out, const void* data, std::size_t len, const void* key, std::size_t nkey);

}
