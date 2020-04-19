#include "sbox.h"

namespace xf::encrypt
{
    namespace aes
    {
        constexpr std::uint32_t BlockSize(16);
        constexpr std::uint32_t KeySize(32);
        constexpr std::uint32_t KeyExpSize(240);

        constexpr std::uint32_t ColumnNumber(4);
        constexpr std::uint32_t RoundNumber(14);
        constexpr std::uint32_t KeyNumber(8);

        using state_t = std::uint8_t[4][4];

        const std::uint8_t _rcon[] = {
            0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
        };

        inline void _make_inverse_sbox(uint8_t(&out)[256], const std::uint8_t(&in)[256])
        {
            for (uint32_t i = 0; i < 256; ++i) out[in[i]] = i;
        }

        inline void _shift_left(uint8_t(&buf)[4])
        {
            const std::uint8_t x = buf[0];
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = buf[3];
            buf[3] = x;
        }

        inline void _translate_state(state_t& state, const std::uint8_t(box)[256])
        {
            for (uint8_t i = 0; i < 4; ++i)
                for (uint8_t j = 0; j < 4; ++j)
                    state[j][i] = box[state[j][i]];
        }

        inline std::uint8_t _xtime(uint8_t x)
        {
            return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
        }

        inline std::uint8_t _multiply(uint8_t x, std::uint8_t y)
        {
            return (((y & 1) * x) ^
                    (((y >> 1) & 1) * _xtime(x)) ^
                    (((y >> 2) & 1) * _xtime(_xtime(x))) ^
                    (((y >> 3) & 1) * _xtime(_xtime(_xtime(x)))) ^
                    (((y >> 4) & 1) * _xtime(_xtime(_xtime(_xtime(x)))))); /* this last call to _xtime() can be omitted */
        }

        inline void key_expansion(uint8_t(&rkey)[KeyExpSize], const std::uint8_t(&key)[KeySize], const std::uint8_t(&box)[256])
        {
            memory_copy(rkey, key, KeySize);

            for (uint32_t i = KeyNumber; i < ColumnNumber * (RoundNumber + 1); ++i)
            {
                std::uint8_t temp[4];
                memory_copy(temp, rkey + ((i - 1) << 2), 4);

                if (i % KeyNumber == 0)
                {
                    _shift_left(temp);

                    temp[0] = box[temp[0]];
                    temp[1] = box[temp[1]];
                    temp[2] = box[temp[2]];
                    temp[3] = box[temp[3]];

                    temp[0] ^= _rcon[i / KeyNumber];
                }

                if (i % KeyNumber == 4)
                {
                    temp[0] = box[temp[0]];
                    temp[1] = box[temp[1]];
                    temp[2] = box[temp[2]];
                    temp[3] = box[temp[3]];
                }

                std::uint32_t k = ((i - KeyNumber) << 2);
                rkey[(i << 2) + 0] = rkey[k + 0] ^ temp[0];
                rkey[(i << 2) + 1] = rkey[k + 1] ^ temp[1];
                rkey[(i << 2) + 2] = rkey[k + 2] ^ temp[2];
                rkey[(i << 2) + 3] = rkey[k + 3] ^ temp[3];
            }
        }

        inline void add_round_key(state_t& state, const std::uint8_t (&rkey)[KeyExpSize], std::uint8_t round)
        {
            for (uint8_t i = 0; i < 4; ++i)
                for (uint8_t j = 0; j < 4; ++j)
                    state[i][j] ^= rkey[(round * ColumnNumber * 4) + (i * ColumnNumber) + j];
        }

        inline void mix_columns(state_t& state)
        {
            for (uint8_t i = 0; i < 4; ++i)
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
            for (uint8_t i = 0; i < 4; ++i)
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

        inline void cipher(state_t& state, const std::uint8_t (&rkey)[KeyExpSize], const std::uint8_t(&box)[256])
        {
            add_round_key(state, rkey, 0);

            for (uint8_t round = 1; ; ++round)
            {
                _translate_state(state, box);
                shift_rows(state);

                if (RoundNumber == round) break;

                mix_columns(state);
                add_round_key(state, rkey, round);
            }

            add_round_key(state, rkey, RoundNumber);
        }

        inline void inverse_cipher(state_t& state, const std::uint8_t (&rkey)[KeyExpSize], const std::uint8_t(&box)[256])
        {
            add_round_key(state, rkey, RoundNumber);

            for (uint8_t round = (RoundNumber - 1); ; --round)
            {
                inverse_shift_rows(state);
                _translate_state(state, box);
                add_round_key(state, rkey, round);

                if (0 == round) break;

                inverse_mix_columns(state);
            }
        }

    }   // namespace aes

    void aes_encrypt(void* data, unsigned int n, const unsigned char(&key)[32], const unsigned char(&iv)[16])
    {
        std::uint8_t sbox[256]{ 0 };
        mix_sbox(sbox, key, 32);

        std::uint8_t round_key[aes::KeyExpSize]{ 0 };
        aes::key_expansion(round_key, key, sbox);

        const std::uint8_t* iv_ptr = iv;
        std::uint8_t* buf = (uint8_t*)data;
        for (uintptr_t i = 0; i < n; i += aes::BlockSize)
        {
            memory_xor(buf, iv_ptr, aes::BlockSize);
            aes::cipher(*((aes::state_t*)buf), round_key, sbox);
            iv_ptr = buf;
            buf += aes::BlockSize;
        }
    }

    void aes_decrypt(void* data, unsigned int n, const unsigned char(&key)[32], const unsigned char(&iv)[16])
    {
        std::uint8_t sbox[256]{ 0 };
        mix_sbox(sbox, key, 32);

        std::uint8_t rsbox[256]{ 0 };
        aes::_make_inverse_sbox(rsbox, sbox);

        std::uint8_t round_key[aes::KeyExpSize]{ 0 };
        aes::key_expansion(round_key, key, sbox);

        std::uint8_t _iv[aes::BlockSize]{ 0 };
        memory_copy(_iv, iv, aes::BlockSize);

        std::uint8_t storeNextIv[aes::BlockSize]{ 0 };
        std::uint8_t* buf = (uint8_t*)data;
        for (uintptr_t i = 0; i < n; i += aes::BlockSize)
        {
            memory_copy(storeNextIv, buf, aes::BlockSize);
            aes::inverse_cipher(*((aes::state_t*)buf), round_key, rsbox);
            memory_xor(buf, _iv, aes::BlockSize);
            memory_copy(_iv, storeNextIv, aes::BlockSize);
            buf += aes::BlockSize;
        }
    }

}   // namespace xf::encrypt
