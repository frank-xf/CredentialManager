#pragma once

#include <string>

namespace xf::credential
{
    using char_t = char;
    using byte_t = std::uint8_t;
    using time_t = std::uint64_t;

    template<typename CharType>
    using _string_type = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>;

    using string_t = _string_type<char_t>;
    using memory_t = _string_type<char>;

    inline const char_t* version() { return "1.2.0"; }

    time_t CurrentTime();

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

    bool LoadFile(const char* file, memory_t& data);
    bool SaveFile(const char* file, const memory_t& data);

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

    struct pair_t;
    struct account_t;
    struct platform_t;
    class CredentialMgr;

    struct pair_t : public node_t<PairItem, pair_t, account_t> {
        using base_type::base_type;
    };

    struct account_t : public node_t<AccountItem, account_t, platform_t>, public list_t<pair_t> {
        using base_type::base_type;
        void Event(event_type at, credential_type ct) override;
    };

    struct platform_t : public node_t<PlatformItem, platform_t, CredentialMgr>, public list_t<account_t> {
        using base_type::base_type;
        void Event(event_type at, credential_type ct) override;
    };

    class CredentialMgr : public ItemBase<credential_type::ct_credential>, public list_t<platform_t>
    {
    private:

        string_t username, version, description;

    public:

        CredentialMgr() = default;

        void Event(event_type at, credential_type ct) override { Updated(); }

        bool Serialize(string_t& str) const;
        bool Deserialize(const string_t& str);

        bool Load(const char* file)
        {
            return Load(file, [](memory_t&, const byte_t*, std::size_t) { return true; }, nullptr, 0);
        }

        bool Save(const char* file) const
        {
            return Save(file, [](memory_t&, const byte_t*, std::size_t) { return true; }, nullptr, 0);
        }

        template<typename FuncType>
        bool Load(const char* file, FuncType decoding, const byte_t* key, std::size_t n)
        {
            memory_t data;
            if (!LoadFile(file, data)) return false;

            if (!decoding(data, key, n)) return false;

            return Deserialize(memory_to_string(data));
        }

        template<typename FuncType>
        bool Save(const char* file, FuncType encodeing, const byte_t* key, std::size_t n) const
        {
            string_t str;
            if (!Serialize(str)) return false;

            memory_t data = string_to_memory(str);
            if (!encodeing(data, key, n)) return false;

            return SaveFile(file, data);
        }

        static bool Encoding(memory_t& data, const byte_t* key, std::size_t n);
        static bool Decoding(memory_t& data, const byte_t* key, std::size_t n);
        static bool ValidateName(const string_t& strName);
        static bool Check(const char* file);

    };  // class CredentialMgr

}   // namespace xf::credential
