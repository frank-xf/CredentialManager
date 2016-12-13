#ifndef _bnb_Tree_H_
#define _bnb_Tree_H_

namespace bnb
{

    class index_type
    {
    public:

        index_type() : _id(_NextID()) { }
        index_type(const index_type&) : _id(_NextID()) { }
        index_type& operator = (const index_type&) { return *this; }
        virtual ~index_type() { }

        unsigned int GetID() const { return (_id); }

    private:

        static unsigned int _NextID() { static unsigned int _x_(0); return (++_x_); }

        const unsigned int _id;

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

        const parent_type* GetParent() const { return (_parent); }
        void SetParent(parent_type* ptr) { (_parent = ptr); }

        child_base(parent_type* ptr = nullptr) : _parent(ptr) { }
        virtual ~child_base() { }

    };

    template<typename _NodeT>
    class list_type
    {
        static_assert(std::is_base_of<index_type, _NodeT>::value, R"(the template parameter "_NodeT" must be inherit from index_type)");

    protected:

        using size_type = unsigned int;
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

    public:

        list_type() = default;
        virtual ~list_type() { Clear(); }

        bool IsEmpty() const { return (nullptr == _first || nullptr == _last || 0 == _nCount); }

        size_type Size() const { return _nCount; }

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

        node_type* Find(unsigned int id)
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                if (id == ptr->GetID())
                    return ptr;

            return nullptr;
        }

        const node_type* Find(unsigned int id) const
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                if (id == ptr->GetID())
                    return ptr;

            return nullptr;
        }

        node_type* PushBack(const node_item& item)
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                if (ptr->GetData() == item)
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

        node_type* PushFront(const node_item& item)
        {
            for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
                if (ptr->GetData() == item)
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

            return ptr;
        }

        bool Update(const node_item& target, const node_item& item)
        {
            node_type* target_ptr = nullptr;

            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
            {
                if (nullptr == target_ptr)
                {
                    if (&target == &ptr->GetData() || target == ptr->GetData())
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
                return true;
            }

            return false;
        }

        bool Remove(const node_item& item)
        {
            for (node_type* ptr = _first; ptr; ptr = ptr->_next)
            {
                if (&item == &ptr->GetData() || item == ptr->GetData())
                {
                    _Take(ptr);
                    --_nCount;
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

                        if (![this, &_compareFunc, &target_ptr]() {
                            for (node_type* insert_ptr = _first; insert_ptr; insert_ptr = insert_ptr->_next)
                            {
                                if (_compareFunc(target_ptr->GetData(), insert_ptr->GetData()))
                                {
                                    _InsertBefore(target_ptr, insert_ptr);
                                    return true;
                                }
                            }

                            return false;
                        }())
                        {
                            target_ptr->_next = nullptr;
                            target_ptr->_prev = _last;
                            _last->_next = target_ptr;
                            _last = target_ptr;
                        }

                        target_ptr = next_ptr;
                    } while (target_ptr);
                }
            }
        }

        void Sort()
        {
            Sort([](const node_item& left, const node_item& right) { return left < right; });
        }

        bool Move(const node_item& key, int offset)
        {
            if (0 < offset)
            {
                for (node_type* target_ptr = _first; target_ptr; target_ptr = target_ptr->_next)
                {
                    if (&key == &target_ptr->GetData() || key == target_ptr->GetData())
                    {
                        node_type* insert_ptr = target_ptr;
                        for (int i = 0; i < offset && insert_ptr != _last; ++i)
                            insert_ptr = insert_ptr->_next;

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
                for (node_type* target_ptr = _last; target_ptr; target_ptr = target_ptr->_prev)
                {
                    if (&key == &target_ptr->GetData() || key == target_ptr->GetData())
                    {
                        node_type* insert_ptr = target_ptr;
                        for (int i = 0; offset < i && insert_ptr != _first; --i)
                            insert_ptr = insert_ptr->_prev;

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
                for (node_type* ptr = _first; ptr; ptr = ptr->_next)
                    if (&key == &ptr->GetData() || key == ptr->GetData())
                        return true;
            }

            return false;
        }

    };

    template<typename _ItemT, typename _NodeT>
    class list_node : public index_type, public child_base<list_type<_NodeT>>, public item_base<_ItemT>
    {
    protected:

        using base_type = list_node;
        using node_type = _NodeT;

    private:

        using base_type1 = child_base<list_type<_NodeT>>;
        using base_type2 = item_base<_ItemT>;

        list_node(const list_node&) = delete;
        list_node& operator=(const list_node&) = delete;

    public:

        node_type* _prev{ nullptr };
        node_type* _next{ nullptr };

        list_node() = default;
        list_node(parent_type* ptr, const item_type& item) : base_type1(ptr), base_type2(item) { }
        virtual ~list_node() { }

    };

}

#endif // _bnb_Tree_H_
