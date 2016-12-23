#include <time.h>
#include <string>
#include <fstream>

#include<iostream>

#include "encrypt/RC4.h"
#include "encrypt/sha256.h"
#include "pugixml/pugixml.hpp"

#include "Credential.h"

void UTF8toANSI(std::string &strUTF8);

namespace bnb
{
    using ifstream_type = std::basic_ifstream<char, std::char_traits<char>>;
    using ofstream_type = std::basic_ofstream<char, std::char_traits<char>>;

    //------------------------------------------------------------------------------

    enum class _credential_key_index : unsigned int
    {
        sk_credential,
        sk_platform,
        sk_account,
        sk_pair,
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

    //------------------------------------------------------------------------------

    const wchar_t* _credential_key_string[]{
        L"credential",
        L"platform",
        L"account",
        L"pair",
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

    //------------------------------------------------------------------------------

#define _sKey(k) ::bnb::_credential_key_string[static_cast<unsigned int>(::bnb::_credential_key_index::k)]

    //------------------------------------------------------------------------------

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

    //------------------------------------------------------------------------------

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

    //------------------------------------------------------------------------------

    void account_node::Updated(param_type aType)
    {
        if (platform_node* ptr = dynamic_cast<platform_node*>(GetParent()))
            ptr->Updated(aType, static_cast<param_type>(credential_enum::pair));
    }

    //------------------------------------------------------------------------------

    void platform_node::Updated(param_type aType)
    {
        Updated(aType, static_cast<param_type>(credential_enum::account));
    }

    void platform_node::Updated(param_type aType, param_type cType)
    {
        if (Credential* ptr = dynamic_cast<Credential*>(GetParent()))
            ptr->Updated(aType, cType);
    }

    //------------------------------------------------------------------------------

    unsigned long long credential_type::_GetTime()
    {
        return time(nullptr);
    }

    void credential_type::UpdateTime()
    {
        m_ullTime = _GetTime();
    }

    bool credential_type::ValidateWord(const string_type& strWord) const
    {
        if (strWord.empty() || m_strWord.size() != strWord.size()) return false;

        const char_type* ps1 = m_strWord.c_str();
        const char_type* ps2 = strWord.c_str();
        for (size_t i = 0; i < m_strWord.size(); ++i)
            if (ps1[i] != ps2[i])
                return false;

        return true;
    }

    //------------------------------------------------------------------------------

    void Credential::SetWord(const string_type& strWord)
    {
        _data.m_strWord = strWord;
        Updated(static_cast<param_type>(action_type::at_update), static_cast<param_type>(credential_enum::credential));
    }

    void Credential::SetUser(const string_type& strUser)
    {
        _data.m_strUser = strUser;
        Updated(static_cast<param_type>(action_type::at_update), static_cast<param_type>(credential_enum::credential));
    }

    void Credential::SetComment(const string_type& strComment)
    {
        _data.m_strComment = strComment;
        Updated(static_cast<param_type>(action_type::at_update), static_cast<param_type>(credential_enum::credential));
    }

    void Credential::Clear()
    {
        list_base::Clear();

        _data.m_strWord.clear();
        _data.m_strUser.clear();
        _data.m_strComment.clear();
        Updated(static_cast<param_type>(action_type::at_clear), static_cast<param_type>(credential_enum::credential));
    }

    void Credential::Updated(param_type aType)
    {
        Updated(aType, static_cast<param_type>(credential_enum::platform));
    }

    void Credential::Updated(param_type aType, param_type cType)
    {
        _data.UpdateTime();

        switch (static_cast<action_type>(aType))
        {
        case action_type::at_insert:
        case action_type::at_delete:
        case action_type::at_update:
        case action_type::at_clear:
        case action_type::at_move:
        case action_type::at_sort:
        case action_type::at_none:
        default:
            break;
        }

        const char* str1[] = { "none", "insert" , "delete" , "update" , "move" , "sort", "clear" };
        const char* str2[] = { "credential", "platform" , "account" , "pair" };

        memory_type xml;

        ToXml(xml);
        std::string _xml((char*)xml.c_str(), xml.size());
        UTF8toANSI(_xml);

        std::cout << str1[aType] << ": " << str2[cType] << std::endl;
        std::cout << _xml << std::endl;
        std::cout << "-------------------------------------------" << std::endl << std::endl;
    }

    account_node* Credential::Find(id_type id1, id_type id2)
    {
        if (platform_node* platform_ptr = Find(id1))
            return platform_ptr->Find(id2);

        return nullptr;
    }

    const account_node* Credential::Find(id_type id1, id_type id2) const
    {
        if (const platform_node* platform_ptr = Find(id1))
            return platform_ptr->Find(id2);

        return nullptr;
    }

    pair_node* Credential::Find(id_type id1, id_type id2, id_type id3)
    {
        if (account_node* account_ptr = Find(id1, id2))
            return account_ptr->Find(id3);

        return nullptr;
    }

    const pair_node* Credential::Find(id_type id1, id_type id2, id_type id3) const
    {
        if (const account_node* account_ptr = Find(id1, id2))
            return account_ptr->Find(id3);

        return nullptr;
    }

    bool Credential::FromXml(const memory_type& mt)
    {
        pugi::xml_document doc;
        if (!doc.load_buffer(mt.c_str(), mt.size(), pugi::parse_default, pugi::encoding_utf8)) return false;

        auto node_credential = doc.child(_sKey(sk_credential));
        if (pugi::node_element != node_credential.type()) return false;

        list_base::Clear();

        for (auto node_platform : node_credential.children(_sKey(sk_platform)))
        {
            auto name_attr_platform = node_platform.attribute(_sKey(sk_name));
            if (name_attr_platform.empty()) return false;

            auto ptr_platform = PushBack({
                name_attr_platform.value(), node_platform.attribute(_sKey(sk_url)).value(), node_platform.attribute(_sKey(sk_comment)).value() }
            );

            for (auto node_account : node_platform.children(_sKey(sk_account)))
            {
                auto name_attr_account = node_account.attribute(_sKey(sk_name));
                if (name_attr_account.empty()) return false;

                auto ptr_account = ptr_platform->PushBack({ name_attr_account.value(), node_account.attribute(_sKey(sk_comment)).value() });

                for (auto node_pair : node_account.children(_sKey(sk_pair)))
                {
                    auto name_attr_pair = node_pair.attribute(_sKey(sk_name));
                    if (name_attr_pair.empty()) return false;

                    auto node_value = node_pair.first_child();
                    if (!node_value.empty())
                    {
                        if (pugi::node_cdata != node_value.type()) return false;

                        ptr_account->PushBack({ name_attr_pair.value(), node_value.value() });
                    }
                }
            }
        }

        _data.m_ullTime = node_credential.attribute(_sKey(sk_time)).as_ullong();
        _data.m_strUser = node_credential.attribute(_sKey(sk_user)).value();
        _data.m_strComment = node_credential.attribute(_sKey(sk_comment)).value();

        return true;
    }

    bool Credential::ToXml(memory_type& mt) const
    {
        pugi::xml_document doc;

        auto declare = doc.append_child(pugi::node_declaration);
        declare.append_attribute(_sKey(sk_version)).set_value(L"1.0");
        declare.append_attribute(_sKey(sk_encoding)).set_value(L"UTF-8");

        auto node_credential = doc.append_child(_sKey(sk_credential));
        node_credential.append_attribute(_sKey(sk_time)).set_value(_data.GetTime());
        node_credential.append_attribute(_sKey(sk_user)).set_value(_data.GetUser().c_str());
        node_credential.append_attribute(_sKey(sk_comment)).set_value(_data.GetComment().c_str());

        PreorderTraversal([&node_credential](const platform_node& platform) {
            auto node_platform = node_credential.append_child(_sKey(sk_platform));
            node_platform.append_attribute(_sKey(sk_name)).set_value(platform.GetData().GetName().c_str());
            node_platform.append_attribute(_sKey(sk_url)).set_value(platform.GetData().GetUrl().c_str());
            node_platform.append_attribute(_sKey(sk_comment)).set_value(platform.GetData().GetComment().c_str());

            platform.PreorderTraversal([&node_platform](const account_node& account) {
                auto node_account = node_platform.append_child(_sKey(sk_account));
                node_account.append_attribute(_sKey(sk_name)).set_value(account.GetData().GetName().c_str());
                node_account.append_attribute(_sKey(sk_comment)).set_value(account.GetData().GetComment().c_str());

                account.PreorderTraversal([&node_account](const pair_node& pair) {
                    auto node_pair = node_account.append_child(_sKey(sk_pair));
                    node_pair.append_attribute(_sKey(sk_name)).set_value(pair.GetData().GetKey().c_str());
                    node_pair.append_child(pugi::node_cdata).set_value(pair.GetData().GetValue().c_str());
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
            if (Decoding(dst, (const byte_type*)_data.GetWord().c_str(), _data.GetWord().size() * sizeof(char_type)))
                if (FromXml(dst))
                    return true;

        return false;
    }

    bool Credential::Save(const char * file) const
    {
        memory_type dst;

        if (ToXml(dst) && Encoding(dst, (const byte_type*)_data.GetWord().c_str(), _data.GetWord().size() * sizeof(char_type)))
        {
            ofstream_type fout;
            fout.open(file, std::ios::out | std::ios::trunc | std::ios::binary);

            if (fout.is_open())
            {
                fout.write((const char*)dst.data(), dst.size());
                fout.close();

                return true;
            }
        }

        return false;
    }

    bool Credential::ValidateName(const string_type & strName)
    {
        if (strName.empty()) return false;

        for (auto character : strName)
        {
            switch (character)
            {
            case '\\': case '\'': case '\"': case '\n': case '\t':
            case '~': case '!': case '#': case '$': case '%': case '^':
            case '&': case '*': case '(': case ')': case '[': case ']':
            case '{': case '}': case '<': case '>': case '+': case '|':
            case ':': case ';': case ',': case '?': case '/': return false;
            default:
                break;
            }
        }

        return true;
    }

    bool Credential::Encoding(memory_type & mt, const byte_type * key, size_t n)
    {
        if (key && n)
        {
            byte_type sha_key[32]{ 0 };
            SHA_256(sha_key, key, n);

            RC4Encoding((byte_type*)mt.data(), mt.data(), mt.size(), key, n);

            mt.insert(_hash_seq(key, n) % mt.size(), sha_key, 32);

            RC4Encoding((byte_type*)mt.data(), mt.data(), mt.size(), sha_key, 32);

            SHA_256(sha_key, mt.data(), mt.size());

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

            RC4Encoding((byte_type*)mt.data(), mt.data(), mt.size(), sha_key, 32);

            if (0 == memcmp(sha_key, mt.data() + pos, 32))
            {
                mt.erase(pos, 32);

                return (0 < RC4Encoding((byte_type*)mt.data(), mt.data(), mt.size(), key, n));
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

#include <windows.h>

void UTF8toANSI(std::string &strUTF8)
{
    //获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区  
    unsigned int nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
    wchar_t *wszBuffer = new wchar_t[nLen + 1];
    nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;

    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    char *szBuffer = new char[nLen + 1];
    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;

    strUTF8 = szBuffer;
    //清理内存  
    delete[]szBuffer;
    delete[]wszBuffer;
}
