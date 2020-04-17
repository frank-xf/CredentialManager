
#include "../third/encrypt/sha256.h"
#include "../third/xf-test/xf_simple_test.h"

#include "../cm-core/cm_core.h"

std::string sha_256_signature(const std::string& str);

namespace xf::credential
{
    _xfTest(test_load_save)
    {
        string_t data;
        _xfAssert(LoadFile("../../resources/credential-demo.xml", data));

        auto s0 = sha_256_signature(data);
        _xfExpect(s0 == "af330205bb4e640dde9c3fde202c3f589a6f473dad7052e7ae844809f1152c82");

        _xfAssert(SaveFile("./temp.xml", data));

        string_t temp;
        _xfAssert(LoadFile("./temp.xml", temp));
        _xfExpect(s0 == sha_256_signature(temp));
    }

    _xfTest(test_validate_name)
    {
        _xfExpect(!CredentialMgr::ValidateName(""));
        _xfExpect(!CredentialMgr::ValidateName("1"));
        _xfExpect(!CredentialMgr::ValidateName("12"));
        _xfExpect(!CredentialMgr::ValidateName("xx#"));
        _xfExpect(!CredentialMgr::ValidateName("xx yy"));
        _xfExpect(CredentialMgr::ValidateName("123"));
        _xfExpect(CredentialMgr::ValidateName("xxx"));
        _xfExpect(CredentialMgr::ValidateName("xxx@163.com"));
        _xfExpect(CredentialMgr::ValidateName("x-y@163.com"));
        _xfExpect(CredentialMgr::ValidateName("x_y@163.com"));
        _xfExpect(CredentialMgr::ValidateName("x.y/a.b@163.com"));
        _xfExpect(CredentialMgr::ValidateName("af330205bb4e640dde9c3fde202c3f589a6f473dad7052e7ae844809f1152c82"));
        _xfExpect(!CredentialMgr::ValidateName("af330205bb4e640dde9c3fde202c3f589a6f473dad7052e7ae844809f1152c820"));
    }

    _xfTest(test_credential)
    {
        CredentialMgr mgr;
        _xfExpect(CredentialMgr::type == credential_type::ct_credential);

        _xfAssert(mgr.Load("../../resources/credential-demo.xml"));
        _xfExpect(3 == mgr.Size());

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

        auto t = mgr.Time();

        _xfExpect(mgr.Sort());
        _xfExpect(t < mgr.Time());
        _xfExpect(mgr[0]->Item().name == u8"新浪");
        _xfExpect(mgr[1]->Item().name == u8"百度");
        _xfExpect(mgr[2]->Item().name == u8"网易");
        _xfExpect(2 == mgr[2]->Size());
        mgr[2]->Traversal([&](const auto& node) { _xfExpect(7 == node.Size()); });

        t = mgr.Time();
        ptr = mgr.PushBack({ u8"新浪", u8"", u8"" });
        _xfExpect(ptr == nullptr);
        _xfExpect(t == mgr.Time());

        ptr = mgr.PushFront({ u8"谷歌", u8"www.google.com", u8"" });
        _xfExpect(ptr->Item().name == u8"谷歌");
        ptr = mgr.PushBack({ u8"微软", u8"www.microsoft.com", u8"" });
        _xfExpect(ptr->Item().name == u8"微软");

        _xfExpect(5 == mgr.Size());
        _xfExpect(mgr[0]->Item().name == u8"谷歌");
        _xfExpect(mgr[4]->Item().name == u8"微软");

        t = mgr.Time();
        _xfExpect(0 == mgr.Move(0, -1));
        _xfExpect(0 == mgr.Move(5, -1));
        _xfExpect(t == mgr.Time());
        _xfExpect(mgr[0]->Item().name == u8"谷歌");

        _xfExpect(mgr.Sort([](const auto& a, const auto& b) { return b.url < a.url; }));
        _xfExpect(mgr[0]->Item().name == u8"新浪");
        _xfExpect(mgr[1]->Item().name == u8"微软");
        _xfExpect(mgr[2]->Item().name == u8"谷歌");
        _xfExpect(mgr[3]->Item().name == u8"百度");
        _xfExpect(mgr[4]->Item().name == u8"网易");

        _xfExpect(1 == mgr[0]->Size());
        _xfExpect(0 == mgr[1]->Size());
        _xfExpect(0 == mgr[2]->Size());
        _xfExpect(1 == mgr[3]->Size());
        _xfExpect(2 == mgr[4]->Size());

        _xfExpect(1 == mgr[0]->operator[](0)->Size());
        _xfExpect(6 == mgr[3]->operator[](0)->Size());
        _xfExpect(7 == mgr[4]->operator[](0)->Size());
        _xfExpect(7 == mgr[4]->operator[](1)->Size());
        _xfExpect(nullptr == mgr[4]->operator[](2));

        _xfExpect(u8"7777777" ==
                  mgr.Find({ u8"网易", u8"", u8"" })
                    ->Find({ u8"momobudeyu@163.com", u8"" })
                    ->Find({ u8"QQ", u8"" })->Item()
                     .value);

        _xfExpect(1 == mgr.Move(0, 1));
        _xfExpect(mgr[0]->Item().name == u8"微软");
        _xfExpect(mgr[1]->Item().name == u8"新浪");
        _xfExpect(4 == mgr.Move(0, 7));
        _xfExpect(mgr[0]->Item().name == u8"新浪");
        _xfExpect(mgr[4]->Item().name == u8"微软");
        _xfExpect(-1 == mgr.Move(2, -1));
        _xfExpect(mgr[0]->Item().name == u8"新浪");
        _xfExpect(mgr[1]->Item().name == u8"百度");
        _xfExpect(mgr[2]->Item().name == u8"谷歌");
        _xfExpect(-4 == mgr.Move(4, -5));
        _xfExpect(mgr[0]->Item().name == u8"微软");
        _xfExpect(mgr[4]->Item().name == u8"网易");

        _xfExpect(!mgr.Update(0, { u8"网易", u8"www", u8"" }));
        _xfExpect(mgr.Update(4, { u8"网易", u8"www.163.com", u8R"(I'm 163)" }));
        _xfExpect(mgr.Update(1, { u8"旧浪", u8"www.sina.com", u8R"(I'm sina)" }));
        
        _xfExpect(u8"7777777@qq.com" == mgr.Find({ u8"旧浪", u8"", u8"" })->operator[](0)->Item().name);
        auto ptr1 = mgr.Find({ u8"旧浪", u8"", u8"" })->Find({ u8"7777777@qq.com", u8"" })->Add({ u8"YY", u8"1988" });
        auto ptr2 = mgr[1]->operator[](0)->operator[](1);
        _xfExpect(ptr1 == ptr2);
        _xfExpect(u8"1988" == ptr2->Item().value);
        _xfExpect(2 == mgr[1]->operator[](0)->Size());

        _xfExpect(mgr.Remove(1));
        _xfExpect(4 == mgr.Size());
        _xfExpect(mgr.Remove({ u8"微软", u8"", u8"" }));
        _xfExpect(3 == mgr.Size());
        _xfExpect(!mgr.Remove({ u8"旧浪", u8"", u8"" }));
        _xfExpect(3 == mgr.Size());
        _xfExpect(mgr.RemoveIf([](const auto& item) { return u8"www.baidu.com" == item.url; }));
        _xfExpect(2 == mgr.Size());

        mgr.Clear();
        _xfExpect(mgr.IsEmpty());

        string_t str;
        _xfExpect(mgr.Serialize(str));
    }

}
