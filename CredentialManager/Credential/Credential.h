#ifndef _bnb_Credential_H_
#define _bnb_Credential_H_

#include <functional>

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
    using param_type = unsigned long;

    using string_type = std::basic_string<char_type, std::char_traits<char_type>, std::allocator<char_type>>;
    using memory_type = std::basic_string<byte_type, std::char_traits<byte_type>, std::allocator<byte_type>>;

    inline bool operator < (const string_type& a, const string_type& b) { return (_wcsicmp(a.c_str(), b.c_str()) < 0); }
    inline bool operator > (const string_type& a, const string_type& b) { return (b < a); }
    inline bool operator == (const string_type& a, const string_type& b) { return (0 == _wcsicmp(a.c_str(), b.c_str())); }
    inline bool operator != (const string_type& a, const string_type& b) { return !(a == b); }

    enum class action_type : unsigned char
    {
        at_none, at_insert, at_delete, at_update, at_move, at_sort, at_clear
    };

    class index_type
    {
    public:

        using id_type = unsigned int;

        index_type() : _id(_NextID()) { }
        index_type(const index_type&) : index_type() { }
        index_type& operator = (const index_type&) { return *this; }
        virtual ~index_type() { }

        id_type GetID() const { return (_id); }

    private:

        static id_type _NextID() { static id_type _x_(0); return (++_x_); }

        const id_type _id;

    };

    template<typename _ItemT>
    class item_base
    {
    public:

        using item_type = _ItemT;

        item_base() = default;
        item_base(const item_type& item) : _data(item) { }
        item_base& operator=(const item_type& item) { if (&_data != &item) (_data = item); return *this; }
        virtual ~item_base() { }

        const item_type& GetData() const { return (_data); }
        void SetData(const item_type& item) { (_data = item); }

    protected:

        item_type _data;

    };

    template<typename _ParentT>
    class child_base
    {
    protected:

        using parent_type = _ParentT;

        parent_type* _parent;

    public:

        parent_type* GetParent() const { return (_parent); }
        void SetParent(parent_type* ptr) { (_parent = ptr); }

        child_base(parent_type* ptr = nullptr) : _parent(ptr) { }
        virtual ~child_base() { }

    };

    template<typename _NodeT>
    class list_type
    {
        static_assert(std::is_base_of<index_type, _NodeT>::value, R"(the template parameter "_NodeT" must be inherit from index_type)");

    protected:

        using list_base = list_type;
        using node_type = typename _NodeT;
        using node_item = typename _NodeT::item_type;

    private:

        node_type* _first{ nullptr };
        node_type* _last{ nullptr };
        size_type _nCount{ 0 };

        list_type(const list_type&) = delete;
        list_type& operator=(const list_type&) = delete;

        void _Take(node_type* ptr)
        {
            if (_first == ptr)
                _first = ptr->_next;
            else
                ptr->_prev->_next = ptr->_next;

            if (_last == ptr)
                _last = ptr->_prev;
            else
                ptr->_next->_prev = ptr->_prev;
        }

        void _InsertAfter(node_type* target_ptr, node_type* insert_ptr)
        {
            target_ptr->_prev = insert_ptr;
            target_ptr->_next = insert_ptr->_next;

            if (_last == insert_ptr)
                _last = target_ptr;
            else
                insert_ptr->_next->_prev = target_ptr;

            insert_ptr->_next = target_ptr;
        }

        void _InsertBefore(node_type* target_ptr, node_type* insert_ptr)
        {
            target_ptr->_prev = insert_ptr->_prev;
            target_ptr->_next = insert_ptr;

            if (_first == insert_ptr)
                _first = target_ptr;
            else
                insert_ptr->_prev->_next = target_ptr;

            insert_ptr->_prev = target_ptr;
        }

        template<typename _findT>
        node_type* _Find(_findT _findFunc)
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                if (_findFunc(*ptr))
                    return ptr;

            return nullptr;
        }

        template<typename _findT>
        const node_type* _Find(_findT _findFunc) const
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                if (_findFunc(*ptr))
                    return ptr;

            return nullptr;
        }

        template<typename _findT>
        bool _Update(_findT _findFunc, const node_item& item)
        {
            node_type* target_ptr = nullptr;

            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
            {
                if (nullptr == target_ptr)
                {
                    if (_findFunc(*ptr))
                    {
                        target_ptr = ptr;
                        continue;
                    }
                }

                if (item == ptr->GetData())
                    return nullptr;
            }

            if (target_ptr)
            {
                target_ptr->SetData(item);
                Updated(static_cast<param_type>(action_type::at_update));

                return true;
            }

            return false;
        }

        template<typename _findT>
        bool _Remove(_findT _findFunc)
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
            {
                if (_findFunc(*ptr))
                {
                    _Take(ptr);
                    --_nCount;
                    delete ptr;

                    Updated(static_cast<param_type>(action_type::at_delete));

                    return true;
                }
            }

            return false;
        }

        template<typename _findT>
        bool _Move(_findT _findFunc, int offset)
        {
            if (0 < offset)
            {
                for (node_type* target_ptr = _first; target_ptr; target_ptr = target_ptr->_next)
                {
                    if (_findFunc(*target_ptr))
                    {
                        node_type* insert_ptr = target_ptr;
                        for (int i = 0; i < offset && insert_ptr != _last; ++i)
                            insert_ptr = insert_ptr->_next;

                        if (target_ptr != insert_ptr)
                        {
                            _Take(target_ptr);
                            _InsertAfter(target_ptr, insert_ptr);
                        }

                        Updated(static_cast<param_type>(action_type::at_move));

                        return true;
                    }
                }
            }
            else if (offset < 0)
            {
                for (node_type* target_ptr = _last; target_ptr; target_ptr = target_ptr->_prev)
                {
                    if (_findFunc(*target_ptr))
                    {
                        node_type* insert_ptr = target_ptr;
                        for (int i = 0; offset < i && insert_ptr != _first; --i)
                            insert_ptr = insert_ptr->_prev;

                        if (target_ptr != insert_ptr)
                        {
                            _Take(target_ptr);
                            _InsertBefore(target_ptr, insert_ptr);
                        }

                        Updated(static_cast<param_type>(action_type::at_move));

                        return true;
                    }
                }
            }
            else
            {
                for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                    if (_findFunc(*ptr))
                        return true;
            }

            return false;
        }

    public:

        list_type() = default;
        virtual ~list_type() { Clear(); }

        bool IsEmpty() const { return (nullptr == _first || nullptr == _last || 0 == _nCount); }

        size_type Size() const { return _nCount; }

        virtual void Updated(param_type aType) { }

        void Clear()
        {
            while (_first)
            {
                node_type* ptr = _first;
                _first = _first->_next;
                delete ptr;
            }

            _last = nullptr;
            _nCount = 0;

            Updated(static_cast<param_type>(action_type::at_clear));
        }

        template<typename _actionT>
        void PreorderTraversal(_actionT _actionFunc)
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                _actionFunc(*ptr);
        }

        template<typename _actionT>
        void PreorderTraversal(_actionT _actionFunc) const
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                _actionFunc(*ptr);
        }

        template<typename _actionT>
        void PostorderTraversal(_actionT _actionFunc)
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                _actionFunc(*ptr);
        }

        template<typename _actionT>
        void PostorderTraversal(_actionT _actionFunc) const
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                _actionFunc(*ptr);
        }

        template<typename _actionT>
        bool PreorderAction(_actionT _actionFunc)
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                if (_actionFunc(*ptr))
                    return true;

            return false;
        }

        template<typename _actionT>
        bool PreorderAction(_actionT _actionFunc) const
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                if (_actionFunc(*ptr))
                    return true;

            return false;
        }

        template<typename _actionT>
        bool PostorderAction(_actionT _actionFunc)
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                if (_actionFunc(*ptr))
                    return true;

            return false;
        }

        template<typename _actionT>
        bool PostorderAction(_actionT _actionFunc) const
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                if (_actionFunc(*ptr))
                    return true;

            return false;
        }

        node_type* Find(typename node_type::id_type id)
        {
            return (_Find([&id](const node_type& node) { return (id == node.GetID()); }));
        }

        const node_type* Find(typename node_type::id_type id) const
        {
            return (_Find([&id](const node_type& node) { return (id == node.GetID()); }));
        }

        node_type* Find(const node_item& key)
        {
            return (_Find([&key](const node_type& node) { return (&key == &node.GetData() || key == node.GetData()); }));
        }

        const node_type* Find(const node_item& key) const
        {
            return (_Find([&key](const node_type& node) { return (&key == &node.GetData() || key == node.GetData()); }));
        }

        node_type* Add(const node_item& item)
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                if (item == ptr->GetData())
                    return nullptr;

            node_type* ptr = new node_type(this, item);

            if (IsEmpty())
            {
                _first = ptr;
                _last = ptr;
            }
            else
            {
                ptr->_prev = _last;
                _last->_next = ptr;
                _last = ptr;
            }

            ++_nCount;

            return ptr;
        }

        node_type* PushBack(const node_item& item)
        {
            if (auto ptr = Add(item))
            {
                Updated(static_cast<param_type>(action_type::at_insert));

                return ptr;
            }

            return nullptr;
        }

        node_type* PushFront(const node_item& item)
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                if (item == ptr->GetData())
                    return nullptr;

            node_type* ptr = new node_type(this, item);

            if (IsEmpty())
            {
                _first = ptr;
                _last = ptr;
            }
            else
            {
                ptr->_next = _first;
                _first->_prev = ptr;
                _first = ptr;
            }

            ++_nCount;

            Updated(static_cast<param_type>(action_type::at_insert));

            return ptr;
        }

        bool Update(typename node_type::id_type id, const node_item& item)
        {
            return (_Update([&id](const node_type& node) { return (id == node.GetID()); }, item));
        }

        bool Update(const node_item& key, const node_item& item)
        {
            return (_Update([&key](const node_type& node) { return (&key == &node.GetData() || key == node.GetData()); }, item));
        }

        bool Remove(typename node_type::id_type id)
        {
            return (_Remove([&id](const node_type& node) { return (id == node.GetID()); }));
        }

        bool Remove(const node_item& key)
        {
            return (_Remove([&key](const node_type& node) { return (&key == &node.GetData() || key == node.GetData()); }));
        }

        template<typename _findT>
        size_type RemoveIf(_findT _findFunc)
        {
            size_type nResult = 0;

            for (node_type* ptr = _first; ptr; )
            {
                node_type* next_ptr = ptr->_next;

                if (_findFunc(ptr->GetData()))
                {
                    _Take(ptr);
                    --_nCount;
                    delete ptr;

                    ++nResult;
                }

                ptr = next_ptr;
            }

            if (0 < nResult)
                Updated(static_cast<param_type>(action_type::at_delete));

            return nResult;
        }

        template<typename _CompareT>
        void Sort(_CompareT _compareFunc)
        {
            if (_first)
            {
                if (node_type* target_ptr = _first->_next)
                {
                    _last = _first;
                    _first->_prev = nullptr;
                    _first->_next = nullptr;

                    do {
                        node_type* next_ptr = target_ptr->_next;

                        if ([this, &_compareFunc, &target_ptr]() {
                            for (node_type* insert_ptr = _first; insert_ptr; insert_ptr = insert_ptr->_next)
                            {
                                if (_compareFunc(target_ptr->GetData(), insert_ptr->GetData()))
                                {
                                    _InsertBefore(target_ptr, insert_ptr);
                                    return false;
                                }
                            }

                            return true;
                        }())
                        {
                            target_ptr->_next = nullptr;
                            target_ptr->_prev = _last;
                            _last->_next = target_ptr;
                            _last = target_ptr;
                        }

                        target_ptr = next_ptr;
                    } while (target_ptr);

                    Updated(static_cast<param_type>(action_type::at_sort));
                }
            }
        }

        void Sort()
        {
            Sort([](const node_item& left, const node_item& right) { return left < right; });
        }

        bool Move(const node_item& key, int offset)
        {
            return (_Move([&key](const node_type& node) { return (&key == &node.GetData() || key == node.GetData()); }, offset));
        }

        bool Move(typename node_type::id_type id, int offset)
        {
            return (_Move([&id](const node_type& node) { return (id == node.GetID()); }, offset));
        }

    };

    template<typename _ItemT, typename _NodeT>
    class list_node : public index_type, public child_base<list_type<_NodeT>>, public item_base<_ItemT>
    {
    private:

        using base_type1 = child_base<list_type<_NodeT>>;
        using base_type2 = item_base<_ItemT>;

        list_node(const list_node&) = delete;
        list_node& operator=(const list_node&) = delete;

    public:

        list_node() = default;
        list_node(parent_type* ptr, const item_type& item) : base_type1(ptr), base_type2(item) { }
        virtual ~list_node() { }

    protected:

        using base_type = list_node;
        using node_type = _NodeT;

        node_type* _prev{ nullptr };
        node_type* _next{ nullptr };

        friend parent_type;
    };

    enum class credential_enum : unsigned char { credential, platform, account, pair };
   
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
            : credential_base(credential_enum::credential)
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
            : credential_base(credential_enum::platform)
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
            : credential_base(credential_enum::account)
            , m_strName(name)
            , m_strComment(comment)
        { }

        const string_type& GetName() const { return m_strName; }
        const string_type& GetComment() const { return m_strComment; }

    private:

        string_type m_strName;
        string_type m_strComment;
    };

    class pair_type : public credential_base
    {
    public:

        pair_type(const string_type& key = string_type(), const string_type& value = string_type())
            : credential_base(credential_enum::pair)
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
    inline bool operator < (const pair_type& a, const pair_type& b) { return a.GetKey() < b.GetKey(); }
    inline bool operator > (const platform_type& a, const platform_type& b) { return (b < a); }
    inline bool operator > (const account_type& a, const account_type& b) { return (b < a); }
    inline bool operator > (const pair_type& a, const pair_type& b) { return (b < a); }
    inline bool operator == (const platform_type& a, const platform_type& b) { return a.GetName() == b.GetName(); }
    inline bool operator == (const account_type& a, const account_type& b) { return a.GetName() == b.GetName(); }
    inline bool operator == (const pair_type& a, const pair_type& b) { return a.GetKey() == b.GetKey(); }
    inline bool operator != (const platform_type& a, const platform_type& b) { return !(a == b); }
    inline bool operator != (const account_type& a, const account_type& b) { return !(a == b); }
    inline bool operator != (const pair_type& a, const pair_type& b) { return !(a == b); }

    class pair_node : public list_node<pair_type, pair_node>
    {
    public:

        using base_type::base_type;
    };

    class account_node : public list_node<account_type, account_node>, public list_type<pair_node>
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

        using updated_handle_type = std::function<void(const Credential&, param_type, param_type)>;
        updated_handle_type _UpdatedHandle;

    public:

        Credential() = default;

        void SetWord(const string_type& strWord) { _data.m_strWord = strWord; }
        void SetUser(const string_type& strUser) { _data.m_strUser = strUser; }
        void SetComment(const string_type& strComment) { _data.m_strComment = strComment; }

        void RegisterHandle(const updated_handle_type& pFunc);

        void Clear();
        void UpdateWord(const string_type& strWord);
        void UpdateInfo(const string_type& strUser, const string_type& strComment);

        void Updated(param_type aType) override;
        void Updated(param_type aType, param_type cType);

        bool FromXml(const memory_type& mt);
        bool ToXml(memory_type& mt) const;

        bool Load(const char* file, const string_type& password);
        bool Load(const char* file, const byte_type* password, size_type n);
        bool Save(const char* file) const;

        using list_base::Find;

        account_node* Find(id_type id1, id_type id2);
        pair_node* Find(id_type id1, id_type id2, id_type id3);
        const account_node* Find(id_type id1, id_type id2) const;
        const pair_node* Find(id_type id1, id_type id2, id_type id3) const;

        static bool ValidateName(const string_type& strName);
        static bool Encoding(memory_type& mt, const byte_type* key, size_t n);
        static bool Decoding(memory_type& mt, const byte_type* key, size_t n);
        static bool CheckFile(const char* file, memory_type* dst);

    };  // class Credential

}   // namespace bnb

#endif
