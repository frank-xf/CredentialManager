#pragma once

namespace xf::credential
{
    using char_t = char;
    using time_t = std::uint64_t;
    using string_t = std::basic_string<char_t, std::char_traits<char_t>, std::allocator<char_t>>;

    enum class credential_type : unsigned char {
        ct_credential, ct_platform, ct_account, ct_pair, ct_max
    };  // enum credential_type

    enum class event_type : unsigned char {
        et_add, et_delete, et_update, et_move, et_sort, et_clear
    };  // enum event_type

    template<typename NodeType>
    class list_t
    {
    protected:

        list_t(const list_t&) = delete;
        list_t& operator=(const list_t&) = delete;

        using list_base = list_t;
        using child_node = typename NodeType::node_type;
        using child_item = typename NodeType::item_type;
        using child_parent = typename NodeType::parent_type;

        child_node* _first{ nullptr };
        child_node* _last{ nullptr };
        std::size_t _count{ 0 };

        void _Take(child_node* ptr)
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

        void _Remove(child_node* ptr)
        {
            _Take(ptr);
            --_count;
            delete ptr;

            Event(event_type::et_delete, child_item::type);
        }

        void _InsertAfter(child_node* where, child_node* ptr)
        {
            ptr->_prev = where;
            ptr->_next = where->_next;

            if (_last == where)
                _last = ptr;
            else
                where->_next->_prev = ptr;

            where->_next = ptr;
        }

        void _InsertBefore(child_node* where, child_node* ptr)
        {
            ptr->_prev = where->_prev;
            ptr->_next = where;

            if (_first == where)
                _first = ptr;
            else
                where->_prev->_next = ptr;

            where->_prev = ptr;
        }

        int _Move(child_node* ptr, int offset)
        {
            int i = 0;

            if (0 < offset)
            {
                child_node* where = ptr;
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
                child_node* where = ptr;
                for (; offset < i && where != _first; --i)
                    where = where->_prev;

                if (ptr != where)
                {
                    _Take(ptr);
                    _InsertBefore(where, ptr);
                }
            }

            if (0 != i) Event(event_type::et_move, child_item::type);

            return i;
        }

        bool _Update(child_node* where, const child_item& item)
        {
            for (child_node* ptr = where->_prev; ptr; ptr = ptr->_prev)
                if (item == ptr->_data)
                    return false;

            for (child_node* ptr = where->_next; ptr; ptr = ptr->_next)
                if (item == ptr->_data)
                    return false;

            where->SetItem(item);
            Event(event_type::et_update, child_item::type);

            return true;
        }

    public:

        list_t() = default;
        virtual ~list_t() { Clear(); }

        virtual void Event(event_type et, credential_type ct) { }

        std::size_t Size() const { return _count; }
        bool IsEmpty() const { return (0 == Size()); }

        virtual void Clear()
        {
            while (_first)
            {
                child_node* ptr = _first;
                _first = _first->_next;
                delete ptr;
            }

            _last = nullptr;
            _count = 0;

            Event(event_type::et_clear, child_item::type);
        }

        child_node* Add(const child_item& item)
        {
            if (Find(item)) return nullptr;

            child_node* ptr = new child_node(dynamic_cast<child_parent*>(this), item);

            if (IsEmpty())
                _first = _last = ptr;
            else
                _InsertAfter(_last, ptr);

            ++_count;

            return ptr;
        }

        child_node* PushBack(const child_item& item)
        {
            if (child_node* ptr = Add(item))
            {
                Event(event_type::et_add, child_item::type);

                return ptr;
            }

            return nullptr;
        }

        child_node* PushFront(const child_item& item)
        {
            if (Find(item)) return nullptr;

            child_node* ptr = new child_node(dynamic_cast<child_parent*>(this), item);

            if (IsEmpty())
                _first = _last = ptr;
            else
                _InsertBefore(_first, ptr);

            ++_count;

            Event(event_type::et_add, child_item::type);
            return ptr;
        }

        template<typename FuncType>
        bool RemoveIf(FuncType func)
        {
            if (child_node* ptr = Find(func))
            {
                _Remove(ptr);
                return true;
            }

            return false;
        }

        bool Remove(const child_item& key)
        {
            return RemoveIf([&key](const auto& item) { return (&key == &item || key == item); });
        }

        bool Remove(std::size_t index)
        {
            if (child_node* ptr = operator[](index))
            {
                _Remove(ptr);
                return true;
            }

            return false;
        }

        template<typename FuncType>
        bool UpdateIf(FuncType func, const child_item& item)
        {
            if (child_node* ptr = Find(func))
                return _Update(ptr, item);

            return false;
        }

        bool Update(const child_item& key, const child_item& item)
        {
            return UpdateIf([&key](const auto& item) { return (&key == &item || key == item); }, item);
        }

        bool Update(std::size_t index, const child_item& item)
        {
            if (child_node* ptr = operator[](index))
                return _Update(ptr, item);

            return false;
        }

        int Move(const child_item& key, int offset)
        {
            if (child_node* ptr = Find(key))
                return _Move(ptr, offset);

            return 0;
        }

        int Move(std::size_t index, int offset)
        {
            if (child_node* ptr = operator[](index))
                return _Move(ptr, offset);

            return 0;
        }

        template<typename FuncType>
        bool Sort(FuncType func)
        {
            if (1 < _count)
            {
                for (child_node* ptr = _first->_next; ptr;)
                {
                    child_node* next = ptr->_next;
                    if (func(ptr->_data, ptr->_prev->_data))
                    {
                        _Take(ptr);
                        _InsertBefore(Find([&ptr, &func](const auto& item) { return !func(item, ptr->_data); }), ptr);
                    }

                    ptr = next;
                }

                Event(event_type::et_sort, child_item::type);
                return true;
            }

            return false;
        }

        bool Sort()
        {
            return Sort([](const auto& a, const auto& b) { return a < b; });
        }

        template<typename FuncType>
        child_node* Find(FuncType func)
        {
            for (child_node* ptr = _first; ptr; ptr = ptr->_next)
                if (func(ptr->_data))
                    return ptr;

            return nullptr;
        }

        template<typename FuncType>
        const child_node* Find(FuncType func) const
        {
            for (child_node* ptr = _first; ptr; ptr = ptr->_next)
                if (func(ptr->_data))
                    return ptr;

            return nullptr;
        }

        child_node* Find(const child_item& key)
        {
            return (Find([&key](const auto& item) { return (&key == &item || key == item); }));
        }

        const child_node* Find(const child_item& key) const
        {
            return (Find([&key](const auto& item) { return (&key == &item || key == item); }));
        }

        child_node* operator[](std::size_t index)
        {
            std::size_t n = 0;
            for (child_node* ptr = _first; ptr; ptr = ptr->_next)
                if (n++ == index)
                    return ptr;

            return nullptr;
        }

        const child_node* operator[](std::size_t index) const
        {
            std::size_t n = 0;
            for (child_node* ptr = _first; ptr; ptr = ptr->_next)
                if (n++ == index)
                    return ptr;

            return nullptr;
        }

        template<typename FuncType>
        void Traversal(FuncType func)
        {
            for (child_node* ptr = _first; ptr; ptr = ptr->_next)
                func(*ptr);
        }

        template<typename FuncType>
        void Traversal(FuncType func) const
        {
            for (child_node* ptr = _first; ptr; ptr = ptr->_next)
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

        using node_base = node_t;
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

}   // namespace xf::credential
