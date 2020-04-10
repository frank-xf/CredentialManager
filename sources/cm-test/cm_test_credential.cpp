
#include "../third/encrypt/sha256.h"
#include "../third/xf-test/xf_simple_test.h"

#include "../cm-core/cm_core.h"

std::string sha_256_signature(const std::string& str);

namespace xf::credential
{
    _xfTest(test_load_save)
    {
        memory_t data;
        _xfAssert(LoadFile("../../resources/credential-demo.xml", data));

        auto s0 = sha_256_signature(data);
        _xfExpect(s0 == "af330205bb4e640dde9c3fde202c3f589a6f473dad7052e7ae844809f1152c82");

        _xfAssert(SaveFile("./temp.xml", data));

        memory_t temp;
        _xfAssert(LoadFile("./temp.xml", temp));
        _xfExpect(s0 == sha_256_signature(temp));
    }

    _xfTest(test_credential)
    {
        CredentialMgr mgr;
        _xfExpect(CredentialMgr::type == credential_type::ct_credential);

        _xfAssert(mgr.Load("../../resources/credential-demo.xml"));
        _xfExpect(mgr.Size() == 3);

        _xfExpect(mgr[0]->Item().name == u8"百度");
        _xfExpect(mgr[0]->Item().url == u8"www.baidu.com");

        _xfExpect(mgr[1]->Item().name == u8"网易");
        _xfExpect(mgr[2]->Item().name == u8"新浪");

        auto ptr = mgr.Find([](const auto& item) { return item.url == u8"www.163.com"; });
        _xfExpect(ptr->Item().name == u8"网易");

        ptr = mgr.Find(PlatformItem(u8"新浪", "", ""));
        _xfExpect(ptr->Item().url == u8"www.sina.com.cn");

        ptr = mgr.Find(PlatformItem(u8"微软", "", ""));
        _xfExpect(ptr == nullptr);
    }

}
