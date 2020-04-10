
enum class credential_type : unsigned char {
    ct_credential, ct_platform, ct_account, ct_pair
};  // enum credential_type

enum class event_type : unsigned char {
    et_add, et_delete, et_update, et_move, et_sort, et_clear
};  // enum event_type

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

template<typename NodeType>
class list_t
{
private:

    list_t(const list_t&) = delete;
    list_t& operator=(const list_t&) = delete;

    using node_t = typename NodeType::node_type;
    using item_t = typename NodeType::item_type;
    using parent_t = typename NodeType::parent_type;

    node_t* _first{ nullptr };
    node_t* _last{ nullptr };
    std::size_t _count{ 0 };

    void _Take(node_t* ptr)
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

    void _Remove(node_t* ptr)
    {
        _Take(ptr);
        --_count;
        delete ptr;

        Event(event_type::et_delete, item_t::type);
    }

    void _InsertAfter(node_t* where, node_t* ptr)
    {
        ptr->_prev = where;
        ptr->_next = where->_next;

        if (_last == where)
            _last = ptr;
        else
            where->_next->_prev = ptr;

        where->_next = ptr;
    }

    void _InsertBefore(node_t* where, node_t* ptr)
    {
        ptr->_prev = where->_prev;
        ptr->_next = where;

        if (_first == where)
            _first = ptr;
        else
            where->_prev->_next = ptr;

        where->_prev = ptr;
    }

    int _Move(node_t* ptr, int offset)
    {
        int i = 0;

        if (0 < offset)
        {
            node_t* where = ptr;
            for (; i < offset && where != _last; ++i)
                where = where->_next;

            if (ptr != where)
            {
                _Take(ptr);
                _InsertAfter(where, ptr);
            }
        }

        if (offset < 0)
        {
            node_t* where = ptr;
            for (; offset < i && where != _first; --i)
                where = where->_prev;

            if (ptr != where)
            {
                _Take(ptr);
                _InsertBefore(where, ptr);
            }
        }

        if (0 != i) Event(event_type::et_move, item_t::type);

        return i;
    }

    bool _Update(node_t* where, const item_t& item)
    {
        for (node_t* ptr = where->_prev; ptr; ptr = ptr->_prev)
            if (item == ptr->_data)
                return false;

        for (node_t* ptr = where->_next; ptr; ptr = ptr->_next)
            if (item == ptr->_data)
                return false;

        where->SetItem(item);
        Event(event_type::et_update, item_t::type);

        return true;
    }

public:

    list_t() = default;
    virtual ~list_t() { Clear(); }

    virtual void Event(event_type et, credential_type ct) { }

    std::size_t Size() const { return _count; }
    bool IsEmpty() const { return (0 == Size()); }

    void Clear()
    {
        while (_first)
        {
            node_t* ptr = _first;
            _first = _first->_next;
            delete ptr;
        }

        _last = nullptr;
        _count = 0;
    }

    node_t* Add(const item_t& item)
    {
        if (Find(item)) return nullptr;

        node_t* ptr = new node_t(dynamic_cast<parent_t*>(this), item);

        if (IsEmpty())
            _first = _last = ptr;
        else
            _InsertAfter(_last, ptr);

        ++_count;

        return ptr;
    }

    node_t* PushBack(const item_t& item)
    {
        if (node_t* ptr = Add(item))
        {
            Event(event_type::et_add, item_t::type);

            return ptr;
        }

        return nullptr;
    }

    node_t* PushFront(const item_t& item)
    {
        if (Find(item)) return nullptr;

        node_t* ptr = new node_t(dynamic_cast<parent_t*>(this), item);

        if (IsEmpty())
            _first = _last = ptr;
        else
            _InsertBefore(_first, ptr);

        ++_count;

        Event(event_type::et_add, item_t::type);
        return ptr;
    }

    template<typename FuncType>
    bool Remove(FuncType func)
    {
        if (node_t* ptr = Find(func))
        {
            _Remove(ptr);
            return true;
        }

        return false;
    }

    bool Remove(const item_t& key)
    {
        return Remove([&key](const item_t& item) { return (&key == &item || key == item); });
    }

