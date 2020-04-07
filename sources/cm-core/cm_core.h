#pragma once

#include "cm_type.h"

namespace xf::credential
{
    inline const char* version() { return "1.2.0"; }

    struct PairItem : public ItemBase {
        string_t key, value;
        PairItem() : ItemBase(credential_type::ct_pair) { }
    };

    struct AccountItem : public ItemBase {
        string_t account, description;
        AccountItem() : ItemBase(credential_type::ct_account) { }
    };

    struct PlatformItem : public ItemBase {
        string_t name, url, description;
        PlatformItem() : ItemBase(credential_type::ct_platform) { }
    };

    struct pair_t;
    struct account_t;
    struct platform_t;
    class CredentialMgr;

    struct pair_t : public node_t<PairItem, pair_t, account_t> {
        using base_type::base_type;
    };

    struct account_t : public node_t<AccountItem, account_t, platform_t>, public list_t<pair_t> {
        using base_type::base_type;
    };

    struct platform_t : public node_t<PlatformItem, platform_t, CredentialMgr>, public list_t<account_t> {
        using base_type::base_type;
    };

    class CredentialMgr : public ItemBase, public list_t<platform_t>
    {
    private:

        string_t username, version, description;

    public:

        CredentialMgr() : ItemBase(credential_type::ct_credential) { }

        bool Serialize(string_t& str) const;
        bool Deserialize(const string_t& str);

        bool Load(const char* file);
        bool Save(const char* file);

        static bool ValidateName(const string_t& strName);
        static bool Encoding(memory_t& mt, const byte_t* key, std::size_t n);
        static bool Decoding(memory_t& mt, const byte_t* key, std::size_t n);
        static bool Check(const char* file);

    };  // class CredentialMgr

}   // namespace xf::credential
