#include <time.h>
#include <string>
#include <fstream>

#include "bnb_global.h"
#include "Credential.h"
#include "encrypt/RC4.h"
#include "encrypt/sha256.h"

#include "pugixml/pugixml.hpp"

_bnb_space_begin_

inline size_t _hash_seq(const byte_t *ptr, size_t n)
{
    size_t _value = 2166136261U;

    for (size_t i = 0; i < n; ++i)
    {
        _value ^= static_cast<size_t>(ptr[i]);
        _value *= 16777619U;
    }

    return (_value);
}

inline void _sha256(byte_t(&_arr)[32], const byte_t* str, size_t n)
{
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, str, n);
    sha256_final(&ctx, _arr);
}

struct xml_memory_writer : pugi::xml_writer
{
public:

    explicit xml_memory_writer(memory_type& mt) : _memory(mt) { }

    virtual void write(const void* data, size_t n)
    {
        _memory.assign(static_cast<const byte_t*>(data), n);
    }

private:

    memory_type& _memory;

};

void Credential::UpdateTime()
{
    char_t strTime[32] = { 0 };
    sprintf_s(strTime, "%llu", time(nullptr));
    m_strTime = strTime;
}

bool Credential::FromXml(const memory_type& mt)
{
    pugi::xml_document doc;
    if (!doc.load_buffer(mt.c_str(), mt.size(), pugi::parse_default, pugi::encoding_utf8)) return false;

    auto node_credential = doc.child("credential");
    if (pugi::node_element != node_credential.type()) return false;

    m_strUser = node_credential.attribute("user").value();
    m_strTime = node_credential.attribute("time").value();

    for (auto node_platform : node_credential.children("platform"))
    {
        auto name_attr_platform = node_platform.attribute("name");
        if (name_attr_platform.empty()) return false;

        auto ptr_platform = m_List.Insert(
            bnb::platform_type(name_attr_platform.value(),
            bnb::platform_data(node_platform.attribute("url").value(), node_platform.attribute("display").value()))
        );

        for (auto node_account : node_platform.children("account"))
        {
            auto name_attr_account = node_account.attribute("name");
            if (name_attr_account.empty()) return false;

            auto ptr_account = ptr_platform->m_Value.Insert(
                bnb::account_type(name_attr_account.value(),
                bnb::account_data(node_account.attribute("display").value()))
            );

            for (auto node_property : node_account.children("property"))
            {
                auto name_attr_property = node_property.attribute("name");
                if (name_attr_property.empty()) return false;

                auto ptr_property = ptr_account->m_Value.Insert(bnb::string_type(name_attr_property.value()));

                auto node_value = node_property.first_child();
                if (!node_value.empty())
                {
                    if (pugi::node_cdata != node_value.type()) return false;

                    ptr_property->m_Value = node_value.value();
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
    declare.append_attribute("version").set_value("1.0");
    declare.append_attribute("encoding").set_value("UTF-8");

    auto node_credential = doc.append_child("credential");
    node_credential.append_attribute("user").set_value(m_strUser.c_str());
    node_credential.append_attribute("time").set_value(m_strTime.c_str());

    for (auto ptr_platform = m_List.Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
    {
        auto node_platform = node_credential.append_child("platform");
        node_platform.append_attribute("name").set_value(ptr_platform->m_Pair.m_Key.m_Key.c_str());
        node_platform.append_attribute("url").set_value(ptr_platform->m_Pair.m_Key.m_Value.m_strUrl.c_str());
        node_platform.append_attribute("display").set_value(ptr_platform->m_Pair.m_Key.m_Value.m_strDisplay.c_str());

        for (auto ptr_account = ptr_platform->m_Pair.m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
        {
            auto node_account = node_platform.append_child("account");
            node_account.append_attribute("name").set_value(ptr_account->m_Pair.m_Key.m_Key.c_str());
            node_account.append_attribute("display").set_value(ptr_account->m_Pair.m_Key.m_Value.m_strDisplay.c_str());

            for (auto ptr_property = ptr_account->m_Pair.m_Value.Head(); ptr_property; ptr_property = ptr_property->m_Next)
            {
                auto node_property = node_account.append_child("property");
                node_property.append_attribute("name").set_value(ptr_property->m_Pair.m_Key.c_str());
                node_property.append_child(pugi::node_cdata).set_value(ptr_property->m_Pair.m_Value.c_str());
            }
        }
    }

    doc.print(xml_memory_writer(mt), " ", pugi::format_default, pugi::encoding_utf8, 0);

    return true;
}

result_type Credential::Load(const char * file)
{
    memory_type dst;

    bnb::result_type result = CheckFile(file, &dst);
    if (bnb::result_type::rt_success == result)
    {
        result = Decoding(dst, (const byte_t*)m_strWord.c_str(), m_strWord.size());
        if (bnb::result_type::rt_success == result)
        {
            if (!FromXml(dst))
            {
                result = bnb::result_type::rt_file_error;
            }
        }
    }

    return result;
}

bool Credential::Save(const char * file) const
{
    memory_type dst;
    
    if (ToXml(dst) && Encoding(dst, (const byte_t*)m_strWord.c_str(), m_strWord.size()))
    {
        std::ofstream fout;
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

bool Credential::Encoding(memory_type & mt, const byte_t * key, size_t n)
{
    if (key && n)
    {
        byte_t sha_key[32] = { 0 };

        _sha256(sha_key, key, n);

        RC4Encoding((byte_t*)mt.c_str(), mt.c_str(), mt.size(), key, n);

        mt.insert(_hash_seq(key, n) % mt.size(), sha_key, 32);

        RC4Encoding((byte_t*)mt.c_str(), mt.c_str(), mt.size(), sha_key, 32);

        _sha256(sha_key, mt.c_str(), mt.size());

        mt.insert(0, sha_key, 32);

        return true;
    }

    return false;
}

result_type Credential::Decoding(memory_type & mt, const byte_t * key, size_t n)
{
    if (nullptr == key || 0 == n) return result_type::rt_password_invalid;

    if (mt.size() < 64) return result_type::rt_file_invalid;

    byte_t sha_key[32] = { 0 };
    /*
    _sha256(sha_key, mt.c_str() + 32, mt.size() - 32);

    if (memcmp(sha_key, mt.c_str(), 32)) return result_type::rt_file_error;

    mt.erase(0, 32);
    */
    _sha256(sha_key, key, n);

    size_t pos = _hash_seq(key, n) % (mt.size() - 32);

    RC4Encoding((byte_t*)mt.c_str(), mt.c_str(), mt.size(), sha_key, 32);

    if (memcmp(sha_key, mt.c_str() + pos, 32)) return result_type::rt_password_error;

    mt.erase(pos, 32);

    RC4Encoding((byte_t*)mt.c_str(), mt.c_str(), mt.size(), key, n);

    return result_type::rt_success;
}

result_type Credential::CheckFile(const char * file, memory_type* dst)
{
    std::ifstream fin;
    fin.open(file, std::ios::in | std::ios::binary);
    if (!fin.is_open())
    {
        return result_type::rt_file_invalid;
    }

    fin.seekg(0, std::ios::end);
    unsigned int n = (unsigned int)fin.tellg();

    if (n < 64)
    {
        fin.close();
        return result_type::rt_file_invalid;
    }

    byte_t* buf = new byte_t[n];
    fin.seekg(0, std::ios::beg);
    fin.read((char*)buf, n);
    fin.close();

    byte_t sha_key[32] = { 0 };
    _sha256(sha_key, buf + 32, n - 32);

    if (memcmp(sha_key, buf, 32))
    {
        delete[] buf;
        return result_type::rt_file_error;
    }

    if (dst)
    {
        dst->assign(buf + 32, n - 32);
    }

    delete[] buf;
    return result_type::rt_success;
}

_bnb_space_end_