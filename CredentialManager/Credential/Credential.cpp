#include <time.h>
#include <string>
#include <fstream>

#include "encrypt/RC4.h"
#include "encrypt/sha256.h"
#include "pugixml/pugixml.hpp"

#include "Credential.h"

namespace bnb
{
    using ifstream_type = std::basic_ifstream<char, std::char_traits<char>>;
    using ofstream_type = std::basic_ofstream<char, std::char_traits<char>>;

    enum class _credential_key_index : unsigned int
    {
        sk_credential,
        sk_platform,
        sk_account,
        sk_property,
        sk_user,
        sk_time,
        sk_name,
        sk_url,
        sk_key,
        sk_value,
        sk_comment,
        sk_version,
        sk_encoding
    };

    const wchar_t* _credential_key_string[]{
        L"credential",
        L"platform",
        L"account",
        L"property",
        L"user",
        L"time",
        L"name",
        L"url",
        L"key",
        L"value",
        L"comment",
        L"version",
        L"encoding"
    };

#define _sKey(k) ::bnb::_credential_key_string[static_cast<unsigned int>(::bnb::_credential_key_index::k)]

    size_t credential_base::_credential_id(1);

    inline size_t _hash_seq(const byte_type *ptr, size_t n)
    {
        size_t _value = 2166136261u;

        for (size_t i = 0; i < n; ++i)
        {
            _value ^= static_cast<size_t>(ptr[i]);
            _value *= 16777619u;
        }

        return (_value);
    }

    struct xml_memory_writer : pugi::xml_writer
    {
    public:

        explicit xml_memory_writer(memory_type& mt) : _memory(mt) { }

        void write(const void* data, size_t n) override
        {
            _memory.append(static_cast<const byte_type*>(data), n);
        }

    private:

        memory_type& _memory;

    };

    void Credential::Clear()
    {
        m_strWord.clear();
        m_strUser.clear();
        m_ullTime = 0;

        m_Tree.Clear();
    }

    void Credential::UpdateTime()
    {
        m_ullTime = time(nullptr);
    }

    bool Credential::ValidateWord(const string_type& strWord) const
    {
        if (strWord.empty() || m_strWord.size() != strWord.size()) return false;

        const char_type* ps1 = m_strWord.c_str();
        const char_type* ps2 = strWord.c_str();
        for (size_t i = 0; i < m_strWord.size(); ++i)
            if (ps1[i] != ps2[i])
                return false;

        return true;
    }

    bool Credential::FromXml(const memory_type& mt)
    {
        pugi::xml_document doc;
        if (!doc.load_buffer(mt.c_str(), mt.size(), pugi::parse_default, pugi::encoding_utf8)) return false;

        auto node_credential = doc.child(_sKey(sk_credential));
        if (pugi::node_element != node_credential.type()) return false;

        m_Tree.Clear();

        m_ullTime = node_credential.attribute(_sKey(sk_time)).as_ullong();
        m_strUser = node_credential.attribute(_sKey(sk_user)).value();
        m_strComment = node_credential.attribute(_sKey(sk_comment)).value();

        for (auto node_platform : node_credential.children(_sKey(sk_platform)))
        {
            auto name_attr_platform = node_platform.attribute(_sKey(sk_name));
            if (name_attr_platform.empty()) return false;

            auto ptr_platform = m_Tree.Insert({
                name_attr_platform.value(), node_platform.attribute(_sKey(sk_url)).value(), node_platform.attribute(_sKey(sk_comment)).value() }
            );

            for (auto node_account : node_platform.children(_sKey(sk_account)))
            {
                auto name_attr_account = node_account.attribute(_sKey(sk_name));
                if (name_attr_account.empty()) return false;

                auto ptr_account = ptr_platform->m_Value.Insert({ name_attr_account.value(), node_account.attribute(_sKey(sk_comment)).value() });

                for (auto node_property : node_account.children(_sKey(sk_property)))
                {
                    auto name_attr_property = node_property.attribute(_sKey(sk_name));
                    if (name_attr_property.empty()) return false;

                    auto ptr_property = ptr_account->m_Value.Insert({ name_attr_property.value() });

                    auto node_value = node_property.first_child();
                    if (!node_value.empty())
                    {
                        if (pugi::node_cdata != node_value.type()) return false;

                        ptr_property->m_Value.m_strName = node_value.value();
                    }
                }
            }
        }

        return true;
    }

