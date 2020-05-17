#include <cstdint>
#include <cstddef>

#include "sbox.h"
#include "aes256.h"

namespace xf::encrypt
{
    constexpr std::size_t _KeyExpSize(240);
    constexpr std::size_t _ColumnNumber(4);
    constexpr std::size_t _RoundNumber(14);
    constexpr std::size_t _KeyNumber(8);

    using state_t = std::uint8_t[4][4];

    const std::uint8_t _rcon[]{
        0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
    };

    inline void _make_inverse_sbox(std::uint8_t (&out)[256], const std::uint8_t (&in)[256])
    {
        for (std::size_t i = 0; i < 256; ++i) out[in[i]] = (std::uint8_t)i;
    }

    inline void _shift_left(std::uint8_t (&buf)[4])
    {
        const std::uint8_t x = buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = x;
    }

    inline void _translate_state(state_t& state, const std::uint8_t (&box)[256])
    {
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                state[j][i] = box[state[j][i]];
    }

    inline std::uint8_t _xtime(std::uint8_t x)
    {
        return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
    }

    inline std::uint8_t _multiply(std::uint8_t x, std::uint8_t y)
    {
        return  (((y & 1) * x) ^
                 (((y >> 1) & 1) * _xtime(x)) ^
                 (((y >> 2) & 1) * _xtime(_xtime(x))) ^
                 (((y >> 3) & 1) * _xtime(_xtime(_xtime(x)))) ^
                 (((y >> 4) & 1) * _xtime(_xtime(_xtime(_xtime(x)))))); /* this last call to _xtime() can be omitted */
    }

    inline void add_round_key(state_t& state, const std::uint8_t (&rkey)[_KeyExpSize], std::size_t round)
    {
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                state[i][j] ^= rkey[(round * _ColumnNumber * 4) + (i * _ColumnNumber) + j];
    }

