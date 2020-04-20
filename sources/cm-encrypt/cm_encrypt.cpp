#include <random>
#include <sstream>
#include <fstream>
// #include <iostream>

#include "../third/encrypt/rc4.h"
#include "../third/encrypt/base64.h"
#include "../third/encrypt/sha256.h"
#include "../third/encrypt/aes256.h"

#include "cm_encrypt.h"

namespace xf::credential::encrypt
{
    using key_pair = std::pair<memory_t, memory_t>;
    using keys_t = std::vector<key_pair>;

    constexpr std::size_t _RequireSize(0x40);
    constexpr std::size_t _RoundNumber(0x01);

    const char* encrypt_version() { return "1.0.2"; }
    const char* encrypt_name() { return "encrypt for xf::credential"; }

    inline std::uint32_t _hash_seq(const void* data, std::size_t n)
    {
        std::uint32_t _value = 2166136261u;
        const std::uint8_t* _ptr = (const std::uint8_t*)data;

        for (std::size_t i = 0; i < n; ++i)
        {
            _value ^= static_cast<std::uint32_t>(_ptr[i]);
            _value *= 16777619u;
        }

        return (_value);
    }

    inline string_t _base64_encoding(const void* str, std::size_t n)
    {
        memory_t text(((n << 2) / 3) + 3);
        auto len = xf::encrypt::base64_encoding(text.data(), str, n);

        return string_t((const string_t::value_type*)text.data(), len);
    }

    inline memory_t _base64_decoding(const void* str, std::size_t n)
    {
        memory_t data(n);
        data.resize(xf::encrypt::base64_decoding(data.data(), str, n));

        return data;
    }

    inline memory_t _sha_256(const void* data, std::size_t n)
    {
        std::uint8_t s[32]{ 0 };
        xf::encrypt::sha_256(s, data, n);

        return memory_t(s, s + 32);
    }

    inline memory_t _rc4(const void* data, std::size_t len, const void* key, std::size_t n)
    {
        memory_t buf(len);
        xf::encrypt::rc4(buf.data(), data, len, key, n);

        return buf;
    }

    inline void _random_memory(memory_t& data, std::size_t n)
    {
        if (0 < n)
        {
            std::random_device rd;
            std::mt19937 e(rd());
            std::uniform_int_distribution<std::uint32_t> ud(0x00, 0xff);

            for (std::size_t i = 0; i < n; ++i)
                data.push_back((std::uint8_t)ud(e));
        }
    }

    inline memory_t _random_memory(std::size_t n)
    {
        memory_t data;
        _random_memory(data, n);

        return data;
    }

    std::uint32_t _friend_number(const void* key, std::size_t n)
    {
        auto value = _hash_seq(key, n);

        std::uint32_t k = 0;
        if (n < _RequireSize) k = std::uint32_t(_RequireSize - n);

        if (k < 2) return value;

        std::uint32_t half = (k >> 1);
        if (value < half || UINT32_MAX - 32 < value) return value;

        std::uint32_t lower = value - half;
        std::uint32_t upper = value + half;

        std::random_device rd;
        std::mt19937 e(rd());

        if (k < 8)
        {
            std::uniform_int_distribution<std::uint32_t> ud(lower, upper);
            return ud(e);
        }
        else
        {
            std::normal_distribution<double> nd(value, 7);
            std::uint32_t x = (std::uint32_t)std::round(nd(e));

            if (x < lower)
                return value - ((lower - x) % half);

            if (x > upper)
                return value + ((x - upper) % half);

            return x;
        }
    }

    keys_t _make_keys(const key_pair& kp, const void* k1, std::size_t n1, const void* k2, std::size_t n2, std::uint32_t friend_number)
    {
        const std::uint8_t* _k1 = (const std::uint8_t*)k1;
        const std::uint8_t* _k2 = (const std::uint8_t*)k2;

        auto _rand = string_to_memory(std::to_string(friend_number));
        auto _rand_signature = _sha_256(_rand.data(), _rand.size());

        // full key
        memory_t _full(_k1, _k1 + n1);
        _full.insert(_full.end(), _rand.begin(), _rand.end());
        _full.insert(_full.end(), _k2, _k2 + n2);
        auto _full_signature = _sha_256(_full.data(), _full.size());

        return { kp, { _rand, _rand_signature }, { _full, _full_signature } };
    }