    bool Credential::ToXml(memory_type& mt) const
    {
        pugi::xml_document doc;

        auto declare = doc.append_child(pugi::node_declaration);
        declare.append_attribute(_sKey(sk_version)).set_value(L"1.0");
        declare.append_attribute(_sKey(sk_encoding)).set_value(L"UTF-8");

        auto node_credential = doc.append_child(_sKey(sk_credential));
        node_credential.append_attribute(_sKey(sk_time)).set_value(m_ullTime);
        node_credential.append_attribute(_sKey(sk_user)).set_value(m_strUser.c_str());
        node_credential.append_attribute(_sKey(sk_comment)).set_value(m_strComment.c_str());

        m_Tree.Foreach([&node_credential](const platform_tree::data_type& platform) {
            auto node_platform = node_credential.append_child(_sKey(sk_platform));
            node_platform.append_attribute(_sKey(sk_name)).set_value(platform.m_Key.m_strName.c_str());
            node_platform.append_attribute(_sKey(sk_url)).set_value(platform.m_Key.m_strUrl.c_str());
            node_platform.append_attribute(_sKey(sk_comment)).set_value(platform.m_Key.m_strComment.c_str());

            platform.m_Value.Foreach([&node_platform](const account_tree::data_type& account) {
                auto node_account = node_platform.append_child(_sKey(sk_account));
                node_account.append_attribute(_sKey(sk_name)).set_value(account.m_Key.m_strName.c_str());
                node_account.append_attribute(_sKey(sk_comment)).set_value(account.m_Key.m_strComment.c_str());

                account.m_Value.Foreach([&node_account](const property_tree::data_type& property) {
                    auto node_property = node_account.append_child(_sKey(sk_property));
                    node_property.append_attribute(_sKey(sk_name)).set_value(property.m_Key.m_strName.c_str());
                    node_property.append_child(pugi::node_cdata).set_value(property.m_Value.m_strName.c_str());
                });
            });
        });

        doc.save(xml_memory_writer(mt), L"    ", pugi::format_default, pugi::encoding_utf8);

        return true;
    }

    bool Credential::Load(const char * file)
    {
        memory_type dst;

        if (CheckFile(file, &dst))
            if (Decoding(dst, (const byte_type*)m_strWord.c_str(), m_strWord.size() * sizeof(char_type)))
                if (FromXml(dst))
                    return true;

        return false;
    }

    bool Credential::Save(const char * file) const
    {
        memory_type dst;

        if (ToXml(dst) && Encoding(dst, (const byte_type*)m_strWord.c_str(), m_strWord.size() * sizeof(char_type)))
        {
            ofstream_type fout;
            fout.open(file, std::ios::out | std::ios::trunc | std::ios::binary);

            if (fout.is_open())
            {
                fout.write((const char*)dst.c_str(), dst.size());
                fout.close();

                return true;
            }
        }

        return false;
    }

    bool Credential::Encoding(memory_type & mt, const byte_type * key, size_t n)
    {
        if (key && n)
        {
            byte_type sha_key[32]{ 0 };
            SHA_256(sha_key, key, n);

            RC4Encoding((byte_type*)mt.c_str(), mt.c_str(), mt.size(), key, n);

            mt.insert(_hash_seq(key, n) % mt.size(), sha_key, 32);

            RC4Encoding((byte_type*)mt.c_str(), mt.c_str(), mt.size(), sha_key, 32);

            SHA_256(sha_key, mt.c_str(), mt.size());

            mt.insert(0, sha_key, 32);

            return true;
        }

        return false;
    }

    bool Credential::Decoding(memory_type & mt, const byte_type * key, size_t n)
    {
        if (key && n && 0x40 < mt.size())
        {
            byte_type sha_key[32]{ 0 };
            SHA_256(sha_key, key, n);

            size_t pos = _hash_seq(key, n) % (mt.size() - 32);

            RC4Encoding((byte_type*)mt.c_str(), mt.c_str(), mt.size(), sha_key, 32);

            if (0 == memcmp(sha_key, mt.c_str() + pos, 32))
            {
                mt.erase(pos, 32);

                return (0 < RC4Encoding((byte_type*)mt.c_str(), mt.c_str(), mt.size(), key, n));
            }
        }

        return false;
    }

    bool Credential::CheckFile(const char * file, memory_type* dst)
    {
        bool result = false;

        ifstream_type fin;
        fin.open(file, std::ios::in | std::ios::binary);

        if (fin.is_open())
        {
            fin.seekg(0, std::ios::end);
            unsigned int n = (unsigned int)fin.tellg();

            if (0x40 < n)
            {
                byte_type* buf = new byte_type[n];
                fin.seekg(0, std::ios::beg);
                fin.read((char*)buf, n);

                byte_type sha_key[32] = { 0 };
                SHA_256(sha_key, buf + 32, n - 32);

                if (result = (0 == memcmp(sha_key, buf, 32)))
                    if (dst)
                        dst->assign(buf + 32, n - 32);

                delete[] buf;
            }

            fin.close();
        }

        return result;
    }

}
