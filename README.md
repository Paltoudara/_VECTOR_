# 🔗 Welcome to the VECTOR API 🌐 :
## 🧩 Interface

```markdown
```cpp
template<typename _Ty>
class vector final {
public:
	using iterator = vector_iterator<true>;
	using const_iterator = vector_iterator<false>;

	vector()noexcept;

	vector(const std::size_t capacity);

	vector(const vector<_Ty>& other);

	vector(const std::initializer_list<_Ty>& other);

	vector(vector<_Ty>&& other)noexcept;

	std::size_t capacity()const noexcept;

	bool empty()const noexcept;

	std::size_t size()const noexcept;

	void push_back(const _Ty& data);

	void push_back(_Ty&& data);

	template<class ..._Valty>
	void emplace_back(_Valty&&..._Val);

	void pop_back()noexcept;

	void show() const;

	const _Ty& operator [](const std::size_t index) const&;

	_Ty& operator [](const std::size_t index)&;

	_Ty&& operator [](const std::size_t index)&&;

	const _Ty&& operator [](const std::size_t index)const&&;

	const _Ty& at(const std::size_t index) const&;

	_Ty&at(const std::size_t index)&;
	
	_Ty&& at(const std::size_t index)&&;

	const _Ty&& at(const std::size_t index)const&&;

	~vector()noexcept;

	void clear()noexcept;

	const _Ty& front() const&;

	_Ty& front()&;

	_Ty&& front()&&;

	const _Ty&& front()const&&;

	const _Ty& back() const&;

	_Ty& back()&;

	_Ty&& back()&&;

	const _Ty&& back()const&&;

	void swap(vector<_Ty>& other)noexcept;

	void reserve(const std::size_t new_capacity);

	void shrink_to_fit();

	void resize(const std::size_t new_size);

	void resize(const std::size_t new_size, const _Ty& value);

	vector<_Ty>& operator =(const vector<_Ty>& other)&;

	void assign(const std::size_t count, const _Ty& value);

	vector<_Ty>& operator =(vector<_Ty>&& other) & noexcept;

	vector<_Ty>& operator =(const std::initializer_list<_Ty>& other)&;

	_Ty** data()noexcept;

	void insert(const std::size_t index, const _Ty& value);

	void insert(const std::size_t index, _Ty&& value);

	template<class..._Valty>
	void emplace(const std::size_t index, _Valty&&..._Val);

	void erase(const std::size_t index)noexcept;

	iterator begin()noexcept;

	iterator end()noexcept;

	const_iterator cbegin()noexcept;

	const_iterator cend()noexcept;

};


```
1) default constructor just initializes the vector pretty much the default state of the vector: no elements no capacity no size

2) a constructor that takes a capacity argument and just gives the vector this capacity or one if the capacity arg is equal to 0

3) this is the copy constructor that just  copies all the contents from other (this is a deep copy) also the vector takes the size and capacity of other

4) this is pretty much the same case as above but this time we have an initializer list and we take copies of the values of the initializer list

5) this is the move constructor we just steal the contets of the other and we leave it in a valid state it just a vector with zero capacity and size
   
6) capacity function just gives the capacity of the vector how many elements the vector currently can hold
