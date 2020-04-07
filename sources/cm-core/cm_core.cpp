#include <ctime>

#include "../third/pugixml/pugixml.hpp"

#include "cm_core.h"


namespace xf::credential
{
#define _str_text(x) x

    std::uint64_t CurrentTime()
    {

    }

    struct xml_string_writer : pugi::xml_writer
    {
    public:

        explicit xml_string_writer(string_t& mt) : _memory(mt) { }

        void write(const void* data, size_t n) override
        {
            _memory.append(static_cast<const char*>(data), n);
        }

    private:

        string_t& _memory;

    };

    bool CredentialMgr::Serialize(string_t& str) const
    {
        pugi::xml_document doc;

        auto declare = doc.append_child(pugi::node_declaration);
        declare.append_attribute(_str_text("version")).set_value(_str_text("1.0"));
        declare.append_attribute(_str_text("encoding")).set_value(_str_text("UTF-8"));

        auto node_credential = doc.append_child(_str_text("credential"));
        node_credential.append_attribute(_str_text("time")).set_value(time);
        node_credential.append_attribute(_str_text("version")).set_value(xf::credential::version());
        node_credential.append_attribute(_str_text("username")).set_value(username.c_str());
        node_credential.append_attribute(_str_text("description")).set_value(description.c_str());

        Traversal([&node_credential](const platform_t& platform) {
            auto node_platform = node_credential.append_child(_str_text("platform"));
            node_platform.append_attribute(_str_text("time")).set_value(platform.GetData().time);
            node_platform.append_attribute(_str_text("name")).set_value(platform.GetData().name.c_str());
            node_platform.append_attribute(_str_text("url")).set_value(platform.GetData().url.c_str());
            node_platform.append_attribute(_str_text("description")).set_value(platform.GetData().description.c_str());

            platform.Traversal([&node_platform](const account_t& account) {
                auto node_account = node_platform.append_child(_str_text("account"));
                node_account.append_attribute(_str_text("time")).set_value(account.GetData().time);
                node_account.append_attribute(_str_text("name")).set_value(account.GetData().name.c_str());
                node_account.append_attribute(_str_text("description")).set_value(account.GetData().description.c_str());

                account.Traversal([&node_account](const pair_t& pair) {
                    auto node_pair = node_account.append_child(_str_text("pair"));
                    node_pair.append_attribute(_str_text("name")).set_value(pair.GetData().key.c_str());
                    node_pair.append_attribute(_str_text("time")).set_value(pair.GetData().time);
                    node_pair.append_child(pugi::node_cdata).set_value(pair.GetData().value.c_str());
                 });
            });
        });

        xml_string_writer writer(str);
        doc.save(writer, "    ", pugi::format_default, pugi::encoding_utf8);

        return false;
    }

    bool CredentialMgr::Deserialize(const string_t& str)
    {
        return false;
    }

    bool CredentialMgr::Load(const char* file)
    {
        return false;
    }

    bool CredentialMgr::Save(const char* file)
    {
        return false;
    }

    bool CredentialMgr::ValidateName(const string_t& strName)
    {
        return false;
    }

    bool CredentialMgr::Encoding(memory_t& mt, const byte_t* key, std::size_t n)
    {
        return false;
    }

    bool CredentialMgr::Decoding(memory_t& mt, const byte_t* key, std::size_t n)
    {
        return false;
    }

    bool CredentialMgr::Check(const char* file)
    {
        return false;
    }

    void f()
    {
        CredentialMgr mgr;

        mgr.Traversal([](platform_t& p) {

            p.GetData().type;
            p.Traversal([](account_t& a) {
                        
                a.GetData().type;
                a.Traversal([](pair_t& p) {

                    p.GetData().type;

                            });
                        
                        });

                      });
    }

}   // namespace xf::credential