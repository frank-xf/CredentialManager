#pragma once

#include <vector>
#include <string>

namespace xf::credential::encrypt
{
    using memory_t = std::vector<std::uint8_t>;
    using string_t = std::string;

    inline memory_t string_to_memory(const string_t& str)
    {
        return memory_t(str.begin(), str.end());
    }

    inline string_t memory_to_string(const memory_t& data)
    {
        return string_t((string_t::value_type*)data.data(), data.size() / sizeof(string_t::value_type));
    }

    inline bool compare_memory(const void* s1, std::size_t n1, const void* s2, std::size_t n2)
    {
        if (n1 != n2) return false;

        const std::uint8_t* k1 = (const std::uint8_t*)s1;
        const std::uint8_t* k2 = (const std::uint8_t*)s2;
        for (std::size_t i = 0; i < n1; ++i)
            if (k1[i] != k2[i])
                return false;

        return true;
    }

    inline bool compare_memory(const memory_t& a, const memory_t& b)
    {
        return compare_memory(a.data(), a.size(), b.data(), b.size());
    }

    const char* encrypt_version();

    bool Load(string_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);
    bool Save(string_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);

    bool Encrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);
    bool Decrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);

    bool ValidateFile(const char* file);

    string_t SignatureText(const std::uint8_t* s, std::size_t n);

    template<std::size_t n>
    inline string_t SignatureText(const std::uint8_t (&s)[n])
    {
        return SignatureText(s, n);
    }

}   // namespace xf::credential::encrypt
