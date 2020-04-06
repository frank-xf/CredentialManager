#pragma once

#include "cm_type.h"

namespace xf::credential
{
    inline const char* version() { return "1.2.0"; }

    struct pair_item : public credential_t {
        string_t key, value;
        pair_item() : credential_t(credential_type::ct_pair) { }
    };

    struct account_item : public credential_t {
        string_t account, description;
        account_item() : credential_t(credential_type::ct_account) { }
    };

    struct platform_item : public credential_t {
        string_t name, url, description;
        platform_item() : credential_t(credential_type::ct_platform) { }
    };

    struct pair_t : public node_t<pair_item, pair_t> {
        using base_type::base_type;
    };

    struct account_t : public node_t<account_item, account_t>, public list_t<pair_t> {
        using base_type::base_type;
    };

    struct platform_t : public node_t<platform_item, platform_t>, public list_t<account_t> {
        using base_type::base_type;
    };

    class CredentialMgr : public credential_t, public list_t<platform_t>
    {
    private:

        string_t username, version, description;

    public:

        CredentialMgr() : credential_t(credential_type::ct_credential) { }

        bool Serialize(memory_t& mem) const;
        bool Deserialize(const memory_t& mem);

        bool Load(const char* file);
        bool Save(const char* file);

        static bool ValidateName(const string_t& strName);
        static bool Encoding(memory_t& mt, const byte_t* key, std::size_t n);
        static bool Decoding(memory_t& mt, const byte_t* key, std::size_t n);
        static bool Check(const char* file);

    };  // class CredentialMgr

}   // namespace xf::credential
