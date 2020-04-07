#include <chrono>

#include "../third/pugixml/pugixml.hpp"

#include "cm_core.h"


namespace xf::credential
{
#define _str_text(x) x

    std::uint64_t CurrentTime()
    {
        auto t = std::chrono::steady_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
    }

    class _xml_string_writer : public pugi::xml_writer {
        string_t& _memory;

    public:

        explicit _xml_string_writer(string_t& mt) : _memory(mt) { }

        void write(const void* data, size_t n) override {
            _memory.append(static_cast<const char*>(data), n);
        }
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
                    node_pair.append_attribute(_str_text("time")).set_value(pair.GetData().time);
                    auto node_key = node_pair.append_child(_str_text("key")).set_value(pair.GetData().key.c_str());
                    auto node_value = node_pair.append_child(_str_text("value"));
                    node_value.append_child(pugi::node_cdata).set_value(pair.GetData().value.c_str());
                 });
            });
        });

        _xml_string_writer _writer(str);
        doc.save(_writer, "    ", pugi::format_default, pugi::encoding_utf8);

        return true;
    }

    bool CredentialMgr::Deserialize(const string_t& str)
    {
        pugi::xml_document doc;
        if (!doc.load_buffer(str.c_str(), str.size(), pugi::parse_default, pugi::encoding_utf8)) return false;

        auto node_credential = doc.child(_str_text("credential"));
        if (pugi::node_element != node_credential.type()) return false;

        Clear();

        for (auto node_platform : node_credential.children(_str_text("platform")))
        {
            auto name_attr_platform = node_platform.attribute(_str_text("name"));
            if (name_attr_platform.empty()) return false;

            auto ptr_platform = Add(
                { name_attr_platform.value(), node_platform.attribute(_str_text("url")).value(), node_platform.attribute(_str_text("comment")).value() }
            );

            for (auto node_account : node_platform.children(_str_text("account")))
            {
                auto name_attr_account = node_account.attribute(_str_text("name"));
                if (name_attr_account.empty()) return false;

                auto ptr_account = ptr_platform->Add({ name_attr_account.value(), node_account.attribute(_str_text("comment")).value() });

                for (auto node_pair : node_account.children(_str_text("pair")))
                {
                    auto name_attr_pair = node_pair.attribute(_str_text("name"));
                    if (name_attr_pair.empty()) return false;

                    auto node_value = node_pair.first_child();
                    if (!node_value.empty())
                    {
                        if (pugi::node_cdata != node_value.type()) return false;

                        ptr_account->Add({ name_attr_pair.value(), node_value.value() });
                    }
                }
            }
        }

        time = node_credential.attribute(_str_text("time")).as_ullong();
        username = node_credential.attribute(_str_text("user")).value();
        description = node_credential.attribute(_str_text("comment")).value();
        version = xf::credential::version();

        return true;
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