#ifndef _bnb_Credential_H_
#define _bnb_Credential_H_

#include "Tree.h"

namespace std
{
    template<typename _Ty> class allocator;
    template<typename _Ty> struct char_traits;
    template<typename _Ty, typename _Traits, typename _Alloc> class basic_string;
}

namespace bnb
{
    using char_type = wchar_t;
    using byte_type = unsigned char;
    using size_type = unsigned int;

    using string_type = std::basic_string<char_type, std::char_traits<char_type>, std::allocator<char_type>>;
    using memory_type = std::basic_string<byte_type, std::char_traits<byte_type>, std::allocator<byte_type>>;

    inline bool operator < (const string_type& a, const string_type& b) { return (_wcsicmp(a.c_str(), b.c_str()) < 0); }
    inline bool operator > (const string_type& a, const string_type& b) { return (b < a); }
    inline bool operator == (const string_type& a, const string_type& b) { return (0 == _wcsicmp(a.c_str(), b.c_str())); }
    inline bool operator != (const string_type& a, const string_type& b) { return !(a == b); }

    enum class credential_enum : unsigned char { ct_credential, ct_platform, ct_account, ct_property };
   
    struct credential_base
    {
    protected:

        const credential_enum m_Type;

        credential_base(credential_enum type) : m_Type(type) { }
        credential_base(const credential_base& other) : m_Type(other.m_Type) { }
        credential_base& operator=(const credential_base& other) { return *this; }

    public:

        credential_enum GetType() const { return m_Type; }

    };

    struct credential_type : public credential_base
    {
        string_type m_strWord;
        string_type m_strUser;
        string_type m_strComment;
        unsigned long long m_ullTime;

        credential_type(const string_type& user = string_type(), const string_type& word = string_type(), const string_type& comment = string_type())
            : credential_base(credential_enum::ct_credential)
            , m_strUser(user)
            , m_strWord(word)
            , m_strComment(comment)
            , m_ullTime(_GetTime())
        { }

        bool IsValid() const { return !(m_strUser.empty() || m_strWord.empty()); }

        const string_type& GetWord() const { return m_strWord; }
        const string_type& GetUser() const { return m_strUser; }
        const string_type& GetComment() const { return m_strComment; }
        unsigned long long GetTime() const { return m_ullTime; }

        void SetTime(unsigned long long ut) { m_ullTime = ut; }
        void SetWord(const string_type& strWord) { m_strWord = strWord; }
        void SetUser(const string_type& strUser) { m_strUser = strUser; }
        void SetComment(const string_type& strDisplay) { m_strComment = strDisplay; }

        void UpdateTime();
        bool ValidateWord(const string_type& strWord) const;
        void Clear();

    private:

        static unsigned long long _GetTime();

    };

    struct platform_type : public credential_base
    {
        platform_type(const string_type& name = string_type(), const string_type& url = string_type(), const string_type& comment = string_type())
            : credential_base(credential_enum::ct_platform)
            , m_strName(name)
            , m_strUrl(url)
            , m_strComment(comment)
        { }

        string_type m_strName;
        string_type m_strUrl;
        string_type m_strComment;
    };

    struct account_type : public credential_base
    {
        account_type(const string_type& name = string_type(), const string_type& comment = string_type())
            : credential_base(credential_enum::ct_account)
            , m_strName(name)
            , m_strComment(comment)
        { }

        string_type m_strName;
        string_type m_strComment;
    };

    struct property_type : public credential_base
    {
        property_type(const string_type& key = string_type(), const string_type& value = string_type())
            : credential_base(credential_enum::ct_property)
            , m_strKey(key)
            , m_strValue(value)
        { }

        string_type m_strKey;
        string_type m_strValue;
    };

    inline bool operator < (const platform_type& a, const platform_type& b) { return a.m_strName < b.m_strName; }
    inline bool operator < (const account_type& a, const account_type& b) { return a.m_strName < b.m_strName; }
    inline bool operator < (const property_type& a, const property_type& b) { return a.m_strKey < b.m_strKey; }
    inline bool operator > (const platform_type& a, const platform_type& b) { return (b < a); }
    inline bool operator > (const account_type& a, const account_type& b) { return (b < a); }
    inline bool operator > (const property_type& a, const property_type& b) { return (b < a); }
    inline bool operator == (const platform_type& a, const platform_type& b) { return a.m_strName == b.m_strName; }
    inline bool operator == (const account_type& a, const account_type& b) { return a.m_strName == b.m_strName; }
    inline bool operator == (const property_type& a, const property_type& b) { return a.m_strKey == b.m_strKey; }
    inline bool operator != (const platform_type& a, const platform_type& b) { return !(a == b); }
    inline bool operator != (const account_type& a, const account_type& b) { return !(a == b); }
    inline bool operator != (const property_type& a, const property_type& b) { return !(a == b); }

    class property_node : public list_node<property_type, property_node>
    {
        using base_type::base_type;
    };
    class account_node : public list_node<account_type, account_node>, public list_type<property_node>
    {
        using base_type::base_type;
    };
    class platform_node : public list_node<platform_type, platform_node>, public list_type<account_node>
    {
        using base_type::base_type;
    };

    class Credential : public index_type, public item_base<credential_type>, public list_type<platform_node>
    {
        Credential(const Credential&) = delete;
        Credential& operator=(const Credential&) = delete;

    public:

        Credential() = default;

        void Clear();

        bool FromXml(const memory_type& mt);
        bool ToXml(memory_type& mt) const;

        bool Load(const char* file);
        bool Save(const char* file) const;

        static bool Encoding(memory_type& mt, const byte_type* key, size_t n);
        static bool Decoding(memory_type& mt, const byte_type* key, size_t n);
        static bool CheckFile(const char* file, memory_type* dst);

    };

}

#endif
