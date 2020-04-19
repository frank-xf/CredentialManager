#pragma once

namespace xf::credential
{
    using memory_t = std::vector<std::uint8_t>;

    const char* encrypt_version();

    bool Load(std::string& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);
    bool Save(std::string& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);

    bool Encrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);
    bool Decrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2);

    bool ValidateFile(const char* file);

    std::string SignatureText(const std::uint8_t* s, std::size_t n);

    template<std::size_t n>
    inline std::string SignatureText(const std::uint8_t (&s)[n])
    {
        return SignatureText(s, n);
    }

}   // namespace xf::credential
