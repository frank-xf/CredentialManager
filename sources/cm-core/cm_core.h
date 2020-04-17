#pragma once

#include <string>
#include <vector>

namespace xf::credential
{
    using char_t = char;
    using byte_t = std::uint8_t;
    using time_t = std::uint64_t;

    template<typename CharType>
    using _string_type = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>;

    using string_t = _string_type<char_t>;
    // using memory_t = std::vector<std::uint8_t>;

    inline const char_t* version() { return "1.2.0"; }

    time_t CurrentTime();

    /*
    inline memory_t string_to_memory(const string_t& str)
    {
        if constexpr (std::is_same<string_t, memory_t>::value)
            return str;
        else
            return memory_t((memory_t::value_type*)str.c_str(), str.size() * sizeof(string_t::value_type));
    }

    inline string_t memory_to_string(const memory_t& data)
    {
        if constexpr (std::is_same<string_t, memory_t>::value)
            return data;
        else
            return string_t((string_t::value_type*)data.c_str(), data.size() / sizeof(string_t::value_type));
    }
    */
    bool LoadFile(const char* file, string_t& data);
    bool SaveFile(const char* file, const string_t& data);

#include "cm_type.inl"

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
    inline bool operator < (const PlatformItem& a, const PlatformItem& b) { return a.name < b.name; }
    inline bool operator < (const AccountItem& a, const AccountItem& b) { return a.name < b.name; }
    inline bool operator < (const PairItem& a, const PairItem& b) { return a.key < b.key; }

    struct pair_t;
    struct account_t;
    struct platform_t;
    class CredentialMgr;

    struct pair_t : public node_t<PairItem, pair_t, account_t> {
        using node_base::node_base;
    };

    struct account_t : public node_t<AccountItem, account_t, platform_t>, public list_t<pair_t> {
        using node_base::node_base;
        void Event(event_type at, credential_type ct) override;
    };

    struct platform_t : public node_t<PlatformItem, platform_t, CredentialMgr>, public list_t<account_t> {
        using node_base::node_base;
        void Event(event_type at, credential_type ct) override;
    };

    class CredentialMgr final : private ItemBase<credential_type::ct_credential>, public list_t<platform_t>
    {
    private:

        string_t username, description, version{ xf::credential::version() };

    public:

        using base_type::type;
        using base_type::Updated;

        CredentialMgr() = default;

        const string_t& Username() const { return username; }
        const string_t& Version() const { return version; }
        const string_t& Description() const { return description; }
        time_t Time() const { return time; }

        bool SetUsername(const string_t& name);
        void SetDescription(const string_t& desc) { description = desc; Updated(); }

        void Clear() override;
        void Event(event_type et, credential_type ct) override { Updated(); }

        bool Serialize(string_t& str) const;
        bool Deserialize(const string_t& str);

        bool Load(const char* file)
        {
            return Load(file, [](string_t&, const byte_t*, std::size_t) { return true; }, nullptr, 0);
        }

        bool Save(const char* file) const
        {
            return Save(file, [](string_t&, const byte_t*, std::size_t) { return true; }, nullptr, 0);
        }

        template<typename FuncType>
        bool Load(const char* file, FuncType decrypt, const byte_t* key, std::size_t n)
        {
            string_t data;
            return (LoadFile(file, data) && decrypt(data, key, n) && Deserialize(data));
        }

        template<typename FuncType>
        bool Save(const char* file, FuncType encrypt, const byte_t* key, std::size_t n) const
        {
            string_t data;
            return Serialize(data) && encrypt(data, key, n) && SaveFile(file, data);
        }

        static bool ValidateName(const string_t& strName);

    };  // class CredentialMgr

}   // namespace xf::credential
