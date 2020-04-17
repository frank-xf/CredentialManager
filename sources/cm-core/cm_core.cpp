#include <chrono>
#include <regex>
#include <fstream>
#include <sstream>

#include "../third/pugixml/pugixml.hpp"

#include "cm_core.h"

namespace xf::credential
{
    std::uint64_t CurrentTime()
    {
        auto t = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
    }

    bool LoadFile(const char* file, string_t& data)
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

    bool SaveFile(const char* file, const string_t& data)
    {
        std::ofstream fout(file);
        if (fout.is_open())
        {
            fout.write(data.c_str(), data.size());

            return true;
        }

        return false;
    }

    class _xml_string_writer : public pugi::xml_writer {
        string_t& _memory;

    public:

        explicit _xml_string_writer(string_t& mt) : _memory(mt) { }

        void write(const void* data, size_t n) override {
            _memory.append(static_cast<const char*>(data), n);
        }
    };

#define _str_text(x) x

    void platform_t::Event(event_type et, credential_type ct)
    {
        _data.Updated();
        if (_parent)
            _parent->Event(et, ct);
    }

    void account_t::Event(event_type et, credential_type ct)
    {
        _data.Updated();
        if (_parent)
            _parent->Event(et, ct);
    }

    bool CredentialMgr::SetUsername(const string_t& name)
    {
        if (ValidateName(name))
        {
            username = name;
            Updated();
            return true;
        }

        return false;
    }

    void CredentialMgr::Clear()
    {
        username.clear();
        description.clear();

        list_base::Clear();
    }

    bool CredentialMgr::Serialize(string_t& str) const
    {
        pugi::xml_document doc;

        auto declare = doc.append_child(pugi::node_declaration);
        declare.append_attribute(_str_text("version")).set_value(_str_text("1.0"));
        declare.append_attribute(_str_text("encoding")).set_value(_str_text("UTF-8"));

        auto node_credential = doc.append_child(_str_text("credential"));
        node_credential.append_attribute(_str_text("time")).set_value(time);
        node_credential.append_attribute(_str_text("version")).set_value(version.c_str());
        node_credential.append_attribute(_str_text("username")).set_value(username.c_str());
        node_credential.append_attribute(_str_text("description")).set_value(description.c_str());

        Traversal([&node_credential](const platform_t& platform) {
            auto node_platform = node_credential.append_child(_str_text("platform"));
            node_platform.append_attribute(_str_text("time")).set_value(platform.Item().time);
            node_platform.append_attribute(_str_text("name")).set_value(platform.Item().name.c_str());
            node_platform.append_attribute(_str_text("url")).set_value(platform.Item().url.c_str());
            node_platform.append_attribute(_str_text("description")).set_value(platform.Item().description.c_str());

            platform.Traversal([&node_platform](const account_t& account) {
                auto node_account = node_platform.append_child(_str_text("account"));
                node_account.append_attribute(_str_text("time")).set_value(account.Item().time);
                node_account.append_attribute(_str_text("name")).set_value(account.Item().name.c_str());
                node_account.append_attribute(_str_text("description")).set_value(account.Item().description.c_str());

                account.Traversal([&node_account](const pair_t& pair) {
                    auto node_pair = node_account.append_child(_str_text("pair"));
                    node_pair.append_attribute(_str_text("time")).set_value(pair.Item().time);
                    auto node_key = node_pair.append_child(_str_text("key"));
                    node_key.append_child(pugi::node_pcdata).set_value(pair.Item().key.c_str());
                    auto node_value = node_pair.append_child(_str_text("value"));
                    node_value.append_child(pugi::node_cdata).set_value(pair.Item().value.c_str());
                 });
            });
        });

        _xml_string_writer _writer(str);
        doc.save(_writer, _str_text("    "), pugi::format_default, pugi::encoding_utf8);

        return true;
    }
    /* // old version
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
    /*/
    bool CredentialMgr::Deserialize(const string_t& str)
    {
        pugi::xml_document doc;
        if (!doc.load_buffer(str.c_str(), str.size(), pugi::parse_default, pugi::encoding_utf8)) return false;

        auto node_credential = doc.child(_str_text("credential"));
        if (pugi::node_element != node_credential.type()) return false;

        Clear();

        for (auto node_platform : node_credential.children(_str_text("platform")))
        {
            auto platform_name = node_platform.attribute(_str_text("name"));
            if (platform_name.empty()) return false;

            auto ptr_platform = Add(
                { platform_name.value(),
                  node_platform.attribute(_str_text("url")).value(),
                  node_platform.attribute(_str_text("description")).value(),
                  node_platform.attribute(_str_text("time")).as_ullong() }
            );

            for (auto node_account : node_platform.children(_str_text("account")))
            {
                auto account_name = node_account.attribute(_str_text("name"));
                if (account_name.empty()) return false;
                
                auto ptr_account = ptr_platform->Add({ account_name.value(),
                                                       node_account.attribute(_str_text("description")).value(),
                                                       node_account.attribute(_str_text("time")).as_ullong() });

                for (auto node_pair : node_account.children(_str_text("pair")))
                {
                    auto pair_time = node_pair.attribute(_str_text("time"));

                    auto node_key = node_pair.child("key");
                    auto k = node_key.first_child();
                    if (k.empty() || pugi::node_pcdata != k.type()) return false;

                    auto node_value = node_pair.child("value");
                    auto v = node_value.first_child();
                    if (v.empty() || pugi::node_cdata != v.type()) return false;

                    ptr_account->Add({ k.value(), v.value(), pair_time.as_ullong() });
                }
            }
        }

        time = node_credential.attribute(_str_text("time")).as_ullong();
        username = node_credential.attribute(_str_text("username")).value();
        description = node_credential.attribute(_str_text("description")).value();
        version = node_credential.attribute(_str_text("version")).value();

        return true;
    }
    /**/

    bool CredentialMgr::ValidateName(const string_t& strName)
    {
        return std::regex_match(strName, std::regex(R"([0-9a-zA-Z\_\-\./@]{3,64})"));
    }

}   // namespace xf::credential
