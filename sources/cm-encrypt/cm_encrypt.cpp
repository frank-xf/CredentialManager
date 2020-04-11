#include <string>

#include "cm_encrypt.h"


namespace xf::credential
{

    inline std::size_t _hash_seq(const void* data, std::size_t n)
    {
        std::size_t _value = 2166136261u;
        const unsigned char* ptr = (const unsigned char*)data;

        for (std::size_t i = 0; i < n; ++i)
        {
            _value ^= static_cast<std::size_t>(ptr[i]);
            _value *= 16777619u;
        }

        return (_value);
    }

    bool Encoding(std::string& data, const void* key, std::size_t n)
    {
        return false;
    }

    bool Decoding(std::string& data, const void* key, std::size_t n)
    {
        return false;
    }

    bool ValidateFile(const char* file)
    {
        return false;
    }

}   // namespace xf::credential