    bool Remove(std::size_t index)
    {
        if (node_t* ptr = operator[](index))
        {
            _Remove(ptr);
            return true;
        }

        return false;
    }

    template<typename FuncType>
    bool Update(FuncType func, const item_t& item)
    {
        if (node_t* ptr = Find(func))
            return _Update(ptr, item);

        return false;
    }

    bool Update(const item_t& key, const item_t& item)
    {
        return Update([&key](const item_t& item) { return (&key == &item || key == item); }, item);
    }

    bool Update(std::size_t index, const item_t& item)
    {
        if (node_t* ptr = operator[](index))
            return _Update(ptr, item);

        return false;
    }

    int Move(const item_t& key, int offset)
    {
        if (node_t* ptr = Find(key))
            return _Move(ptr, offset);

        return 0;
    }

    int Move(std::size_t index, int offset)
    {
        if (node_t* ptr = operator[](index))
            return _Move(ptr, offset);

        return 0;
    }

    template<typename FuncType>
    bool Sort(FuncType compareFunc)
    {
        if (1 < _count)
        {
            for (node_t* ptr = _first->_next; ptr;)
            {
                node_t* next = ptr->_next;
                if (compareFunc(ptr->_data, ptr->_prev->_data))
                {
                    _Take(ptr);
                    _InsertBefore(Find([&ptr](const item_t& item) { return !compareFunc(item < ptr->_data); }), ptr);
                }

                ptr = next;
            }

            Event(event_type::et_sort, item_t::type);
            return true;
        }

        return false;
    }

    template<typename FuncType>
    node_t* Find(FuncType func)
    {
        for (node_t* ptr = _first; ptr; ptr = ptr->_next)
            if (func(ptr->_data))
                return ptr;

        return nullptr;
    }

    template<typename FuncType>
    const node_t* Find(FuncType func) const
    {
        for (node_t* ptr = _first; ptr; ptr = ptr->_next)
            if (func(ptr->_data))
                return ptr;

        return nullptr;
    }

    node_t* Find(const item_t& key)
    {
        return (Find([&key](const item_t& item) { return (&key == &item || key == item); }));
    }

    const node_t* Find(const item_t& key) const
    {
        return (Find([&key](const item_t& item) { return (&key == &item || key == item); }));
    }

    node_t* operator[](std::size_t index)
    {
        std::size_t n = 0;
        for (node_t* ptr = _first; ptr; ptr = ptr->_next)
            if (n++ == index)
                return ptr;

        return nullptr;
    }

    const node_t* operator[](std::size_t index) const
    {
        std::size_t n = 0;
        for (node_t* ptr = _first; ptr; ptr = ptr->_next)
            if (n++ == index)
                return ptr;

        return nullptr;
    }

    template<typename FuncType>
    void Traversal(FuncType func)
    {
        for (node_t* ptr = _first; ptr; ptr = ptr->_next)
            func(*ptr);
    }

    template<typename FuncType>
    void Traversal(FuncType func) const
    {
        for (node_t* ptr = _first; ptr; ptr = ptr->_next)
            func(*ptr);
    }

};  // class list_t

template<typename ItemType, typename NodeType, typename ParentType>
class node_t
{
private:

    node_t(const node_t&) = delete;
    node_t& operator=(const node_t&) = delete;

public:

    using base_type = node_t;
    using node_type = NodeType;
    using item_type = ItemType;
    using parent_type = ParentType;

    node_t() = default;
    node_t(parent_type* ptr, const item_type& item) : _parent(ptr), _data(item) { }

    parent_type* Parent() const { return _parent; }
    const item_type& Item() const { return _data; }
    void SetItem(item_type item) { _data = item; _data.Updated(); }

    void Event(event_type at, credential_type ct)
    {
        _data.Event(at, ct);
        if (_parent)
            _parent->Event(at, ct);
    }

protected:

    item_type _data;

    node_type* _prev{ nullptr };
    node_type* _next{ nullptr };
    parent_type* _parent{ nullptr };

    friend list_t<node_type>;
};  // class node_t
