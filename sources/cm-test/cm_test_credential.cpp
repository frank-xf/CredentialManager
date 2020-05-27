#include "../third/xf-test/xf_simple_test.h"

#include "../cm-core/cm_core.h"

bool LoadFile(const char* file, std::string& data);

namespace xf::credential
{

    _xfTest(test_validate_name)
    {
        _xfExpect(!credential_t::ValidateName(""));
        _xfExpect(!credential_t::ValidateName("1"));
        _xfExpect(!credential_t::ValidateName("12"));
        _xfExpect(!credential_t::ValidateName("xx#"));
        _xfExpect(!credential_t::ValidateName("xx yy"));
        _xfExpect(credential_t::ValidateName("123"));
        _xfExpect(credential_t::ValidateName("xxx"));
        _xfExpect(credential_t::ValidateName("xxx@163.com"));
        _xfExpect(credential_t::ValidateName("x-y@163.com"));
        _xfExpect(credential_t::ValidateName("x_y@163.com"));
        _xfExpect(credential_t::ValidateName("x.y/a.b@163.com"));
        _xfExpect(credential_t::ValidateName("af330205bb4e640dde9c3fde202c3f589a6f473dad7052e7ae844809f1152c82"));
        _xfExpect(!credential_t::ValidateName("af330205bb4e640dde9c3fde202c3f589a6f473dad7052e7ae844809f1152c820"));
    }

    _xfTest(test_credential)
    {
        credential_t credential;
        _xfExpect(credential_t::type == credential_type::ct_credential);

        string_t str;
        _xfAssert(LoadFile("../../resources/credential-demo.xml", str));
        _xfAssert(credential.Deserialize(str));
        _xfExpect(3 == credential.Size());

        _xfExpect(credential[0]->Item().name == u8"百度");
        _xfExpect(credential[0]->Item().url == u8"www.baidu.com");

        _xfExpect(credential[1]->Item().name == u8"网易");
        _xfExpect(credential[2]->Item().name == u8"新浪");

        auto ptr = credential.Find([](const auto& item) { return item.url == u8"www.163.com"; });
        _xfExpect(ptr->Item().name == u8"网易");

        ptr = credential.Find(PlatformItem(u8"新浪", "", ""));
        _xfExpect(ptr->Item().url == u8"www.sina.com.cn");

        ptr = credential.Find(PlatformItem(u8"微软", "", ""));
        _xfExpect(ptr == nullptr);

        auto t = credential.Time();

        _xfExpect(credential.Sort());
        _xfExpect(t < credential.Time());
        _xfExpect(credential[0]->Item().name == u8"新浪");
        _xfExpect(credential[1]->Item().name == u8"百度");
        _xfExpect(credential[2]->Item().name == u8"网易");
        _xfExpect(2 == credential[2]->Size());
        credential[2]->Traversal([&](const auto& node) { _xfExpect(7 == node.Size()); });

        t = credential.Time();
        ptr = credential.PushBack({ u8"新浪", u8"", u8"" });
        _xfExpect(ptr == nullptr);
        _xfExpect(t == credential.Time());

        ptr = credential.PushFront({ u8"谷歌", u8"www.google.com", u8"" });
        _xfExpect(ptr->Item().name == u8"谷歌");
        ptr = credential.PushBack({ u8"微软", u8"www.microsoft.com", u8"" });
        _xfExpect(ptr->Item().name == u8"微软");

        _xfExpect(5 == credential.Size());
        _xfExpect(credential[0]->Item().name == u8"谷歌");
        _xfExpect(credential[4]->Item().name == u8"微软");

        t = credential.Time();
        _xfExpect(0 == credential.Move(0, -1));
        _xfExpect(0 == credential.Move(5, -1));
        _xfExpect(t == credential.Time());
        _xfExpect(credential[0]->Item().name == u8"谷歌");

        _xfExpect(credential.Sort([](const auto& a, const auto& b) { return b.url < a.url; }));
        _xfExpect(credential[0]->Item().name == u8"新浪");
        _xfExpect(credential[1]->Item().name == u8"微软");
        _xfExpect(credential[2]->Item().name == u8"谷歌");
        _xfExpect(credential[3]->Item().name == u8"百度");
        _xfExpect(credential[4]->Item().name == u8"网易");

        _xfExpect(1 == credential[0]->Size());
        _xfExpect(0 == credential[1]->Size());
        _xfExpect(0 == credential[2]->Size());
        _xfExpect(1 == credential[3]->Size());
        _xfExpect(2 == credential[4]->Size());

        _xfExpect(1 == credential[0]->operator[](0)->Size());
        _xfExpect(6 == credential[3]->operator[](0)->Size());
        _xfExpect(7 == credential[4]->operator[](0)->Size());
        _xfExpect(7 == credential[4]->operator[](1)->Size());
        _xfExpect(nullptr == credential[4]->operator[](2));

        _xfExpect(u8"7777777" ==
                  credential.Find({ u8"网易", u8"", u8"" })
                    ->Find({ u8"momobudeyu@163.com", u8"" })
                    ->Find({ u8"QQ", u8"" })->Item()
                     .value);

        _xfExpect(1 == credential.Move(0, 1));
        _xfExpect(credential[0]->Item().name == u8"微软");
        _xfExpect(credential[1]->Item().name == u8"新浪");
        _xfExpect(4 == credential.Move(0, 7));
        _xfExpect(credential[0]->Item().name == u8"新浪");
        _xfExpect(credential[4]->Item().name == u8"微软");
        _xfExpect(-1 == credential.Move(2, -1));
        _xfExpect(credential[0]->Item().name == u8"新浪");
        _xfExpect(credential[1]->Item().name == u8"百度");
        _xfExpect(credential[2]->Item().name == u8"谷歌");
        _xfExpect(-4 == credential.Move(4, -5));
        _xfExpect(credential[0]->Item().name == u8"微软");
        _xfExpect(credential[4]->Item().name == u8"网易");

        _xfExpect(!credential.Update(0, { u8"网易", u8"www", u8"" }));
        _xfExpect(credential.Update(4, { u8"网易", u8"www.163.com", u8R"(I'm 163)" }));
        _xfExpect(credential.Update(1, { u8"旧浪", u8"www.sina.com", u8R"(I'm sina)" }));
        
        _xfExpect(u8"7777777@qq.com" == credential.Find({ u8"旧浪", u8"", u8"" })->operator[](0)->Item().name);
        auto ptr1 = credential.Find({ u8"旧浪", u8"", u8"" })->Find({ u8"7777777@qq.com", u8"" })->Add({ u8"YY", u8"1988" });
        auto ptr2 = credential[1]->operator[](0)->operator[](1);
        _xfExpect(ptr1 == ptr2);
        _xfExpect(u8"1988" == ptr2->Item().value);
        _xfExpect(2 == credential[1]->operator[](0)->Size());

        _xfExpect(credential.Remove(1));
        _xfExpect(4 == credential.Size());
        _xfExpect(credential.Remove({ u8"微软", u8"", u8"" }));
        _xfExpect(3 == credential.Size());
        _xfExpect(!credential.Remove({ u8"旧浪", u8"", u8"" }));
        _xfExpect(3 == credential.Size());
        _xfExpect(credential.RemoveIf([](const auto& item) { return u8"www.baidu.com" == item.url; }));
        _xfExpect(2 == credential.Size());

        credential.Clear();
        _xfExpect(credential.IsEmpty());

        _xfExpect(credential.Serialize(str));
    }

}
