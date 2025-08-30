# 🔗 Welcome to the VECTOR API 🌐 :
## 🧩 Interface

### ✅ Option 1: Add a dummy body
```markdown
```cpp
template<typename _Ty>
class vector final{
public:
	using iterator = vector_iterator<true>;
	using const_iterator = vector_iterator<false>;
	vector()noexcept;
	vector(const std::size_t capacity);
	//copy constructor func done
	vector(const vector<_Ty>& other);
	//constructor with initializer_list func done
	vector(const std::initializer_list<_Ty>& other);
	//move constructor func done
	vector(vector<_Ty>&& other)noexcept;
	//capacity func done
	std::size_t capacity()const noexcept;
	bool empty()const noexcept;
	//size func done 
	std::size_t size()const noexcept;
	//push_back func done
	void push_back(const _Ty& data);
	//push back func done
	void push_back(_Ty&& data);
	//emplace back func done
	template<class ..._Valty>
	void emplace_back(_Valty&&..._Val);
	//pop_back func done
	void pop_back()noexcept;
	//
	void show() const;
	//operator []func done 
	const _Ty& operator [](const std::size_t index) const&;
	//operator []func done 
	_Ty& operator [](const std::size_t index)&;
	//operator []func done 
	_Ty&& operator [](const std::size_t index)&&;
	//operator []func done 
	const _Ty&& operator [](const std::size_t index)const&&;
	//at func done 
	const _Ty& at(const std::size_t index) const&;
	//at func done 
	_Ty& at(const std::size_t index)&;
	//at func done 
	_Ty&& at(const std::size_t index)&&;
	//at func done 
	const _Ty&& at(const std::size_t index)const&&;
	//destructor done
	~vector()noexcept;
	//clear func done 
	void clear()noexcept;
	//front func done
	const _Ty& front() const&;
	//front func done 
	_Ty& front()&;
	//front func done
	_Ty&& front()&&;
	//front func done
	const _Ty&& front()const&&;
	//back func done
	const _Ty& back() const&;
	//back func done 
	_Ty& back()&;
	//back func done
	_Ty&& back()&&;
	//back func done
	const _Ty&& back()const&&;
	//swap func done
	void swap(vector<_Ty>& other)noexcept;
	//reserve func done
	void reserve(const std::size_t new_capacity);
	//shring_to_fit func done
	void shrink_to_fit();
	//resize func done
	void resize(const std::size_t new_size);
	//resize func done
	void resize(const std::size_t new_size, const _Ty& value);
	//copy operator func done
	vector<_Ty>& operator =(const vector<_Ty>& other)&;
	//assign func done
	void assign(const std::size_t count, const _Ty& value);
	//move operator func done
	vector<_Ty>& operator =(vector<_Ty>&& other) & noexcept;
	//
	vector<_Ty>& operator =(const std::initializer_list<_Ty>& other)&;
	// Returns a pointer to the internal array of pointers.
	// The vector retains ownership of the objects.
	// Users may read or modify the objects via the pointers, but must NOT delete them.
	_Ty** data()noexcept;
	//implement one insert 
	void insert(const std::size_t index, const _Ty& value);
	void insert(const std::size_t index, _Ty&& value);
	template<class..._Valty>
	void emplace(const std::size_t index, _Valty&&..._Val);
	//prosoxh stous invalid iteratos se pointers pou den yparxoun pleon
	void erase(const std::size_t index)noexcept;
	iterator begin()noexcept;
	iterator end()noexcept;
	const_iterator cbegin()noexcept;
	const_iterator cend()noexcept;
	
};
//-----------------------
//		INTERFACE END
//-----------------------




