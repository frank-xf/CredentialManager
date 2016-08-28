#ifndef _bnb_Credential_H_
#define _bnb_Credential_H_

_bnb_space_begin_

inline bool operator == (const string_type& left, const string_type& right)
{
    return (0 == _stricmp(left.c_str(), right.c_str()));
}

inline bool operator < (const string_type& left, const string_type& right)
{
    return (_stricmp(left.c_str(), right.c_str()) < 0);
}

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
inline bool operator < (const pair_type<_Ty1, _Ty2>& left, const pair_type<_Ty1, _Ty2>& right)
{
    return left.m_Key < right.m_Key;
}

template<typename _Ty1, typename _Ty2>
inline bool operator > (const pair_type<_Ty1, _Ty2>& left, const pair_type<_Ty1, _Ty2>& right)
{
    return right.m_Key < left.m_Key;
}

template<typename _Ty1, typename _Ty2>
inline bool operator == (const pair_type<_Ty1, _Ty2>& left, const pair_type<_Ty1, _Ty2>& right)
{
    return (left.m_Key == right.m_Key);
}

template<typename _Ty1, typename _Ty2>
inline bool operator != (const pair_type<_Ty1, _Ty2>& left, const pair_type<_Ty1, _Ty2>& right)
{
    return !(left == right);
}

template<typename _Ty1, typename _Ty2>
class list_type
{
    using key_type = _Ty1;
    using value_type = _Ty2;
    using data_type = pair_type<key_type, value_type>;

    struct node_type
    {
        node_type() : m_Next(nullptr) { }
        node_type(const key_type& key) : m_Pair(key), m_Next(nullptr) { }

        data_type  m_Pair;
        node_type* m_Next;
    };

    node_type* m_Head{ nullptr };
    size_t m_nCount{ 0 };

    list_type(const list_type&) = delete;
    list_type& operator=(const list_type&) = delete;

public:

    list_type() = default;

    ~list_type()
    {
        Clear();
    }

    bool IsEmpty() const { return (!m_Head || 0 == m_nCount); }

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

    node_type* Head() { return m_Head; }
    const node_type* Head() const { return m_Head; }

    data_type* Insert(const key_type& key)
    {
        node_type* prev = nullptr;

        for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
        {
            if (ptr->m_Pair.m_Key == key) return nullptr;

            if (ptr->m_Pair.m_Key < key)
                prev = ptr;
            else
                break;
        }

        node_type* ptr = new node_type(key);

        if (prev)
        {
            ptr->m_Next = prev->m_Next;
            prev->m_Next = ptr;
        }
        else
        {
            ptr->m_Next = m_Head;
            m_Head = ptr;
        }

        ++m_nCount;

        return &ptr->m_Pair;
    }

    bool Remove(const key_type& key)
    {
        for (node_type *prev = nullptr, *curr = m_Head; curr; curr = curr->m_Next)
        {
            if (curr->m_Pair.m_Key == key)
            {
                if (curr == m_Head)
                    m_Head = curr->m_Next;
                else
                    prev->m_Next = curr->m_Next;

                --m_nCount;

                delete curr;
                return true;
            }

            prev = curr;
        }

        return false;
    }

    bool CanUpdate(const key_type& key) const
    {
        for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            if (&ptr->m_Pair.m_Key != &key)
                if (ptr->m_Pair.m_Key == key)
                    return false;

        return true;
    }

    data_type* Find(const key_type& key)
    {
        for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            if (ptr->m_Pair.m_Key == key)
                return &ptr->m_Pair;

        return nullptr;
    }

};

struct account_data
{
    string_type m_strDisplay;

    explicit account_data(const string_type& display = string_type())
        : m_strDisplay(display)
    { }
};

struct platform_data
{
    string_type m_strUrl;
    string_type m_strDisplay;

    explicit platform_data(const string_type& url = string_type(), const string_type& display = string_type())
        : m_strUrl(url), m_strDisplay(display)
    { }
};

using account_type = pair_type<string_type, account_data>;
using platform_type = pair_type<string_type, platform_data>;

using property_list = list_type<string_type, string_type>;
using account_list = list_type<account_type, property_list>;
using platform_list = list_type<platform_type, account_list>;

enum class result_type : unsigned char
{
    rt_success,
    rt_file_invalid,
    rt_file_error,
    rt_password_invalid,
    rt_password_error
};

inline property_list::data_type* property_list::Insert(const string_type& key)
{
    node_type* last = nullptr;

    for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
    {
        if (ptr->m_Pair.m_Key == key) return nullptr;

        last = ptr;
    }

    node_type* ptr = new node_type(key);

    if (last)
        last->m_Next = ptr;
    else
        m_Head = ptr;

    ++m_nCount;

    return &ptr->m_Pair;
}

class Credential
{
    string_type m_strWord;
    string_type m_strUser;
    unsigned long long m_ullTime;

    platform_list m_List;

    Credential(const Credential&) = delete;
    Credential& operator=(const Credential&) = delete;

public:

    Credential() = default;
    explicit Credential(const string_type& strWord) : m_strWord(strWord) { }

    void Clear();
    
    bool IsValid() const { return !(m_strUser.empty() || m_strWord.empty()); }

    platform_list& List() { return m_List; }
    const platform_list& List() const { return m_List; }
    
    const string_type& GetWord() const { return m_strWord; }
    const string_type& GetUser() const { return m_strUser; }
    unsigned long long GetTime() const { return m_ullTime; }
    void SetWord(const string_type& strWord) { m_strWord = strWord; }
    void SetUser(const string_type& strUser) { m_strUser = strUser; }
    void UpdateTime();

    bool FromXml(const memory_type& mt);
    bool ToXml(memory_type& mt) const;

    result_type Load(const char* file);
    bool Save(const char* file) const;

    static bool Encoding(memory_type& mt, const byte_t* key, size_t n);
    static result_type Decoding(memory_type& mt, const byte_t* key, size_t n);
    static result_type CheckFile(const char* file, memory_type* dst);

};

_bnb_space_end_

#endif