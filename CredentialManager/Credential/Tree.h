#ifndef _bnb_Tree_H_
#define _bnb_Tree_H_

class index_type
{
protected:

    index_type() : _id(_NextID()) { }
    index_type(const index_type&) : _id(_NextID()) { }
    index_type& operator = (const index_type&) { return *this; }

public:

    virtual ~index_type() { }

    unsigned int GetID() const { return (_id); }

private:

    static unsigned int _NextID() { static unsigned int _x_(0); return (++_x_); }

    const unsigned int _id;

};

template<typename _ChildT>
class list_type;

template<typename _ItemT>
class list_node : index_type
{
public:

    using parent_type = list_type<list_node>;
    using item_type = _ItemT;

    list_node() = default;
    list_node(parent_type* parent, const item_type& item) : _parent(parent), _data(item) { }

    const item_type& GetData() const { return (_data); }
    void SetData(const item_type& item) { (_data = item); }

    parent_type* _parent{ nullptr };
    list_node* _prev{ nullptr };
    list_node* _next{ nullptr };

private:

    item_type _data;

    list_node(const list_node&) = delete;
    list_node& operator=(const list_node&) = delete;

};

template<typename _ChildT>
class list_type
{
    static_assert(std::is_base_of<index_type, _ChildT>::value, R"(the template parameter "_ChildT" must be inherit from index_type)");

    using size_type = unsigned int;
    using child_type = typename _ChildT::parent_type;
    using data_type = child_type::item_type;

private:

    child_type* _first{ nullptr };
    child_type* _last{ nullptr };
    size_type _nCount{ 0 };

    list_type(const list_type&) = delete;
    list_type& operator=(const list_type&) = delete;

    void _Take(child_type* ptr)
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

    void _InsertAfter(child_type* target_ptr, child_type* insert_ptr)
    {
        target_ptr->_prev = insert_ptr;
        target_ptr->_next = insert_ptr->_next;

        if (_last == insert_ptr)
            _last = target_ptr;
        else
            insert_ptr->_next->_prev = target_ptr;

        insert_ptr->_next = target_ptr;
    }

    void _InsertBefore(child_type* target_ptr, child_type* insert_ptr)
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
            child_type* ptr = _first;
            _first = _first->_next;
            delete ptr;
        }

        _last = nullptr;
        _nCount = 0;
    }

    template<typename _actionT>
    void PreorderTraversal(_actionT _actionFunc)
    {
        for (child_type* ptr = _first; ptr; ptr = ptr->_next)
            _actionFunc(ptr->GetData());
    }

    template<typename _actionT>
    void PreorderTraversal(_actionT _actionFunc) const
    {
        for (child_type* ptr = _first; ptr; ptr = ptr->_next)
            _actionFunc(ptr->GetData());
    }

    template<typename _actionT>
    void PostorderTraversal(_actionT _actionFunc)
    {
        for (child_type* ptr = _last; ptr; ptr = ptr->_prev)
            _actionFunc(ptr->GetData());
    }

    template<typename _actionT>
    void PostorderTraversal(_actionT _actionFunc) const
    {
        for (child_type* ptr = _last; ptr; ptr = ptr->_prev)
            _actionFunc(ptr->GetData());
    }

    template<typename _actionT>
    bool PreorderAction(_actionT _actionFunc)
    {
        for (child_type* ptr = _first; ptr; ptr = ptr->_next)
            if (_actionFunc(ptr->GetData()))
                return true;

        return false;
    }

    template<typename _actionT>
    bool PreorderAction(_actionT _actionFunc) const
    {
        for (child_type* ptr = _first; ptr; ptr = ptr->_next)
            if (_actionFunc(ptr->GetData()))
                return true;

        return false;
    }

    template<typename _actionT>
    bool PostorderAction(_actionT _actionFunc)
    {
        for (child_type* ptr = _last; ptr; ptr = ptr->_prev)
            if (_actionFunc(ptr->GetData()))
                return true;

        return false;
    }

    template<typename _actionT>
    bool PostorderAction(_actionT _actionFunc) const
    {
        for (child_type* ptr = _last; ptr; ptr = ptr->_prev)
            if (_actionFunc(ptr->GetData()))
                return true;

        return false;
    }

    child_type* Find(unsigned int id)
    {
        for (child_type* ptr = _last; ptr; ptr = ptr->_prev)
            if (_actionFunc(ptr->GetID()))
                return ptr->GetData();

        return nullptr;
    }

    const child_type* Find(unsigned int id) const
    {
        for (child_type* ptr = _last; ptr; ptr = ptr->_prev)
            if (_actionFunc(ptr->GetID()))
                return ptr->GetData();

        return nullptr;
    }

    child_type* PushBack(const data_type& item)
    {
        for (child_type* ptr = _first; ptr; ptr = ptr->_next)
            if (ptr->GetData() == item)
                return nullptr;

        child_type* ptr = new child_type(this, item);

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

    data_type* PushFront(const data_type& item)
    {
        for (node_type* ptr = _last; ptr; ptr = ptr->_prev)
            if (ptr->GetData() == item)
                return nullptr;

        child_type* ptr = new child_type(this, item);

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

    bool Update(const data_type& target, const data_type& item)
    {
        child_type* target_ptr = nullptr;

        for (child_type* ptr = _first; ptr; ptr = ptr->_next)
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

    bool Remove(const data_type& item)
    {
        for (child_type* ptr = _first; ptr; ptr = ptr->_next)
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

        for (child_type* ptr = _first; ptr; )
        {
            child_type* next_ptr = ptr->_next;

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
            if (child_type* target_ptr = _first->_next)
            {
                _last = _first;
                _first->_prev = nullptr;
                _first->_next = nullptr;

                do {
                    child_type* next_ptr = target_ptr->_next;

                    if (![this, &_compareFunc, &target_ptr]() {
                            for (child_type* insert_ptr = _first; insert_ptr; insert_ptr = insert_ptr->_next)
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
                } while (target_ptr)
            }
        }
    }

    void Sort()
    {
        Sort([](const data_type& left, const data_type& right) { return left < right; });
    }

    bool Move(const data_type& key, int offset)
    {
        if (0 < offset)
        {
            for (child_type* target_ptr = _first; target_ptr; target_ptr = target_ptr->_next)
            {
                if (&key == &target_ptr->GetData() || key == target_ptr->GetData())
                {
                    child_type* insert_ptr = target_ptr;
                    for (int i = 0; i < offset && insert_ptr != m_End; ++i)
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
            for (child_type* target_ptr = _last; target_ptr; target_ptr = target_ptr->_prev)
            {
                if (&key == &target_ptr->GetData() || key == target_ptr->GetData())
                {
                    child_type* insert_ptr = target_ptr;
                    for (int i = 0; offset < i && insert_ptr != m_Header; --i)
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
            for (child_type* ptr = _first; ptr; ptr = ptr->_next)
                if (&key == &ptr->GetData() || key == ptr->GetData())
                    return true;
        }

        return false;
    }

};

template<typename _DataT, typename _ChildT>
class tree_node : public list_node<_DataT>, public list_type<_ChildT>
{

};

#endif // !_bnb_Tree_H_

