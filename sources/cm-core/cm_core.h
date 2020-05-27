#pragma once

#include <string>
#include <vector>

#include "cm_type.h"

namespace xf::credential
{

    const char_t* core_version();

    time_t CurrentTime();

    template<credential_type ct>
    struct ItemBase
    {
        using base_type = ItemBase;

        static constexpr credential_type type = ct;

        time_t time;

        ItemBase() : ItemBase(CurrentTime()) { }
        ItemBase(time_t t) : time(t) { }
        ~ItemBase() { }

        void Updated() {
            time = CurrentTime();
        }

    };  // class ItemBase

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
    class credential_t;

    struct pair_t : public node_t<PairItem, pair_t, account_t> {
        using node_base::node_base;
    };

    struct account_t : public node_t<AccountItem, account_t, platform_t>, public list_t<pair_t> {
        using node_base::node_base;
        void Event(event_type at, credential_type ct) override;
    };

    struct platform_t : public node_t<PlatformItem, platform_t, credential_t>, public list_t<account_t> {
        using node_base::node_base;
        void Event(event_type at, credential_type ct) override;
    };

    class credential_t final : private ItemBase<credential_type::ct_credential>, public list_t<platform_t>
    {
    private:

        string_t username, description, version{ core_version() };

    public:

        using base_type::type;
        using base_type::Updated;

        credential_t() = default;

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

        static bool ValidateName(const string_t& strName);

    };  // class credential_t

}   // namespace xf::credential
