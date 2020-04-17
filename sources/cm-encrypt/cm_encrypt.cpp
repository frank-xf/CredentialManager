#include <string>
#include <vector>

#include "../third/encrypt/rc4.h"
#include "../third/encrypt/base64.h"
#include "../third/encrypt/sha256.h"
#include "../third/encrypt/aes256.h"

#include "cm_encrypt.h"

namespace xf::credential
{
    using array_t = std::vector<std::uint8_t>;

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

    inline array_t _random_byte()
    {

    }

    inline array_t _random_byte(std::uint32_t value)
    {

    }

    void _fill_string(array_t& data)
    {

    }

    void _copy_byte(std::uint8_t* k, std::uint32_t x)
    {
        k[0] = ((x >> 24) & 0xff);
        k[1] = ((x >> 16) & 0xff);
        k[2] = ((x >> 8) & 0xff);
        k[3] = (x & 0xff);
    }

    void _make_initial_vector(std::uint8_t (&iv)[16]
                              , const std::uint8_t* k1, std::size_t n1, const std::uint8_t(&s1)[32]
                              , const std::uint8_t* k2, std::size_t n2, const std::uint8_t(&s2)[32])
    {
        _copy_byte(iv + 0,  _hash_seq(k1, n1));
        _copy_byte(iv + 4,  _hash_seq(s1, 32));
        _copy_byte(iv + 8,  _hash_seq(k2, n2));
        _copy_byte(iv + 12, _hash_seq(s2, 32));
    }


    bool Encrypt(std::string& data, const void* key, std::size_t n)
    {
        const std::uint8_t* _key = (const std::uint8_t*)key;

        std::uint8_t _key_signature[32]{ 0 };
        xf::encrypt::sha_256(_key_signature, _key, n);


        auto _rand = _random_byte(0);

        std::uint8_t _rand_signature[32]{ 0 };
        xf::encrypt::sha_256(_rand_signature, _rand.data(), _rand.size());

        auto _full(_rand);
        _full.insert(_full.end(), _key, _key + n);
        std::uint8_t _full_signature[32]{ 0 };
        xf::encrypt::sha_256(_full_signature, _full.data(), _full.size());

        array_t buf(data.size());
        xf::encrypt::rc4(buf.data(), data.c_str(), data.size(), _full.data(), _full.size());
        _fill_string(buf);

        std::uint8_t _data_signature[32]{ 0 };
        xf::encrypt::sha_256(_data_signature, buf.data(), buf.size());
        buf.insert(buf.end(), _data_signature, _data_signature + 32);

        std::uint8_t iv[16]{ 0 };
        _make_initial_vector(iv, _key, n, _key_signature, _rand.data(), _rand.size(), _rand_signature);

        for (std::size_t i = 0; i < 2; ++i)
        {
            encrypt_round(buf, _key_signature,  iv, _key, n);
            encrypt_round(buf, _rand_signature, iv, _rand.data(), _rand.size());
            encrypt_round(buf, _full_signature, iv, _full.data(), _full.size());
        }

        return false;
    }

    bool Decrypt(std::string& data, const void* key, std::size_t n)
    {
        return false;
    }

    bool encrypt_round(array_t& buf, const std::uint8_t(&key)[32], const std::uint8_t(&iv)[16], const std::uint8_t* str, std::size_t n)
    {
        auto fill = _random_byte();
        buf.insert(buf.begin(), fill.begin(), fill.end());

        xf::encrypt::aes_encrypt(buf.data(), buf.size(), key, iv);

        auto fill = _random_byte();
        buf.insert(buf.begin(), fill.begin(), fill.end());

        array_t state(buf.size());
        xf::encrypt::rc4(state.data(), buf.data(), buf.size(), str, n);

        buf = state;
    }

    bool decrypt_round()
    {

    }

    bool ValidateFile(const char* file)
    {
        return false;
    }

}   // namespace xf::credential
