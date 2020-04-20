#pragma once

#include <vector>
#include <string>

namespace xf::credential::encrypt
{
    using memory_t = std::vector<std::uint8_t>;
    using string_t = std::string;

    inline memory_t string_to_memory(const string_t& str) {
        return memory_t(str.begin(), str.end());
    }

    inline string_t memory_to_string(const memory_t& data) {
        return string_t((string_t::value_type*)data.data(), data.size() / sizeof(string_t::value_type));
    }

    inline bool compare_memory(const void* s1, std::size_t n1, const void* s2, std::size_t n2) {
        return (n1 == n2 && 0 == std::memcmp(s1, s2, n1));
    }

    inline bool compare_memory(const memory_t& a, const memory_t& b) {
        return compare_memory(a.data(), a.size(), b.data(), b.size());
    }

    const char* encrypt_version();

    bool Load(const char* file, string_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);
    bool Save(const char* file, const string_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);

    bool Encrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);
    bool Decrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);

    bool ValidateFile(const char* file);

    string_t SignatureText(const void* s, std::size_t n);

    template<std::size_t n>
    inline string_t SignatureText(const std::uint8_t (&s)[n]) {
        return SignatureText(s, n);
    }

}   // namespace xf::credential::encrypt
