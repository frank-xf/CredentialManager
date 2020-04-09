#include "../third/xf-test/xf_simple_test.h"
#include "../third/encrypt/sha256.h"

#include "../cm-core/cm_core.h"

_xfTest(test_sha_256)
{
    char x[64]{ 0 };

    xf::encrypt::sha_256(x, (const unsigned char*)"", 0);
    _xfExpect(std::string("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") == x);

    xf::encrypt::sha_256(x, (const unsigned char*)" ", 1);
    _xfExpect(std::string("36a9e7f1c95b82ffb99743e0c5c4ce95d83c9a430aac59f84ef3cbfab6145068") == x);

    xf::encrypt::sha_256(x, (const unsigned char*)"x", 1);
    _xfExpect(std::string("2d711642b726b04401627ca9fbac32f5c8530fb1903cc4db02258717921a4881") == x);

    xf::encrypt::sha_256(x, (const unsigned char*)"X", 1);
    _xfExpect(std::string("4b68ab3847feda7d6c62c1fbcbeebfa35eab7351ed5e78f4ddadea5df64b8015") == x);

    xf::encrypt::sha_256(x, (const unsigned char*)"Frank", 5);
    _xfExpect(std::string("db605e8f71913d1f3966ad908d78b8a8084f5047122037b2b91a7192b598a9ad") == x);

    xf::encrypt::sha_256(x, (const unsigned char*)"Xiong", 5);
    _xfExpect(std::string("522eda9ae0455b4c3037c00b5a01503c4cf5203d2526ae41f90317ba55aeba61") == x);

    xf::encrypt::sha_256(x, (const unsigned char*)"FrankXiong", 10);
    _xfExpect(std::string("1bf256efaa765f921a9531d3fe0178be78399e872b2aca178b4bf28d278676f1") == x);

    xf::encrypt::sha_256(x, (const unsigned char*)"CredentialManager", 17);
    _xfExpect(std::string("56a77307d5e791304996db1e7d79a5136d64e07084320030d4ca4fbd90c359f3") == x);
}

_xfTest(test_load_file)
{
    xf::credential::memory_t data;
    _xfAssert(xf::credential::LoadFile("../../resources/credential-demo.xml", data));

    char x[64]{ 0 };
    xf::encrypt::sha_256(x, (const unsigned char*)data.c_str(), data.size());

    _xfExpect(std::string("") == x);
}


