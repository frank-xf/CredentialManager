#ifndef _bnb_Credential_H_
#define _bnb_Credential_H_

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

    enum class credential_type : unsigned char { ct_credential, ct_platform, ct_account, ct_property };

    template<typename _Ty1, typename _Ty2>
    struct pair_type
    {
        pair_type() = default;
        explicit pair_type(const _Ty1& key) : m_Key(key) { }
        pair_type(const _Ty1& key, const _Ty2& value) : m_Key(key), m_Value(value) { }

        _Ty1 m_Key;
        _Ty2 m_Value;
    };

    template<typename _Ty1, typename _Ty2>
    class list_type
    {
    public:

        using key_type = _Ty1;
        using value_type = _Ty2;
        using data_type = pair_type<key_type, value_type>;

    private:

        class node_type
        {
        private:

            node_type(const node_type&) = delete;
            node_type& operator=(const node_type&) = delete;

        public:

            node_type() = default;
            explicit node_type(const key_type& key) : m_Pair(key) { }
            node_type(const key_type& key, const value_type& value) : m_Pair(key, value) { }

            data_type  m_Pair;
            node_type* m_Next{ nullptr };
            node_type* m_Prev{ nullptr };

        };

        node_type* m_Head{ nullptr };
        size_t m_nCount{ 0 };

        list_type(const list_type&) = delete;
        list_type& operator=(const list_type&) = delete;

        data_type* _Update(const key_type& target, const key_type& key)
        {
            data_type* target_ptr = nullptr;
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            {
                if (nullptr == target_ptr)
                {
                    if (&target == &ptr->m_Pair.m_Key || target == ptr->m_Pair.m_Key)
                    {
                        target_ptr = &ptr->m_Pair;
                        continue;
                    }
                }

                if (key == ptr->m_Pair.m_Key)
                    return false;
            }

            if (target_ptr)
                target_ptr->m_Key = key;

            return target_ptr;
        }

        void _Take(node_type* ptr)
        {
            if (m_Head == ptr)
                m_Head = ptr->m_Next;
            else
                ptr->m_Prev->m_Next = ptr->m_Next;

            if (ptr->m_Next)
                ptr->m_Next->m_Prev = ptr->m_Prev;
        }

    public:

        list_type() = default;

        ~list_type() { Clear(); }

        bool IsEmpty() const { return (nullptr == m_Head || 0 == m_nCount); }

        size_t Size() const { return m_nCount; }

        void Clear()
        {
            while (m_Head)
            {
                node_type* ptr = m_Head;
                m_Head = m_Head->m_Next;
                delete ptr;
            }

            m_nCount = 0;
        }

        template<typename _actionT>
        void Foreach(_actionT _actionFunc)
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                _actionFunc(ptr->m_Pair);
        }

        template<typename _actionT>
        void Foreach(_actionT _actionFunc) const
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                _actionFunc(ptr->m_Pair);
        }

        template<typename _actionT>
        bool Action(_actionT _actionFunc)
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                if (_actionFunc(ptr->m_Pair))
                    return true;

            return false;
        }

        template<typename _actionT>
        bool Action(_actionT _actionFunc) const
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                if (_actionFunc(ptr->m_Pair))
                    return true;

            return false;
        }

        data_type* Find(const key_type& key)
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                if (&key == &ptr->m_Pair.m_Key || key == ptr->m_Pair.m_Key)
                    return &ptr->m_Pair;

            return nullptr;
        }

        const data_type* Find(const key_type& key) const
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                if (&key == &ptr->m_Pair.m_Key || key == ptr->m_Pair.m_Key)
                    return &ptr->m_Pair;

            return nullptr;
        }

        data_type* Add(const key_type& key)
        {
            node_type* last_ptr = nullptr;

            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            {
                if (ptr->m_Pair.m_Key == key) return nullptr;

                last_ptr = ptr;
            }

            node_type* ptr = new node_type(key);
            ptr->m_Prev = last_ptr;

            if (last_ptr)
                last_ptr->m_Next = ptr;
            else
                m_Head = ptr;

            ++m_nCount;

            return &ptr->m_Pair;
        }

        data_type* Add(const key_type& key, const value_type& value)
        {
            node_type* last_ptr = nullptr;

            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            {
                if (ptr->m_Pair.m_Key == key) return nullptr;

                last_ptr = ptr;
            }

            node_type* ptr = new node_type(key, value);
            ptr->m_Prev = last_ptr;

            if (last_ptr)
                last_ptr->m_Next = ptr;
            else
                m_Head = ptr;

            ++m_nCount;

            return &ptr->m_Pair;
        }

        bool Update(const key_type& target, const key_type& key)
        {
            return (_Update(target, key));
        }

        bool Update(const key_type& target, const key_type& key, const value_type& value)
        {
            if (auto ptr = _Update(target, key))
            {
                ptr->m_Value = value;
                return true;
            }

            return false;
        }

        bool Remove(const key_type& key)
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            {
                if (&key == &ptr->m_Pair.m_Key || key == ptr->m_Pair.m_Key)
                {
                    _Take(ptr);
                    --m_nCount;
                    delete ptr;

                    return true;
                }
            }

            return false;
        }

        template<typename _CompareT>
        void Sort(_CompareT _compareFunc)
        {
            if (m_Head)
            {
                node_type* new_header = m_Head;
                node_type* target_ptr = m_Head->m_Next;

                new_header->m_Prev = nullptr;
                new_header->m_Next = nullptr;

                while (target_ptr)
                {
                    node_type* next_ptr = target_ptr->m_Next;

                    for (node_type* ptr = new_header; ptr; ptr = ptr->m_Next)
                    {
                        if (_compareFunc(target_ptr->m_Pair, ptr->m_Pair))
                        {
                            target_ptr->m_Prev = ptr->m_Prev;
                            target_ptr->m_Next = ptr;

                            if (new_header == ptr)
                                new_header = target_ptr;
                            else
                                ptr->m_Prev->m_Next = target_ptr;

                            ptr->m_Prev = target_ptr;

                            break;
                        }
                    }

                    target_ptr = next_ptr;
                }

                m_Head = new_header;
            }
        }

        void Sort()
        {
            Sort([](const data_type& left, const data_type& right) { return left < right; });
        }

        bool Move(const key_type& key, unsigned int index)
        {
            node_type* target_ptr = nullptr;
            node_type* insert_ptr = nullptr;
            node_type* end_ptr = nullptr;

            unsigned int i = 0;

            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            {
                if (nullptr == target_ptr)
                    if (&key == &ptr->m_Pair.m_Key || key == ptr->m_Pair.m_Key)
                        target_ptr = ptr;

                if (nullptr == insert_ptr)
                    if (index == i)
                        insert_ptr = ptr;

                if (target_ptr && insert_ptr)
                    break;

                end_ptr = ptr;

                ++i;
            }

            if (target_ptr)
            {
                if (insert_ptr)
                {
                    if (target_ptr != insert_ptr)
                    {
                        _Take(target_ptr);

                        target_ptr->m_Prev = insert_ptr->m_Prev;
                        target_ptr->m_Next = insert_ptr;

                        if (m_Head == insert_ptr)
                            m_Head = target_ptr;
                        else
                            insert_ptr->m_Prev->m_Next = target;

                        insert_ptr->m_Prev = target_ptr;
                    }
                }
                else
                {
                    if (target_ptr != end_ptr)
                    {
                        _Take(target_ptr);

                        end_ptr->m_Next = target_ptr;
                        target_ptr->m_Prev = end_ptr;
                        target_ptr->m_Next = nullptr;
                    }
                }

                return true;
            }

            return false;
        }

    };

    struct credential_base
    {
    protected:

        static size_t _credential_id;

        const credential_type m_Type;
        const size_t m_ID;

        credential_base(credential_type type) : m_Type(type), m_ID(_credential_id++) { }
        credential_base(const credential_base& other) : m_Type(other.m_Type), m_ID(_credential_id++) { }
        credential_base& operator=(const credential_base& other) { return *this; }

    public:

        size_t GetID() const { return m_ID; }
        credential_type GetType() const { return m_Type; }

    };

    struct platform_type : public credential_base
    {
        platform_type(const string_type& name = string_type(), const string_type& url = string_type(), const string_type& comment = string_type())
            : credential_base(credential_type::ct_platform)
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
            : credential_base(credential_type::ct_account)
            , m_strName(name)
            , m_strComment(comment)
        { }

        string_type m_strName;
        string_type m_strComment;
    };

    struct property_key : public credential_base
    {
        property_key(const string_type& name = string_type())
            : credential_base(credential_type::ct_property)
            , m_strName(name)
        { }

        string_type m_strName;
    };

    struct property_value
    {
        property_value(const string_type& value = string_type()) : m_strName(value) { }

        string_type m_strName;
    };

    inline bool operator < (const platform_type& a, const platform_type& b) { return a.m_strName < b.m_strName; }
    inline bool operator < (const account_type& a, const account_type& b) { return a.m_strName < b.m_strName; }
    inline bool operator < (const property_key& a, const property_key& b) { return a.m_strName < b.m_strName; }
    inline bool operator < (const property_value& a, const property_value& b) { return a.m_strName < b.m_strName; }
    inline bool operator > (const platform_type& a, const platform_type& b) { return (b < a); }
    inline bool operator > (const account_type& a, const account_type& b) { return (b < a); }
    inline bool operator > (const property_key& a, const property_key& b) { return (b < a); }
    inline bool operator > (const property_value& a, const property_value& b) { return (b < a); }
    inline bool operator == (const platform_type& a, const platform_type& b) { return a.m_strName == b.m_strName; }
    inline bool operator == (const account_type& a, const account_type& b) { return a.m_strName == b.m_strName; }
    inline bool operator == (const property_key& a, const property_key& b) { return a.m_strName == b.m_strName; }
    inline bool operator == (const property_value& a, const property_value& b) { return a.m_strName == b.m_strName; }
    inline bool operator != (const platform_type& a, const platform_type& b) { return !(a == b); }
    inline bool operator != (const account_type& a, const account_type& b) { return !(a == b); }
    inline bool operator != (const property_key& a, const property_key& b) { return !(a == b); }
    inline bool operator != (const property_value& a, const property_value& b) { return !(a == b); }

    using property_list = list_type<property_key, property_value>;
    using account_list = list_type<account_type, property_list>;
    using platform_list = list_type<platform_type, account_list>;

    class Credential : public credential_base
    {
        string_type m_strWord;
        string_type m_strUser;
        string_type m_strComment;
        unsigned long long m_ullTime{ 0 };

        platform_list m_List;

        Credential(const Credential&) = delete;
        Credential& operator=(const Credential&) = delete;

    public:

        Credential() : credential_base(credential_type::ct_credential) { }
        explicit Credential(const string_type& strWord) : credential_base(credential_type::ct_credential), m_strWord(strWord) { }

        void Clear();

        bool IsValid() const { return !(m_strUser.empty() || m_strWord.empty()); }

        platform_list& List() { return m_List; }
        const platform_list& List() const { return m_List; }

        const string_type& GetWord() const { return m_strWord; }
        const string_type& GetUser() const { return m_strUser; }
        const string_type& GetComment() const { return m_strComment; }
        unsigned long long GetTime() const { return m_ullTime; }
        void SetWord(const string_type& strWord) { m_strWord = strWord; }
        void SetUser(const string_type& strUser) { m_strUser = strUser; }
        void SetComment(const string_type& strDisplay) { m_strComment = strDisplay; }
        void UpdateTime();
        bool ValidateWord(const string_type& strWord) const;

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
