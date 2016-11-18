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

	using char_t = char;
	using byte_t = unsigned char;
	using size_t = unsigned int;

	using string_type = std::basic_string<char_t, std::char_traits<char_t>, std::allocator<char_t>>;
	using memory_type = std::basic_string<byte_t, std::char_traits<byte_t>, std::allocator<byte_t>>;

	inline bool operator < (const string_type& a, const string_type& b) { return (_stricmp(a.c_str(), b.c_str()) < 0); }
	inline bool operator > (const string_type& a, const string_type& b) { return (b < a); }
	inline bool operator == (const string_type& a, const string_type& b) { return (0 == _stricmp(a.c_str(), b.c_str())); }
	inline bool operator != (const string_type& a, const string_type& b) { return !(a == b); }

	extern size_t _g_id;
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
	class tree_type
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

			node_type() : m_Next(nullptr) { }
			explicit node_type(const key_type& key) : m_Pair(key), m_Next(nullptr) { }
			node_type(const key_type& key, const value_type& value) : m_Pair(key, value), m_Next(nullptr) { }

			data_type  m_Pair;
			node_type* m_Next;

		};

		node_type* m_Head{ nullptr };
		size_t m_nCount{ 0 };

		tree_type(const tree_type&) = delete;
		tree_type& operator=(const tree_type&) = delete;

	public:

		tree_type() = default;

		~tree_type() { Clear(); }

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

		node_type* Head() { return m_Head; }
		const node_type* Head() const { return m_Head; }

		template<typename _Func>
		void Foreach(_Func pFunc)
		{
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                pFunc(ptr->m_Pair);
		}

		template<typename _Func>
		void Foreach(_Func pFunc) const
		{
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                pFunc(ptr->m_Pair);
		}

        template<typename _Func>
        bool Action(_Func pFunc)
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                if (pFunc(ptr->m_Pair))
                    return true;

            return false;
        }

        template<typename _Func>
        bool Action(_Func pFunc) const
        {
            for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
                if (pFunc(ptr->m_Pair))
                    return true;

            return false;
        }

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

		data_type* Insert(const key_type& key, const value_type& value)
		{
			if (auto ptr = Insert(key))
			{
				ptr->m_Value = value;
				return ptr;
			}

			return nullptr;
		}

		bool Remove(const key_type& key)
		{
			for (node_type *prev = nullptr, *curr = m_Head; curr; curr = curr->m_Next)
			{
				if (&key == &curr->m_Pair.m_Key || key == curr->m_Pair.m_Key)
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

		data_type* _Update(const key_type& target, const key_type& key)
		{
			data_type* ptr_target = nullptr;
			for (node_type* ptr = m_Head; ptr; ptr = ptr->m_Next)
			{
				if (nullptr == ptr_target)
				{
					if (&target == &ptr->m_Pair.m_Key || target == ptr->m_Pair.m_Key)
					{
						ptr_target = &ptr->m_Pair;
						continue;
					}
				}

				if (key == ptr->m_Pair.m_Key)
					return false;
			}

			if (ptr_target)
				ptr_target->m_Key = key;

			return ptr_target;
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

	};

	struct credential_base
	{
	public:

		const size_t m_ID;
		string_type m_strName;

	protected:

		credential_base(const string_type& name) : m_ID(_g_id++), m_strName(name) { }

		credential_base(const credential_base& other) : m_ID(_g_id++), m_strName(other.m_strName) { }
		credential_base& operator=(const credential_base& other) {
			if (&other != this)
				m_strName = other.m_strName;

			return *this;
		}
	};

	struct platform_type : public credential_base
	{
		platform_type(const string_type& name = string_type(), const string_type& url = string_type(), const string_type& display = string_type())
			: credential_base(name)
			, m_strUrl(url)
			, m_strDisplay(display)
		{ }

		string_type m_strUrl;
		string_type m_strDisplay;
	};

	struct account_type : public credential_base
	{
		account_type(const string_type& name = string_type(), const string_type& display = string_type())
			: credential_base(name)
			, m_strDisplay(display)
		{ }

		string_type m_strDisplay;
	};

	struct property_key : public credential_base
	{
		property_key(const string_type& name = string_type())
			: credential_base(name)
		{ }

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

	using property_tree = tree_type<property_key, property_value>;
	using account_tree = tree_type<account_type, property_tree>;
	using platform_tree = tree_type<platform_type, account_tree>;

	inline property_tree::data_type* property_tree::Insert(const property_key& key)
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
		string_type m_strDisplay;
		unsigned long long m_ullTime;
		const size_t m_uID;

		platform_tree m_Tree;

		Credential(const Credential&) = delete;
		Credential& operator=(const Credential&) = delete;

	public:

		Credential() : m_uID(_g_id++) { }
		explicit Credential(const string_type& strWord) : m_uID(_g_id++), m_strWord(strWord) { }

		void Clear();

		bool IsValid() const { return !(m_strUser.empty() || m_strWord.empty()); }

		platform_tree& Tree() { return m_Tree; }
		const platform_tree& Tree() const { return m_Tree; }

		size_t GetID() const { return m_uID; }
		const string_type& GetWord() const { return m_strWord; }
		const string_type& GetUser() const { return m_strUser; }
		const string_type& GetDisplay() const { return m_strDisplay; }
		unsigned long long GetTime() const { return m_ullTime; }
		void SetWord(const string_type& strWord) { m_strWord = strWord; }
		void SetUser(const string_type& strUser) { m_strUser = strUser; }
		void SetDisplay(const string_type& strDisplay) { m_strDisplay = strDisplay; }
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

}

#endif
