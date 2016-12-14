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
   
    class credential_base
    {
    protected:

        const credential_enum m_Type;

        credential_base(credential_enum type) : m_Type(type) { }
        credential_base(const credential_base& other) : m_Type(other.m_Type) { }
        credential_base& operator=(const credential_base& other) { return *this; }

    public:

        credential_enum GetType() const { return m_Type; }

    };

    class credential_type : public credential_base
    {
    private:

        static unsigned long long _GetTime();

    public:

        credential_type(const string_type& user = string_type(), const string_type& word = string_type(), const string_type& comment = string_type())
            : credential_base(credential_enum::ct_credential)
            , m_strUser(user)
            , m_strWord(word)
            , m_strComment(comment)
            , m_ullTime(_GetTime())
        { }

        const string_type& GetWord() const { return m_strWord; }
        const string_type& GetUser() const { return m_strUser; }
        const string_type& GetComment() const { return m_strComment; }
        unsigned long long GetTime() const { return m_ullTime; }

        bool IsValid() const { return !(m_strUser.empty() || m_strWord.empty()); }
        bool ValidateWord(const string_type& strWord) const;
        void UpdateTime();

    private:

        string_type m_strWord;
        string_type m_strUser;
        string_type m_strComment;
        unsigned long long m_ullTime;

        friend class Credential;
    };

    class platform_type : public credential_base
    {
    public:

        platform_type(const string_type& name = string_type(), const string_type& url = string_type(), const string_type& comment = string_type())
            : credential_base(credential_enum::ct_platform)
            , m_strName(name)
            , m_strUrl(url)
            , m_strComment(comment)
        { }

        const string_type& GetName() const { return m_strName; }
        const string_type& GetUrl() const { return m_strUrl; }
        const string_type& GetComment() const { return m_strComment; }

    private:

        string_type m_strName;
        string_type m_strUrl;
        string_type m_strComment;
    };

    class account_type : public credential_base
    {
    public:

        account_type(const string_type& name = string_type(), const string_type& comment = string_type())
            : credential_base(credential_enum::ct_account)
            , m_strName(name)
            , m_strComment(comment)
        { }

        const string_type& GetName() const { return m_strName; }
        const string_type& GetComment() const { return m_strComment; }

    private:

        string_type m_strName;
        string_type m_strComment;
    };

    class property_type : public credential_base
    {
    public:

        property_type(const string_type& key = string_type(), const string_type& value = string_type())
            : credential_base(credential_enum::ct_property)
            , m_strKey(key)
            , m_strValue(value)
        { }

        const string_type& GetKey() const { return m_strKey; }
        const string_type& GetValue() const { return m_strValue; }

    private:

        string_type m_strKey;
        string_type m_strValue;
    };

    inline bool operator < (const platform_type& a, const platform_type& b) { return a.GetName() < b.GetName(); }
    inline bool operator < (const account_type& a, const account_type& b) { return a.GetName() < b.GetName(); }
    inline bool operator < (const property_type& a, const property_type& b) { return a.GetKey() < b.GetKey(); }
    inline bool operator > (const platform_type& a, const platform_type& b) { return (b < a); }
    inline bool operator > (const account_type& a, const account_type& b) { return (b < a); }
    inline bool operator > (const property_type& a, const property_type& b) { return (b < a); }
    inline bool operator == (const platform_type& a, const platform_type& b) { return a.GetName() == b.GetName(); }
    inline bool operator == (const account_type& a, const account_type& b) { return a.GetName() == b.GetName(); }
    inline bool operator == (const property_type& a, const property_type& b) { return a.GetKey() == b.GetKey(); }
    inline bool operator != (const platform_type& a, const platform_type& b) { return !(a == b); }
    inline bool operator != (const account_type& a, const account_type& b) { return !(a == b); }
    inline bool operator != (const property_type& a, const property_type& b) { return !(a == b); }

    class property_node : public list_node<property_type, property_node>
    {
    public:

        using base_type::base_type;
    };

    class account_node : public list_node<account_type, account_node>, public list_type<property_node>
    {
    public:

        using base_type::base_type;

        void Updated(param_type aType) override;
    };

    class platform_node : public list_node<platform_type, platform_node>, public list_type<account_node>
    {
    public:

        using base_type::base_type;

        void Updated(param_type aType) override;
        void Updated(param_type aType, param_type cType);
    };

    class Credential : public index_type, public item_base<credential_type>, public list_type<platform_node>
    {
    private:

        Credential(const Credential&) = delete;
        Credential& operator=(const Credential&) = delete;

    public:

        Credential() = default;

        void Clear();
        void SetWord(const string_type& strWord);
        void SetUser(const string_type& strUser);
        void SetComment(const string_type& strDisplay);

        void Updated(param_type aType) override;
        void Updated(param_type aType, param_type cType);

        bool FromXml(const memory_type& mt);
        bool ToXml(memory_type& mt) const;

        bool Load(const char* file);
        bool Save(const char* file) const;

        using list_base::FindByID;

        account_node* FindByID(id_type id1, id_type id2);
        property_node* FindByID(id_type id1, id_type id2, id_type id3);
        const account_node* FindByID(id_type id1, id_type id2) const;
        const property_node* FindByID(id_type id1, id_type id2, id_type id3) const;

        static bool Encoding(memory_type& mt, const byte_type* key, size_t n);
        static bool Decoding(memory_type& mt, const byte_type* key, size_t n);
        static bool CheckFile(const char* file, memory_type* dst);

        class delegate_type
        {
        private:

            delegate_type(const delegate_type&) = delete;
            delegate_type& operator=(const delegate_type&) = delete;

        protected:

            delegate_type() = default;

        public:

            virtual bool OnAddCredential() { return false; }
            virtual bool OnAddPlatform(id_type id1) { return false; }
            virtual bool OnAddAccount(id_type id1, id_type id2) { return false; }
            virtual bool OnAddProperty(id_type id1, id_type id2, id_type id3) { return false; }

            virtual bool OnUpdateCredential(id_type id1) { return false; }
            virtual bool OnUpdatePlatform(id_type id1, id_type id2) { return false; }
            virtual bool OnUpdateAccount(id_type id1, id_type id2, id_type id3) { return false; }
            virtual bool OnUpdateProperty(id_type id1, id_type id2, id_type id3, id_type id4) { return false; }

            virtual bool OnRemoveCredential(id_type id1) { return false; }
            virtual bool OnRemovePlatform(id_type id1, id_type id2) { return false; }
            virtual bool OnRemoveAccount(id_type id1, id_type id2, id_type id3) { return false; }
            virtual bool OnRemoveProperty(id_type id1, id_type id2, id_type id3, id_type id4) { return false; }

            virtual ~delegate_type() = 0 { }
        };

    };

}

#endif