    keys_t _make_keys(const void* k1, std::size_t n1, const void* k2, std::size_t n2)
    {
        const std::uint8_t* _k1 = (const std::uint8_t*)k1;
        const std::uint8_t* _k2 = (const std::uint8_t*)k2;

        // original key
        memory_t _key(_k1, _k1 + n1);
        _key.insert(_key.end(), _k2, _k2 + n2);
        auto _key_signature = _sha_256(_key.data(), _key.size());
        auto _friend = _friend_number(_key.data(), _key.size());

        return (_make_keys({ _key, _key_signature }, k1, n1, k2, n2, _friend));
    }

    void _make_initial_vector(std::uint8_t(&iv)[16], const keys_t& keys)
    {
        memory_t fk;
        for (const auto& [k, s] : keys)
        {
            fk.insert(fk.end(), k.begin(), k.end());
            fk.insert(fk.end(), s.begin(), s.end());
        }

        std::uint8_t signatrue[32]{ 0 };
        xf::encrypt::sha_256(signatrue, fk.data(), fk.size());

        for (std::size_t i = 0; i < 16; ++i)
            iv[i] = (signatrue[i << 1] ^ signatrue[(i << 1) + 1]);
    }

    void _encrypt_round(memory_t& buf, const key_pair& kp, const std::uint8_t(&iv)[16])
    {
        auto s1 = _random_memory(16);
        buf.insert(buf.begin(), s1.begin(), s1.end());

        xf::encrypt::aes_encrypt(buf.data(), buf.size(), *(const std::uint8_t(*)[32])kp.second.data(), iv);

        auto s2 = _random_memory(16);
        buf.insert(buf.begin(), s2.begin(), s2.end());

        buf = (_rc4(buf.data(), buf.size(), kp.first.data(), kp.first.size()));
    }

    void _decrypt_round(memory_t& buf, const key_pair& kp, const std::uint8_t(&iv)[16])
    {
        buf = _rc4(buf.data(), buf.size(), kp.first.data(), kp.first.size());
        buf.erase(buf.begin(), buf.begin() + 16);

        xf::encrypt::aes_decrypt(buf.data(), buf.size(), *(const std::uint8_t(*)[32])kp.second.data(), iv);
        buf.erase(buf.begin(), buf.begin() + 16);
    }

    bool _decrypt(memory_t& data, const keys_t& keys)
    {
        // make initial vector 
        std::uint8_t iv[16]{ 0 };
        _make_initial_vector(iv, keys);

        memory_t buf(data);

        // round decrypt
        for (std::size_t i = 0; i < _RoundNumber; ++i)
            for (auto iter = keys.rbegin(); iter != keys.rend(); ++iter)
                _decrypt_round(buf, *iter, iv);

        auto len = buf.size() - 32;
        auto signature = _sha_256(buf.data(), len);

        // compare sha 256 signatrue
        if (compare_memory(signature.data(), signature.size(), buf.data() + len, 32))
        {
            buf.resize(len);
            if (std::size_t n = buf[0]; 0 < n && n < 17)
            {
                data = _rc4(buf.data() + n, buf.size() - n, keys.back().first.data(), keys.back().first.size());
                return true;
            }
        }

        return false;
    }

    memory_t _validate_file(const char* file)
    {
        std::ifstream fin(file);
        if (!fin.is_open())
            return memory_t();

        string_t text;

        // compare encrypt name
        std::getline(fin, text);
        if (!compare_memory(text.c_str(), text.size(), encrypt_name(), std::strlen(encrypt_name())))
            return memory_t();

        // compare encrypt version
        std::getline(fin, text);
        if (!compare_memory(text.c_str(), text.size(), encrypt_version(), std::strlen(encrypt_version())))
            return memory_t();

        // sha256 signatrue
        std::getline(fin, text);

        // read data
        std::ostringstream os;
        os << fin.rdbuf();
        text = os.str();
        auto data = _base64_decoding(text.c_str(), text.size());

        // compare signatrue
        if (!compare_memory(_base64_decoding(text.c_str(), text.size()), _sha_256(data.data(), data.size())))
            return memory_t();

        return data;
    }