    inline void mix_columns(state_t& state)
    {
        for (std::size_t i = 0; i < 4; ++i)
        {
            std::uint8_t x = state[i][0];
            std::uint8_t y = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3];

            state[i][0] ^= _xtime(state[i][0] ^ state[i][1]) ^ y;
            state[i][1] ^= _xtime(state[i][1] ^ state[i][2]) ^ y;
            state[i][2] ^= _xtime(state[i][2] ^ state[i][3]) ^ y;
            state[i][3] ^= _xtime(state[i][3] ^ x) ^ y;
        }
    }

    inline void inverse_mix_columns(state_t& state)
    {
        for (std::size_t i = 0; i < 4; ++i)
        {
            std::uint8_t a = state[i][0];
            std::uint8_t b = state[i][1];
            std::uint8_t c = state[i][2];
            std::uint8_t d = state[i][3];

            state[i][0] = _multiply(a, 0x0e) ^ _multiply(b, 0x0b) ^ _multiply(c, 0x0d) ^ _multiply(d, 0x09);
            state[i][1] = _multiply(a, 0x09) ^ _multiply(b, 0x0e) ^ _multiply(c, 0x0b) ^ _multiply(d, 0x0d);
            state[i][2] = _multiply(a, 0x0d) ^ _multiply(b, 0x09) ^ _multiply(c, 0x0e) ^ _multiply(d, 0x0b);
            state[i][3] = _multiply(a, 0x0b) ^ _multiply(b, 0x0d) ^ _multiply(c, 0x09) ^ _multiply(d, 0x0e);
        }
    }

    inline void shift_rows(state_t& state)
    {
        std::uint8_t temp = state[0][1];
        state[0][1] = state[1][1];
        state[1][1] = state[2][1];
        state[2][1] = state[3][1];
        state[3][1] = temp;

        temp = state[0][2];
        state[0][2] = state[2][2];
        state[2][2] = temp;

        temp = state[1][2];
        state[1][2] = state[3][2];
        state[3][2] = temp;

        temp = state[0][3];
        state[0][3] = state[3][3];
        state[3][3] = state[2][3];
        state[2][3] = state[1][3];
        state[1][3] = temp;
    }

    inline void inverse_shift_rows(state_t& state)
    {
        std::uint8_t temp = state[3][1];
        state[3][1] = state[2][1];
        state[2][1] = state[1][1];
        state[1][1] = state[0][1];
        state[0][1] = temp;

        temp = state[0][2];
        state[0][2] = state[2][2];
        state[2][2] = temp;

        temp = state[1][2];
        state[1][2] = state[3][2];
        state[3][2] = temp;

        temp = state[0][3];
        state[0][3] = state[1][3];
        state[1][3] = state[2][3];
        state[2][3] = state[3][3];
        state[3][3] = temp;
    }

    inline void cipher(state_t& state, const std::uint8_t (&rkey)[_KeyExpSize], const std::uint8_t (&box)[256])
    {
        add_round_key(state, rkey, 0);

        for (std::size_t round = 1; ; ++round)
        {
            _translate_state(state, box);
            shift_rows(state);

            if (_RoundNumber == round) break;

            mix_columns(state);
            add_round_key(state, rkey, round);
        }

        add_round_key(state, rkey, _RoundNumber);
    }

    inline void inverse_cipher(state_t& state, const std::uint8_t (&rkey)[_KeyExpSize], const std::uint8_t (&box)[256])
    {
        add_round_key(state, rkey, _RoundNumber);

        for (std::size_t round = (_RoundNumber - 1); ; --round)
        {
            inverse_shift_rows(state);
            _translate_state(state, box);
            add_round_key(state, rkey, round);

            if (0 == round) break;

            inverse_mix_columns(state);
        }
    }

    void key_expansion(std::uint8_t(&rkey)[_KeyExpSize], const std::uint8_t (&key)[KeySize], const std::uint8_t (&box)[256])
    {
        memory_copy(rkey, key, KeySize);

        for (std::size_t i = _KeyNumber; i < _ColumnNumber * (_RoundNumber + 1); ++i)
        {
            std::uint8_t temp[4];
            memory_copy(temp, rkey + ((i - 1) << 2), 4);

            if (i % _KeyNumber == 0)
            {
                _shift_left(temp);

                temp[0] = box[temp[0]];
                temp[1] = box[temp[1]];
                temp[2] = box[temp[2]];
                temp[3] = box[temp[3]];

                temp[0] ^= _rcon[i / _KeyNumber];
            }

            if (i % _KeyNumber == 4)
            {
                temp[0] = box[temp[0]];
                temp[1] = box[temp[1]];
                temp[2] = box[temp[2]];
                temp[3] = box[temp[3]];
            }

            std::size_t k = ((i - _KeyNumber) << 2);
            rkey[(i << 2) + 0] = rkey[k + 0] ^ temp[0];
            rkey[(i << 2) + 1] = rkey[k + 1] ^ temp[1];
            rkey[(i << 2) + 2] = rkey[k + 2] ^ temp[2];
            rkey[(i << 2) + 3] = rkey[k + 3] ^ temp[3];
        }
    }

    void aes_encrypt(void* data, std::size_t n, const std::uint8_t (&key)[KeySize], const std::uint8_t (&iv)[BlockSize])
    {
        std::uint8_t sbox[256]{ 0 };
        mix_sbox(sbox, key, KeySize);

        std::uint8_t round_key[_KeyExpSize]{ 0 };
        key_expansion(round_key, key, sbox);

        const std::uint8_t* iv_ptr = iv;
        std::uint8_t* buf = (std::uint8_t*)data;
        for (std::size_t i = 0; i < n; i += BlockSize)
        {
            memory_xor(buf, iv_ptr, BlockSize);
            cipher(*((state_t*)buf), round_key, sbox);
            iv_ptr = buf;
            buf += BlockSize;
        }
    }

    void aes_decrypt(void* data, std::size_t n, const std::uint8_t (&key)[KeySize], const std::uint8_t (&iv)[BlockSize])
    {
        std::uint8_t sbox[256]{ 0 };
        mix_sbox(sbox, key, KeySize);

        std::uint8_t rsbox[256]{ 0 };
        _make_inverse_sbox(rsbox, sbox);

        std::uint8_t round_key[_KeyExpSize]{ 0 };
        key_expansion(round_key, key, sbox);

        std::uint8_t _iv[BlockSize]{ 0 };
        memory_copy(_iv, iv, BlockSize);

        std::uint8_t storeNextIv[BlockSize]{ 0 };
        std::uint8_t* buf = (std::uint8_t*)data;
        for (std::size_t i = 0; i < n; i += BlockSize)
        {
            memory_copy(storeNextIv, buf, BlockSize);
            inverse_cipher(*((state_t*)buf), round_key, rsbox);
            memory_xor(buf, _iv, BlockSize);
            memory_copy(_iv, storeNextIv, BlockSize);
            buf += BlockSize;
        }
    }

}   // namespace xf::encrypt
