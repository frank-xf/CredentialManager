#pragma once

#include <string>

namespace xf::credential
{

    enum class credential_type : unsigned char {
        ct_credential, ct_platform, ct_account, ct_pair
    };  // enum credential_type

    enum class action_type : unsigned char {
        at_add, at_delete, at_update, at_move, at_sort, at_clear
    };  // enum action_type

    struct credential_t
    {
        const credential_type type;
        std::uint64_t time;

        credential_t(credential_type t) : type(t) { }

        virtual void Updated(action_type at, credential_type ct);

    };  // class credential_t

    template<typename NodeType>
    class list_t
    {
    protected:

        using node_t = typename NodeType::node_type;
        using item_t = typename NodeType::item_type;

    private:

        node_t* _first{ nullptr };
        node_t* _last{ nullptr };
        std::size_t _count{ 0 };

        template<typename FuncType>
        node_t* _Find(FuncType func)
        {
            for (node_t* ptr = _first; ptr; ptr = ptr->_next)
                if (func(ptr->_data))
                    return ptr;

            return nullptr;
        }

        template<typename FuncType>
        const node_t* _Find(FuncType func) const
        {
            for (node_t* ptr = _first; ptr; ptr = ptr->_next)
                if (func(ptr->_data))
                    return ptr;

            return nullptr;
        }

        void _Remove(node_t* ptr)
        {
            if (_first == ptr)
                _first = ptr->_next;
            else
                ptr->_prev->_next = ptr->_next;

            if (_last == ptr)
                _last = ptr->_prev;
            else
                ptr->_next->_prev = ptr->_prev;

            --_count;
            delete ptr;
        }

    public:

        list_t() = default;
        virtual ~list_t() { Clear(); }

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

            node_t* ptr = new node_t(this, item);

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

            ++_count;
            
            return ptr;
        }

        bool Remove(node_t* where)
        {
            for (node_t* ptr = _first; ptr; ptr = ptr->_next)
            {
                if (ptr == where)
                {
                    _Remove(ptr);
                    return true;
                }
            }

            return false;
        }

        template<typename FuncType>
        bool Remove(FuncType func)
        {
            if (node_t* ptr = _Find(func))
            {
                _Remove(ptr);
                return true;
            }

            return false;
        }

        bool Remove(const item_t& item)
        {
            if (node_t* ptr = Find(item))
            {
                _Remove(ptr);
                return true;
            }

            return false;
        }

        node_t* Find(const item_t& key)
        {
            return (_Find([&key](const item_t& item) { return (&key == &item || key == item); }));
        }

        const node_t* Find(const item_t& key) const
        {
            return (_Find([&key](const item_t& item) { return (&key == &item || key == item); }));
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

    };  // class list_t

    template<typename ItemType, typename NodeType>
    class node_t
    {
        static_assert(std::is_base_of<credential_t, ItemType>::value,
                      R"(the template parameter "ItemType" must be inherit from credential_t)");

    private:

        node_t(const node_t&) = delete;
        node_t& operator=(const node_t&) = delete;

    public:

        using base_type = node_t;
        using node_type = NodeType;
        using item_type = ItemType;
        using parent_type = list_t<node_type>;

        node_t() = default;
        node_t(parent_type* ptr, const item_type& item) : _parent(ptr), _data(item) { }

        parent_type* Parent() const { return _parent; }
        const item_type& GetData() const { return _data; }
        void SetData(const item_type& item) { _data = item; }

        void Updated(action_type at, credential_type ct)
        {
            _data.Updated(at, ct);
            if (_parent)
                _parent->Updated(at, ct);
        }

    protected:

        item_type _data;

        node_type* _prev{ nullptr };
        node_type* _next{ nullptr };
        parent_type* _parent{ nullptr };

        friend parent_type;
    };  // class node_t

}   // namespace xf::credential
