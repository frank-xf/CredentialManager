#pragma once

namespace xf::credential
{

    bool Encoding(std::string& data, const void* key, std::size_t n);
    bool Decoding(std::string& data, const void* key, std::size_t n);

    bool ValidateFile(const char* file);
}   // namespace xf::credential
