#include <string>
#include <vector>
#include <random>

#include "../third/encrypt/rc4.h"
#include "../third/encrypt/base64.h"
#include "../third/encrypt/sha256.h"
#include "../third/encrypt/aes256.h"

#include "cm_encrypt.h"

namespace xf::credential
{
    using memory_t = std::vector<std::uint8_t>;

    constexpr std::size_t _RequireSize(0x40);

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

    inline memory_t _random_byte()
    {
        return { };
    }

    inline std::uint32_t _friend_number(const void* key, std::size_t n)
    {
        auto value = _hash_seq(key, n);


        std::uint32_t v = 0;
        if (n < _RequireSize) v = _RequireSize - n;

        if (v < 2) return value;

        std::uint32_t half = (v >> 1);
        std::uint32_t lower = value - half;
        std::uint32_t upper = value + half;

        std::random_device rd;
        std::mt19937 e(rd());

        if (v < 8)
        {
            std::uniform_int_distribution<std::uint32_t> ud(lower, upper);
            return ud(e);
        }
        else
        {
            std::normal_distribution<double> nd(value, 7);
            std::uint32_t x = std::lround(nd(e));

            if (x < lower)
                return value - ((lower - x) % half);

            if (x > upper)
                return value + ((x - upper) % half);

            return x;
        }
    }

    inline memory_t _make_random(std::uint32_t x)
    {
        std::string str = std::to_string(x);
        return memory_t(str.begin(), str.end());
    }

    inline void _fill_string(memory_t& data)
    {

    }

    void _copy_byte(std::uint8_t* k, std::uint32_t x)
    {
        k[0] = ((x >> 24) & 0xff);
        k[1] = ((x >> 16) & 0xff);
        k[2] = ((x >> 8) & 0xff);
        k[3] = (x & 0xff);
    }

    inline void _make_initial_vector(std::uint8_t (&iv)[16], std::uint32_t x, std::uint32_t y)
    {

    }

    inline bool encrypt_round(memory_t& buf, const std::uint8_t(&key)[32], const std::uint8_t(&iv)[16], const std::uint8_t* str, std::size_t n)
    {
        auto s1 = _random_byte();
        buf.insert(buf.begin(), s1.begin(), s1.end());

        xf::encrypt::aes_encrypt(buf.data(), buf.size(), key, iv);

        auto s2 = _random_byte();
        buf.insert(buf.begin(), s2.begin(), s2.end());

        memory_t state(buf.size());
        xf::encrypt::rc4(state.data(), buf.data(), buf.size(), str, n);

        buf = state;

        return true;
    }

    inline bool decrypt_round()
    {
        return false;
    }

    bool Encrypt(std::string& data, const void* key, std::size_t n)
    {
        // original key
        const std::uint8_t* _key = (const std::uint8_t*)key;
        std::uint8_t _key_signature[32]{ 0 };
        xf::encrypt::sha_256(_key_signature, _key, n);

        // rand key from original
        auto _friend = _friend_number(key, n);
        auto _rand = _make_random(_friend);
        std::uint8_t _rand_signature[32]{ 0 };
        xf::encrypt::sha_256(_rand_signature, _rand.data(), _rand.size());

        // full key
        auto _full(_rand);
        _full.insert(_full.end(), _key, _key + n);
        std::uint8_t _full_signature[32]{ 0 };
        xf::encrypt::sha_256(_full_signature, _full.data(), _full.size());

        // fill buf
        memory_t buf(data.size());
        xf::encrypt::rc4(buf.data(), data.c_str(), data.size(), _full.data(), _full.size());
        _fill_string(buf);

        // add signature
        std::uint8_t _data_signature[32]{ 0 };
        xf::encrypt::sha_256(_data_signature, buf.data(), buf.size());
        buf.insert(buf.end(), _data_signature, _data_signature + 32);

        // make initial vector 
        std::uint8_t iv[16]{ 0 };
        _make_initial_vector(iv, _friend, std::hash<std::uint32_t>{}(_friend));

        for (std::size_t i = 0; i < 2; ++i)
        {
            encrypt_round(buf, _key_signature, iv, _key, n);
            encrypt_round(buf, _rand_signature, iv, _rand.data(), _rand.size());
            encrypt_round(buf, _full_signature, iv, _full.data(), _full.size());
        }

        return false;
    }

    bool Decrypt(std::string& data, const void* key, std::size_t n)
    {
        return false;
    }

    bool ValidateFile(const char* file)
    {
        return false;
    }

}   // namespace xf::credential
