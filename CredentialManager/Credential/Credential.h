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
class tree_type
{
    using key_type = _Ty1;
    using value_type = _Ty2;
    using data_type = pair_type<key_type, value_type>;

    class node_type
    {
	private:

		node_type(const node_type&) = delete;
		node_type& operator=(const node_type&) = delete;

	public:

        node_type() : m_Next(nullptr) { }
        node_type(const key_type& key) : m_Pair(key), m_Next(nullptr) { }

        data_type  m_Pair;
        node_type* m_Next;

    };

    node_type* m_Head{ nullptr };
    size_t m_nCount{ 0 };

    tree_type(const tree_type&) = delete;
    tree_type& operator=(const tree_type&) = delete;

public:

    tree_type() = default;

    ~tree_type()
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

    bool SetKey(key_type& src, const key_type& dst)
    {
        for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
            if (&ptr->m_Pair.m_Key != &src)
                if (ptr->m_Pair.m_Key == dst)
                    return false;

        src = dst;
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

enum class credential_type : unsigned char
{
    ct_credential,
    ct_platform,
    ct_account,
    ct_property
};

struct credential_base
{
    const credential_type m_Type;
    string_type m_strName;

protected:

    explicit credential_base(credential_type ct) : m_Type(ct) { }
    credential_base(credential_type ct, const string_type& name) : m_Type(ct), m_strName(name) { }

};

struct platform_type : public credential_base
{
    platform_type(const string_type& name = string_type(), const string_type& url = string_type(), const string_type& display = string_type())
        : credential_base(credential_type::ct_property, name)
        , m_strUrl(url)
        , m_strDisplay(display)
    { }

    string_type m_strUrl;
    string_type m_strDisplay;
};

struct account_type : public credential_base
{
    account_type(const string_type& name = string_type(), const string_type& display = string_type())
        : credential_base(credential_type::ct_property, name)
        , m_strDisplay(display)
    { }

    string_type m_strDisplay;
};

struct property_type : public credential_base
{
    property_type(const string_type& name = string_type())
        : credential_base(credential_type::ct_property, name)
    { }

};

struct property_value
{
    property_value(const string_type& value = string_type()) : m_strValue(value) { }

    string_type m_strValue;
};

inline bool operator < (const platform_type& a, const platform_type& b) { return a.m_strName < b.m_strName; }
inline bool operator < (const account_type& a, const account_type& b) { return a.m_strName < b.m_strName; }
inline bool operator < (const property_type& a, const property_type& b) { return a.m_strName < b.m_strName; }
inline bool operator < (const property_value& a, const property_value& b) { return a.m_strValue < b.m_strValue; }
inline bool operator > (const platform_type& a, const platform_type& b) { return b.m_strName < a.m_strName; }
inline bool operator > (const account_type& a, const account_type& b) { return b.m_strName < a.m_strName; }
inline bool operator > (const property_type& a, const property_type& b) { return b.m_strName < a.m_strName; }
inline bool operator > (const property_value& a, const property_value& b) { return b.m_strValue < a.m_strValue; }
inline bool operator == (const platform_type& a, const platform_type& b) { return a.m_strName == b.m_strName; }
inline bool operator == (const account_type& a, const account_type& b) { return a.m_strName == b.m_strName; }
inline bool operator == (const property_type& a, const property_type& b) { return a.m_strName == b.m_strName; }
inline bool operator == (const property_value& a, const property_value& b) { return a.m_strValue == b.m_strValue; }
inline bool operator != (const platform_type& a, const platform_type& b) { return !(a.m_strName == b.m_strName); }
inline bool operator != (const account_type& a, const account_type& b) { return !(a.m_strName == b.m_strName); }
inline bool operator != (const property_type& a, const property_type& b) { return !(a.m_strName == b.m_strName); }
inline bool operator != (const property_value& a, const property_value& b) { return !(a.m_strValue == b.m_strValue); }

using property_tree = tree_type<property_type, property_value>;
using account_tree = tree_type<account_type, property_tree>;
using platform_tree = tree_type<platform_type, account_tree>;

inline property_tree::data_type* property_tree::Insert(const property_type& key)
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

    platform_tree m_Tree;

    Credential(const Credential&) = delete;
    Credential& operator=(const Credential&) = delete;

public:

    Credential() = default;
    explicit Credential(const string_type& strWord) : m_strWord(strWord) { }

    void Clear();
    
    bool IsValid() const { return !(m_strUser.empty() || m_strWord.empty()); }

    platform_tree& Tree() { return m_Tree; }
    const platform_tree& Tree() const { return m_Tree; }
    
    const string_type& GetWord() const { return m_strWord; }
    const string_type& GetUser() const { return m_strUser; }
    unsigned long long GetTime() const { return m_ullTime; }
    void SetWord(const string_type& strWord) { m_strWord = strWord; }
    void SetUser(const string_type& strUser) { m_strUser = strUser; }
    void UpdateTime();
	bool ValidateWord(const string_type& strWord) const;

    bool FromXml(const memory_type& mt);
    bool ToXml(memory_type& mt) const;

    bool Load(const char* file);
    bool Save(const char* file) const;

    static bool Encoding(memory_type& mt, const byte_t* key, size_t n);
    static bool Decoding(memory_type& mt, const byte_t* key, size_t n);
    static bool CheckFile(const char* file, memory_type* dst);

};

_bnb_space_end_

#endif