    void _fill_string(memory_t& data)
    {
        std::size_t x = 16 - (data.size() % 16);
        memory_t s(1, (std::uint8_t)x);
        _random_memory(s, x - 1);
        
        data.insert(data.begin(), s.begin(), s.end());
    }

    bool Load(const char* file, string_t& str, const void* k1, std::size_t n1, const void* k2, std::size_t n2)
    {
        auto data = _validate_file(file);
        if (data.empty())
            return false;

        if (!Decrypt(data, k1, n1, k2, n2))
            return false;

        str = memory_to_string(data);

        return true;
    }

    bool Save(const char* file, const string_t& str, const void* k1, std::size_t n1, const void* k2, std::size_t n2)
    {
        auto data = string_to_memory(str);
        if (!Encrypt(data, k1, n1, k2, n2))
            return false;

        std::ofstream fout(file);
        if (!fout.is_open())
            return false;

        fout << _base64_encoding(encrypt_name(), std::strlen(encrypt_name())) << std::endl;
        fout << _base64_encoding(encrypt_version(), std::strlen(encrypt_version())) << std::endl;

        auto s = _sha_256(data.data(), data.size());
        fout << _base64_encoding(s.data(), s.size()) << std::endl;
        fout << _base64_encoding(data.data(), data.size()) << std::endl;

        fout.close();

        return true;
    }

    bool Encrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2)
    {
        // make keys
        auto keys = _make_keys(k1, n1, k2, n2);

        // make initial vector 
        std::uint8_t iv[16]{ 0 };
        _make_initial_vector(iv, keys);

        // fill buf
        data = _rc4(data.data(), data.size(), keys.back().first.data(), keys.back().first.size());
        _fill_string(data);

        // add signature
        auto _data_signature = _sha_256(data.data(), data.size());
        data.insert(data.end(), _data_signature.begin(), _data_signature.end());

        // round encrypt
        for (std::size_t i = 0; i < _RoundNumber; ++i)
            for (const auto& kp : keys)
                _encrypt_round(data, kp, iv);

        return true;
    }

    bool Decrypt(memory_t& data, const void* k1, std::size_t n1, const void* k2, std::size_t n2)
    {
        const std::uint8_t* _k1 = (const std::uint8_t*)k1;
        const std::uint8_t* _k2 = (const std::uint8_t*)k2;

        // original key
        memory_t _key(_k1, _k1 + n1);
        _key.insert(_key.end(), _k2, _k2 + n2);
        auto _key_signature = _sha_256(_key.data(), _key.size());

        // rand key from original
        auto value = _hash_seq(_key.data(), _key.size());

        key_pair kp(_key, _key_signature);
        if (_decrypt(data, _make_keys(kp, k1, n1, k2, n2, value)))
            return true;

        std::size_t n = 0;
        if (_key.size() < _RequireSize)
            n = _RequireSize - _key.size();

        for (std::uint32_t i = 1; i < (n >> 1); ++i)
        {
            if (_decrypt(data, _make_keys(kp, k1, n1, k2, n2, value + i)))
                return true;

            if (_decrypt(data, _make_keys(kp, k1, n1, k2, n2, value - i)))
                return true;
        }

        return false;
    }

    bool ValidateFile(const char* file)
    {
        return (!_validate_file(file).empty());
    }

    string_t SignatureText(const std::uint8_t* s, std::size_t n)
    {
        const char _c[]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

        string_t text;
        for (unsigned int i = 0; i < n; ++i)
            text.append(1, _c[s[i] >> 4]).append(1, _c[s[i] & 0x0f]);

        return text;
    }

}   // namespace xf::credential::encrypt
