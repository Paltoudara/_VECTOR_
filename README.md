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
# 📝NOTE THAT:
THIS IS THE INTERFACE OF THE VECTOR WITH THIS INTERFACE YOU CAN MANIPULATE THE VECTOR AND CAN USE IT IF YOU WANT MORE DETAILS ABOUT THE IMPLEMENTATION JUST SEE THE vector.h and Macros.h for how things are done
also this vector accepts only elements that are nothrow destructible.

# ⚙️Member functions
1) default constructor just initializes the vector pretty much the default state of the vector: no elements no capacity no size

2) a constructor that takes a capacity argument and just gives the vector this capacity or one if the capacity arg is equal to 0

3) this is the copy constructor that just  copies all the contents from other (this is a deep copy) also the vector takes the size and capacity of other

4) this is pretty much the same case as above but this time we have an initializer list and we take copies of the values of the initializer list

5) this is the move constructor we just steal the contets of the other and we leave it in a valid state it just a vector with zero capacity and size
   
6) capacity function just gives the capacity of the vector how many elements the vector currently can hold

7) empty functions just gives if the vector has size==0 or not simple

8) size function just give the size of the  elements that are in the vector right now not how many can hold the vector itself

9) push_back just pushes  an element by copy or move to the vector, if the vector doesn't have enough capacity size==capacity then we reallocate a new vector that has capacity doubled over the previous, if something goes wrong push_back does nothing (strong guarantee)

10) emplace_back does the same thing as push_back the only difference is that it just constructs the element in place and then pushes it back simple

11) pop_back just pops the last element from the vector reduces size by one and doesn't change the capacity no matter how many elements you pop, if you wanna change the capacity if you popped enough elements just use shrink_to_fit

12) show justs prints all the elements in the vector uses this func if the elements are printable (we can call cout in them

13) operator [] we use it just to access the vector and change its contents this doesn't check in release if you passed valid index(use with care)

14) at function this func we use it for the same purpose as operator [] but we check if the index is valid and then we throw for invalid indexes

15) destructor function justs destroys the vector and frees the memory and all the size=capacity=0

16) clear function it just deletes all the elements in the vector but doesn't affect capacity it justs affects size and makes it 0

17) front function it just gives the first element of the vector it doesn't check on release if the vector has any elements (use with care)

18)back function it just gives the last element of the vector it doesn't check on release if the vector has any elements (use with care)

19) swap functions just swaps the contents of two vectors

20)reserve just changes the capacity of the vector if you give a greater capacity of what the vector currently has, if something goes wrong  reserve doesn't do anything (strong guarantee)

21) shrink_to_fit function just shrinks the capacity of the vector to be equal to the size  of the vector, if something goes wrong shrink_to_fit doesn't do anything (strong guarantee)

22) resize function first overload :
    1. if the size==new_size then nothing happents
    2. if the size>new_size then we pop the elements from the vector we don't change the capacity
    3. if the size<new_size we push_back the default constructible objects of type _Ty if the new_size is greater than capacity we also reallocate space this func may change the state of the vector if something goes wrong see implementation
23) resize function second overload
