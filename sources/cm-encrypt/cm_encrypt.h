#pragma once

namespace xf::credential
{

    bool Encrypt(std::string& data, const void* key, std::size_t n);
    bool Decrypt(std::string& data, const void* key, std::size_t n);

    bool ValidateFile(const char* file);
}   // namespace xf::credential
