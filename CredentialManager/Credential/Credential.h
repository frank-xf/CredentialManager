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
    /*
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

        node_type* m_Header{ nullptr };
        node_type* m_End{ nullptr };
        size_t m_nCount{ 0 };

        list_type(const list_type&) = delete;
        list_type& operator=(const list_type&) = delete;

        data_type* _Traversal(const key_type& target, const key_type& key)
        {
            data_type* target_ptr = nullptr;

            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
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
                    return nullptr;
            }

            return target_ptr;
        }

        void _Take(node_type* ptr)
        {
            if (m_Header == ptr)
                m_Header = ptr->m_Next;
            else
                ptr->m_Prev->m_Next = ptr->m_Next;

            if (m_End == ptr)
                m_End = ptr->m_Prev;
            else
                ptr->m_Next->m_Prev = ptr->m_Prev;
        }

        void _InsertAfter(node_type* target_ptr, node_type* insert_ptr)
        {
            target_ptr->m_Prev = insert_ptr;
            target_ptr->m_Next = insert_ptr->m_Next;

            if (m_End == insert_ptr)
                m_End = target_ptr;
            else
                insert_ptr->m_Next->m_Prev = target_ptr;

            insert_ptr->m_Next = target_ptr;
        }

        void _InsertBefore(node_type* target_ptr, node_type* insert_ptr)
        {
            target_ptr->m_Prev = insert_ptr->m_Prev;
            target_ptr->m_Next = insert_ptr;

            if (m_Header == insert_ptr)
                m_Header = target_ptr;
            else
                insert_ptr->m_Prev->m_Next = target_ptr;

            insert_ptr->m_Prev = target_ptr;
        }

    public:

        list_type() = default;

        ~list_type() { Clear(); }

        bool IsEmpty() const { return (nullptr == m_Header || nullptr == m_End || 0 == m_nCount); }

        size_t Size() const { return m_nCount; }

        void Clear()
        {
            while (m_Header)
            {
                node_type* ptr = m_Header;
                m_Header = m_Header->m_Next;
                delete ptr;
            }

            m_End = nullptr;
            m_nCount = 0;
        }

        template<typename _actionT>
        void PreorderTraversal(_actionT _actionFunc)
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                _actionFunc(ptr->m_Pair);
        }

        template<typename _actionT>
        void PreorderTraversal(_actionT _actionFunc) const
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                _actionFunc(ptr->m_Pair);
        }

        template<typename _actionT>
        void PostorderTraversal(_actionT _actionFunc)
        {
            for (node_type* ptr = m_End; ptr; ptr = ptr->m_Prev)
                _actionFunc(ptr->m_Pair);
        }

        template<typename _actionT>
        void PostorderTraversal(_actionT _actionFunc) const
        {
            for (node_type* ptr = m_End; ptr; ptr = ptr->m_Prev)
                _actionFunc(ptr->m_Pair);
        }

        template<typename _actionT>
        bool PreorderAction(_actionT _actionFunc)
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                if (_actionFunc(ptr->m_Pair))
                    return true;

            return false;
        }

        template<typename _actionT>
        bool PreorderAction(_actionT _actionFunc) const
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                if (_actionFunc(ptr->m_Pair))
                    return true;

            return false;
        }

        template<typename _actionT>
        bool PostorderAction(_actionT _actionFunc)
        {
            for (node_type* ptr = m_End; ptr; ptr = ptr->m_Prev)
                if (_actionFunc(ptr->m_Pair))
                    return true;

            return false;
        }

        template<typename _actionT>
        bool PostorderAction(_actionT _actionFunc) const
        {
            for (node_type* ptr = m_End; ptr; ptr = ptr->m_Prev)
                if (_actionFunc(ptr->m_Pair))
                    return true;

            return false;
        }

        data_type* Find(const key_type& key)
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                if (&key == &ptr->m_Pair.m_Key || key == ptr->m_Pair.m_Key)
                    return &ptr->m_Pair;

            return nullptr;
        }

        const data_type* Find(const key_type& key) const
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                if (&key == &ptr->m_Pair.m_Key || key == ptr->m_Pair.m_Key)
                    return &ptr->m_Pair;

            return nullptr;
        }

        data_type* PushBack(const key_type& key)
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                if (ptr->m_Pair.m_Key == key)
                    return nullptr;

            node_type* ptr = new node_type(key);

            if (IsEmpty())
            {
                m_Header = ptr;
                m_End = ptr;
            }
            else
            {
                ptr->m_Prev = m_End;
                m_End->m_Next = ptr;
                m_End = ptr;
            }

            ++m_nCount;

            return &ptr->m_Pair;
        }

        data_type* PushBack(const key_type& key, const value_type& value)
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                if (ptr->m_Pair.m_Key == key)
                    return nullptr;

            node_type* ptr = new node_type(key, value);

            if (IsEmpty())
            {
                m_Header = ptr;
                m_End = ptr;
            }
            else
            {
                ptr->m_Prev = m_End;
                m_End->m_Next = ptr;
                m_End = ptr;
            }

            ++m_nCount;

            return &ptr->m_Pair;
        }

        data_type* PushFront(const key_type& key)
        {
            for (node_type* ptr = m_End; ptr; ptr = ptr->m_Prev)
                if (ptr->m_Pair.m_Key == key)
                    return nullptr;

            node_type* ptr = new node_type(key);

            if (IsEmpty())
            {
                m_Header = ptr;
                m_End = ptr;
            }
            else
            {
                ptr->m_Next = m_Header;
                m_Header->m_Prev = ptr;
                m_Header = ptr;
            }

            ++m_nCount;

            return &ptr->m_Pair;
        }

        data_type* PushFront(const key_type& key, const value_type& value)
        {
            for (node_type* ptr = m_End; ptr; ptr = ptr->m_Prev)
                if (ptr->m_Pair.m_Key == key)
                    return nullptr;

            node_type* ptr = new node_type(key, value);

            if (IsEmpty())
            {
                m_Header = ptr;
                m_End = ptr;                
            }
            else
            {
                ptr->m_Next = m_Header;
                m_Header->m_Prev = ptr;
                m_Header = ptr;
            }

            ++m_nCount;

            return &ptr->m_Pair;
        }

        bool Update(const key_type& target, const key_type& key)
        {
            if (auto ptr = _Traversal(target, key))
            {
                ptr->m_Key = key;
                return true;
            }

            return false;
        }

        bool Update(const key_type& target, const key_type& key, const value_type& value)
        {
            if (auto ptr = _Traversal(target, key))
            {
                ptr->m_Key = key;
                ptr->m_Value = value;
                return true;
            }

            return false;
        }

        bool Remove(const key_type& key)
        {
            for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
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

        template<typename _findT>
        size_type RemoveIf(_findT _findFunc)
        {
            size_type nResult = 0;

            for (node_type* ptr = m_Header; ptr; )
            {
                node_type* next_ptr = ptr->m_Next;

                if (_findFunc(ptr->m_Pair))
                {
                    _Take(ptr);
                    --m_nCount;
                    delete ptr;

                    ++nResult;
                }

                ptr = next_ptr;
            }

            return nResult;
        }

        template<typename _CompareT>
        void Sort(_CompareT _compareFunc)
        {
            if (m_Header)
            {
                if (node_type* target_ptr = m_Header->m_Next)
                {
                    m_End = m_Header;
                    m_Header->m_Prev = nullptr;
                    m_Header->m_Next = nullptr;

                    do {
                        node_type* next_ptr = target_ptr->m_Next;

                        if (![this, &_compareFunc, &target_ptr]() {
                                for (node_type* insert_ptr = m_Header; insert_ptr; insert_ptr = insert_ptr->m_Next)
                                {
                                    if (_compareFunc(target_ptr->m_Pair, insert_ptr->m_Pair))
                                    {
                                        _InsertBefore(target_ptr, insert_ptr);
                                        return true;
                                    }
                                }

                                return false;
                            }())
                        {
                            target_ptr->m_Next = nullptr;
                            target_ptr->m_Prev = m_End;
                            m_End->m_Next = target_ptr;
                            m_End = target_ptr;
                        }

                        target_ptr = next_ptr;
                    } while (target_ptr)
                }
            }
        }

        void Sort()
        {
            Sort([](const data_type& left, const data_type& right) { return left < right; });
        }

        bool Move(const key_type& key, int offset)
        {
            if (0 < offset)
            {
                for (node_type* target_ptr = m_Header; target_ptr; target_ptr = target_ptr->m_Next)
                {
                    if (&key == &target_ptr->m_Pair.m_Key || key == target_ptr->m_Pair.m_Key)
                    {
                        node_type* insert_ptr = target_ptr;
                        for (int i = 0; i < offset && insert_ptr != m_End; ++i)
                            insert_ptr = insert_ptr->m_Next;

                        if (target_ptr != insert_ptr)
                        {
                            _Take(target_ptr);
                            _InsertAfter(target_ptr, insert_ptr);
                        }

                        return true;
                    }
                }
            }
            else if (offset < 0)
            {
                for (node_type* target_ptr = m_End; target_ptr; target_ptr = target_ptr->m_Prev)
                {
                    if (&key == &target_ptr->m_Pair.m_Key || key == target_ptr->m_Pair.m_Key)
                    {
                        node_type* insert_ptr = target_ptr;
                        for (int i = 0; offset < i && insert_ptr != m_Header; --i)
                            insert_ptr = insert_ptr->m_Prev;

                        if (target_ptr != insert_ptr)
                        {
                            _Take(target_ptr);
                            _InsertBefore(target_ptr, insert_ptr);
                        }

                        return true;
                    }
                }
            }
            else
            {
                for (node_type* ptr = m_Header; ptr; ptr = ptr->m_Next)
                    if (&key == &ptr->m_Pair.m_Key || key == ptr->m_Pair.m_Key)
                        return true;
            }

            return false;
        }

    };
    */
    struct credential_base
    {
    protected:

        static size_t _credential_id;

        const credential_enum m_Type;
        const size_t m_ID;

        credential_base(credential_enum type) : m_Type(type), m_ID(_credential_id++) { }
        credential_base(const credential_base& other) : m_Type(other.m_Type), m_ID(_credential_id++) { }
        credential_base& operator=(const credential_base& other) { return *this; }

    public:

        size_t GetID() const { return m_ID; }
        credential_enum GetType() const { return m_Type; }

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
    /*
    using property_list = list_type<property_key, property_value>;
    using account_list = list_type<account_type, property_list>;
    using platform_list = list_type<platform_type, account_list>;
    */
    /*
    using account_node = tree_node<account_type, platform_node, property_node>;
    using property_node = list_node<property_type, account_node>;
    using platform_node = tree_node<platform_type, Credential, account_node>;
    */
    class Credential;

    using property_node = list_node<property_type>;
    using account_node = tree_node<account_type, property_node>;
    using platform_node = tree_node<platform_type, account_node>;

    class Credential : public credential_base, public list_type<platform_node>
    {
        string_type m_strWord;
        string_type m_strUser;
        string_type m_strComment;
        unsigned long long m_ullTime{ 0 };

        Credential(const Credential&) = delete;
        Credential& operator=(const Credential&) = delete;

        static unsigned long long _GetTime();

    public:

        Credential(const string_type& user = string_type(), const string_type& word = string_type(), const string_type& comment = string_type())
            : credential_base(credential_enum::ct_credential)
            , m_strUser(user)
            , m_strWord(word)
            , m_strComment(comment)
            , m_ullTime(_GetTime())
        { }

        void Clear();

        bool IsValid() const { return !(m_strUser.empty() || m_strWord.empty()); }

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
