#include <sstream>
#include <fstream>

#include "../third/xf-test/xf_simple_test.h"
#include "../third/encrypt/rc4.h"
#include "../third/encrypt/base64.h"
#include "../third/encrypt/aes256.h"
#include "../third/encrypt/sha256.h"

#include "../cm-encrypt/cm_encrypt.h"

std::string sha_256_signature(const std::string& str)
{
    unsigned char x[32]{ 0 };
    xf::encrypt::sha_256(x, str.c_str(), str.size());

    return xf::credential::encrypt::SignatureText(x);
}

bool LoadFile(const char* file, std::string& data)
{
    std::ifstream fin(file);
    if (fin.is_open())
    {
        std::ostringstream os;
        os << fin.rdbuf();
        data.assign(os.str());

        return true;
    }

    return false;
}

namespace xf::credential::encrypt
{
    _xfTest(_test_sha_256)
    {
        // 以下用于比较的 sha256 签名由 https://emn178.github.io/online-tools/sha256.html 计算得出

        auto s1 = sha_256_signature("");
        _xfExpect(s1 == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

        auto s2 = sha_256_signature(" ");
        _xfExpect(s2 == "36a9e7f1c95b82ffb99743e0c5c4ce95d83c9a430aac59f84ef3cbfab6145068");

        auto s3 = sha_256_signature("x");
        _xfExpect(s3 == "2d711642b726b04401627ca9fbac32f5c8530fb1903cc4db02258717921a4881");

        auto s4 = sha_256_signature("X");
        _xfExpect(s4 == "4b68ab3847feda7d6c62c1fbcbeebfa35eab7351ed5e78f4ddadea5df64b8015");

        auto s5 = sha_256_signature("Frank");
        _xfExpect(s5 == "db605e8f71913d1f3966ad908d78b8a8084f5047122037b2b91a7192b598a9ad");

        auto s6 = sha_256_signature("Xiong");
        _xfExpect(s6 == "522eda9ae0455b4c3037c00b5a01503c4cf5203d2526ae41f90317ba55aeba61");

        auto s7 = sha_256_signature("FrankXiong");
        _xfExpect(s7 == "1bf256efaa765f921a9531d3fe0178be78399e872b2aca178b4bf28d278676f1");

        auto s8 = sha_256_signature("CredentialManager");
        _xfExpect(s8 == "56a77307d5e791304996db1e7d79a5136d64e07084320030d4ca4fbd90c359f3");

        string_t data;
        _xfAssert(LoadFile("../../resources/credential-demo.xml", data));
        auto s9 = sha_256_signature(data);
        _xfExpect(s9 == "af330205bb4e640dde9c3fde202c3f589a6f473dad7052e7ae844809f1152c82");
    }

    _xfTest(_test_rc4_string)
    {
        char str[0x400]{ 0 };

        string_t data("CredentialManager");
        auto s0 = sha_256_signature(data);

        _xfExpect(data.size() == xf::encrypt::rc4(str, data.c_str(), data.size(), nullptr, 0));
        string_t data1(str, data.size());
        auto s1 = sha_256_signature(data1);

        _xfExpect(s0 != s1);

        xf::encrypt::rc4(str, data1.c_str(), data1.size(), nullptr, 0);
        string_t data2(str, data.size());
        auto s2 = sha_256_signature(data2);

        _xfExpect(data == data2 && s0 == s2);

        _xfExpect(data.size() == xf::encrypt::rc4(str, data.c_str(), data.size(), "FrankXiong", 10));
        string_t data3(str, data.size());
        auto s3 = sha_256_signature(data3);

        _xfExpect(s0 != s3);

        xf::encrypt::rc4(str, data3.c_str(), data3.size(), "FrankXiong", 10);
        string_t data4(str, data.size());
        auto s4 = sha_256_signature(data4);

        _xfExpect(data == data4 && s0 == s4);
    }

    _xfTest(_test_rc4_file)
    {
        string_t data;
        _xfAssert(LoadFile("../../resources/credential-demo.xml", data));
        auto s0 = sha_256_signature(data);

        string_t key("CredentialManager");

        memory_t buf(data.size());
        _xfExpect(data.size() == xf::encrypt::rc4(buf.data(), data.data(), data.size(), key.data(), key.size()));
        string_t data1 = memory_to_string(buf);
        auto s1 = sha_256_signature(data1);

        _xfExpect(s0 != s1);

        xf::encrypt::rc4(buf.data(), data1.data(), data1.size(), key.data(), key.size());
        string_t data2 = memory_to_string(buf);
        auto s2 = sha_256_signature(data2);

        _xfExpect(data == data2 && s0 == s2);
    }

    _xfTest(_test_aes_256)
    {
        const uint8_t key[] = {
            0x43, 0x72, 0x65, 0x64, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x4d, 0x61, 0x6e, 0x61, 0x67, 0x65,
            0x43, 0x72, 0x65, 0x64, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x4d, 0x61, 0x6e, 0x61, 0x67, 0x65
        };
        const uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

        const string_t data("Credential Manager By FrankXiong");
        unsigned char str[32]{ 0 };
        for (int i = 0; i < 32; ++i) str[i] = data[i];

        xf::encrypt::aes_encrypt(str, 32, key, iv);
        _xfExpect(string_t((char*)str, 32) != data);
        // auto text = signature_string(str);
        // _xfExpect(text == "bf97ef27b90e209062d2a55a565cdf4fea1bcabb7e0ce60c520dba687c028817");

        xf::encrypt::aes_decrypt(str, 32, key, iv);
        _xfExpect(string_t((char*)str, 32) == data);
    }

    _xfTest(_test_base64)
    {
        string_t str("CredentialManager");

        memory_t buf(0x80);;
        _xfExpect(23 == xf::encrypt::base64_encoding(buf.data(), str.data(), str.size()));

        string_t text((const char*)buf.data(), 23);
        _xfExpect(text == "2TYKZMJupMK7hEL7h?ISZRU");

        _xfExpect(17 == xf::encrypt::base64_decoding(buf.data(), text.data(), text.size()));
        _xfExpect(compare_memory(str.data(), str.size(), buf.data(), str.size()));
    }

    _xfTest(_test_encrypt)
    {
        string_t str("Credential Manager by FrankXiong  2020-04");
        string_t k1("Frank"), k2("1234567890");

        memory_t buf = string_to_memory(str);
        _xfAssert(compare_memory(str.data(), str.size(), buf.data(), buf.size()));

        _xfAssert(Encrypt(buf, k1.data(), k1.size(), k2.data(), k2.size()));
        _xfAssert(!compare_memory(str.data(), str.size(), buf.data(), buf.size()));

        _xfAssert(Decrypt(buf, k1.data(), k1.size(), k2.data(), k2.size()));
        _xfAssert(compare_memory(str.data(), str.size(), buf.data(), buf.size()));

        const char* temp_file = "./tempfile.txt";
        _xfAssert(Save(temp_file, str, k1.data(), k1.size(), k2.data(), k2.size()));
        _xfAssert(ValidateFile(temp_file));
        _xfAssert(Load(temp_file, str, k1.data(), k1.size(), k2.data(), k2.size()));
        _xfAssert(compare_memory(str.data(), str.size(), buf.data(), buf.size()));
    }

}
