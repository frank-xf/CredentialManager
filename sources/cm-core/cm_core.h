#pragma once

#include "cm_type.h"

namespace xf::credential
{
    inline const char_t* version() { return "1.2.0"; }

    struct PairItem : public ItemBase<credential_type::ct_pair> {
        string_t key, value;

        PairItem(const string_t& k, const string_t& v)
            : base_type(), key(k), value(v) { }
        PairItem(const string_t& k, const string_t& v, time_t t)
            : base_type(t), key(k), value(v) { }
    };

    struct AccountItem : public ItemBase<credential_type::ct_account> {
        string_t name, description;

        AccountItem(const string_t& n, const string_t& d)
            : base_type(), name(n), description(d) { }
        AccountItem(const string_t& n, const string_t& d, time_t t)
            : base_type(t), name(n), description(d) { }
    };

    struct PlatformItem : public ItemBase<credential_type::ct_platform> {
        string_t name, url, description;

        PlatformItem(const string_t& n, const string_t& u, const string_t& d)
            : base_type(), name(n), url(u), description(d) { }
        PlatformItem(const string_t& n, const string_t& u, const string_t& d, time_t t)
            : base_type(t), name(n), url(u), description(d) { }
    };

    inline bool operator == (const PlatformItem& a, const PlatformItem& b) { return a.name == b.name; }
    inline bool operator == (const AccountItem& a, const AccountItem& b) { return a.name == b.name; }
    inline bool operator == (const PairItem& a, const PairItem& b) { return a.key == b.key; }

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

    class CredentialMgr : public ItemBase<credential_type::ct_credential>, public list_t<platform_t>
    {
    private:

        string_t username, version, description;

    public:

        CredentialMgr() = default;

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
