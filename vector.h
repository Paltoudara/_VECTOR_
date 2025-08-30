#pragma once
#include<iostream>
#include<new>
#include<algorithm>
#include<type_traits>
#include<cstdlib>
#include<initializer_list>
#include<utility>
#include<cassert>
#include<functional>
#include<memory>
#include"Macros.h"
_PANAGIOTIS_BEGIN
//-----------------------
//		INTERFACE BEGIN
//-----------------------
template<typename _Ty>
class vector final {
private:
	//private members an array of pointers
	//a size and a capacity
	//size shows how many elements the vector currently has
	//capacity shows how many elements the vector can hold
	std::size_t _capacity;
	std::size_t _size;
	_Ty** _array;
	//make iterator point to a valid vector on stack declared or in heap
	//don't use and iterator when it is invalid
	//THIS iterator is pretty much a wrapper around a pointer and an index 
	//all the constructors do the same thing they just initialize the _index,_owner
	//and also all the copy and move operators and destructors
	//note that this iterator is a template value=true for iterator
	//and value=false for const_iterator in order to have what functions
	//we need for every type of iterator
	//also note that the const_iterator is only for reading elements 
	//not change them
	template<bool value>
	class vector_iterator final {
	private:
		std::size_t _index;
		vector<_Ty>* _owner;
		friend class vector;
		//this cosntructor is private because it i only used with the begin,cbegin,cend,end functions
		vector_iterator(const std::size_t index, vector<_Ty>* const owner)
			noexcept :_index{ index }, _owner{ owner } {
		}
	public:
		//default state of the iterator
		vector_iterator()
			noexcept :_index{}, _owner{} {
		}
		//copy constructor default behavior
		vector_iterator(const vector_iterator& other)noexcept = default;
		//move constructor default behavior
		vector_iterator(vector_iterator&& other)noexcept = default;
		//copy operator we don't need to check it the other iterator is invalid it is your responsibility to not assign to another iterator 
		vector_iterator operator=(const vector_iterator& other)noexcept {
			_index = other._index;
			_owner = other._owner;
			return *this;
		}
		//move operator we don't need to check it the other iterator is invalid it is your responsibility to not assign to another iterator
		vector_iterator operator=(vector_iterator&& other)noexcept {
			_index = other._index;
			_owner = other._owner;
			return *this;
		}
		//operator ++ we only increment the iterator if it is in the range [0,size-1]
		vector_iterator operator ++()noexcept {
			if (_owner != nullptr && _index < _owner->_size) {
				_index++;
			}
			return { _index,_owner };
		}
		//operator ++ we only increment the iterator if it is in the range [0,size-1]
		vector_iterator operator++(int)noexcept {
			vector_iterator tmp{ _index,_owner };
			if (_owner != nullptr && _index < _owner->_size) {
				_index++;
			}
			return tmp;
		}
		//
		bool operator!=(const vector_iterator& other)const noexcept {
			return _index != other._index || other._owner != _owner;
		}
		//
		bool operator ==(const vector_iterator& other)const noexcept {
			return _index == other._index && other._owner == _owner;
		}
		//this operator is used only if the index is in the range[0,size-1]
		//its like the at function of the vector if index<_size we good
		const _Ty& operator *()const& {
			if (_owner != nullptr && _index < _owner->_size) {
				return *_owner->_array[_index];
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//this operator is used only if the index is in the range[0,size-1]
		//its like the at function of the vector if index<_size we good
		template<typename = std::enable_if_t<value>>
		_Ty& operator *()& {
			if (_owner != nullptr && _index < _owner->_size) {
				return *_owner->_array[_index];
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//this operator is used only if the index is in the range[0,size-1]
		//its like the at function of the vector if index<_size we good
		const _Ty&& operator *()const&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::move(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//this operator is used only if the index is in the range[0,size-1]
		//its like the at function of the vector if index<_size we good
		template<typename = std::enable_if_t<value>>
		_Ty&& operator *()&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::move(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//operator+=increments the iterator by a counter if index+counter
		//is in the range [0,size]
		vector_iterator operator+=(const std::size_t counter)noexcept {
			if (_owner != nullptr && _index + counter <= _owner->_size) {
				_index += counter;
			}
			return { _index,_owner };
		}
		//use it only to access the method of the object that the iterator
		//points to the container only in the range [0,size-1]
		const _Ty* operator->()const& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//use it only to access the method of the object that the iterator
		//points to the container only in the range [0,size-1]
		template<typename = std::enable_if_t<value>>
		_Ty* operator->()& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//use it only to access the method of the object that the iterator
		//points to the container only in the range [0,size-1]
		const _Ty* operator->()const&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//use it only to access the method of the object that the iterator
		//points to the container only in the range [0,size-1]
		template<typename = std::enable_if_t<value>>
		_Ty* operator->()&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		//use it only to access the method of the object that the iterator
		//points to the container only in the range [0,size-1]
		vector_iterator operator+(const std::size_t counter)const noexcept {
			vector_iterator curr{ _index,_owner };
			if (curr._owner != nullptr && curr._index + counter <= _owner->_size) {
				curr._index += counter;
			}
			return curr;
		}
		//operator -- decrements the iterator making it go backwards
		//only if the iterator is in the range(0,size]
		vector_iterator operator--()noexcept {
			if (_owner != nullptr && _index > 0 && _index <= _owner->_size) {
				_index--;
			}
			return { _index,_owner };
		}
		//
		vector_iterator operator--(int)noexcept {
			vector_iterator tmp{ _index,_owner };
			if (_owner != nullptr && _index > 0 && _index <= _owner->_size) {
				_index--;
			}
			return tmp;
		}
		//operator - decrements the iterator making it go backwards by
		//a counter only if the counter will make the iterator stay 
		//in the range[0,size] but this is done to a temporary iterator
		vector_iterator operator-(const std::size_t counter)const noexcept {
			vector_iterator curr{ _index,_owner };
			if (curr._owner != nullptr && counter <= curr._index &&
				curr._index - counter <= _owner->_size) {
				curr._index -= counter;
			}
			return curr;
		}
		//operator - decrements the iterator making it go backwards by
		//a counter only if the counter will make the iterator stay 
		//in the range[0,size]
		vector_iterator operator-=(const std::size_t counter)noexcept {
			if (_owner != nullptr && counter <= _index &&
				_index - counter <= _owner->_size) {
				_index -= counter;
			}
			return { _index,_owner };
		}
		//operator [] is like the operator [] from the vector
		//but it direferences the iterator with the index from 
		//where it is already at
		template<typename = std::enable_if_t<value>>
		_Ty& operator [](const std::size_t index)& {
			if (_owner != nullptr && _index + index < _owner->_size) {
				return *_owner->_array[_index + index];
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		//
		template<typename = std::enable_if_t<value>>
		_Ty&& operator [](const std::size_t index)&& {
			if (_owner != nullptr && _index + index < _owner->_size) {
				return std::move(*_owner->_array[_index + index]);
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		//
		const _Ty& operator [](const std::size_t index)const& {
			if (_owner != nullptr && _index + index < _owner->_size) {
				return *_owner->_array[_index + index];
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		//
		const _Ty&& operator [](const std::size_t index)const&& {
			if (_owner != nullptr && _index + index < _owner->_size) {
				return std::move(*_owner->_array[_index + index]);
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		//resets the iterator to the default state 
		~vector_iterator()noexcept {
			_index = 0;
			_owner = nullptr;
		}
	};
	template<class _Valty>
	void insert_element(const std::size_t index, _Valty&& _Val);
	template<class _Valty>
	void push_back_element(_Valty&& _Val);
public:
	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>, "no references allowed");
	static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
	static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");
	using iterator = vector_iterator<true>;
	using const_iterator = vector_iterator<false>;
	//default func constructor done
	vector()noexcept;
	//constructor func done
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


//-----------------------
//		IMPLEMENTATION BEGIN
//-----------------------

//insert_element func done
template<typename _Ty>
template<class _Valty>
void vector<_Ty>::insert_element(const std::size_t index, _Valty&& _Val) {
	if (index >= _size) {
		push_back(std::forward<_Valty>(_Val));
		return;
	}
	if (_size == _capacity) {//realloc
		_Ty** new_array = new _Ty * [2 * _capacity + 1] {};
		for (std::size_t i = 0; i < _size; i++) {
			new_array[i] = _array[i];
		}
		delete[]_array;
		_array = new_array;
		_capacity = _capacity * 2 + 1;
	}
	_Ty* ptr{ new _Ty(std::forward<_Valty>(_Val)) };
	for (std::size_t i = _size; i > index; i--) {
		_array[i] = _array[i - 1];
	}
	_array[index] = ptr;
	_size++;
}
//push_back_element func done
template<typename _Ty>
template<class _Valty>
void vector<_Ty>::push_back_element(_Valty&& _Val) {
	if (_size == _capacity) {
		std::size_t new_capacity = _capacity * 2 + 1;
		_Ty** new_array = new _Ty * [new_capacity] {}; // zero-initialize

		for (std::size_t i = 0; i < _size; ++i) {
			new_array[i] = _array[i]; // move pointers
		}
		if (_array != nullptr) {
			delete[] _array; // delete old pointer array (not the objects)
		}
		_array = new_array;
		_capacity = new_capacity;
	}
	_array[_size] = new _Ty(std::forward<_Valty>(_Val)); // allocate new object
	_size++;
}
//defaul constructor func done
//initializes the members to the default state
template<typename _Ty>
vector<_Ty>::vector()
noexcept :_capacity{}, _size{}, _array{} {
}
//constructor func done
//zero capacity  is not allowed in this constructor
template<typename _Ty>
vector<_Ty>::vector(const std::size_t capacity) :
	_size{}, _capacity{ capacity > 0 ? capacity : 1 }
{
	_array = new _Ty * [capacity > 0 ? capacity : 1]{};
}
//copy constructor func done
template<typename _Ty>
vector<_Ty>::vector(const vector<_Ty>& other) :_capacity{}, _size{}, _array{}
{
	static_assert(std::is_copy_constructible_v<_Ty>, "the type"
		"must be copy constructible");
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type"
		"must be destructible without throwing");
	if (other._capacity == 0)return;
	_Ty** new_array = new _Ty * [other._capacity] {};
	std::size_t i{};
	//makes a deep copy of the other vector passed 
	//if everything is good if it is not an exception is thrown
	//and we delete everything that we tried creating
	try {
		for (; i < other._size; i++) {
			//make a deep copy of the contents
			new_array[i] = new _Ty(*other._array[i]);
		}
	}
	catch (...) {
		//delete everything that you crafted is something goes wrong
		//and rethrow 
		for (std::size_t j = 0; j < i; j++) {
			delete new_array[j];
		}
		delete[]new_array;
		throw 1;
	}
	//if everything is good we make our vector take this new_array
	_array = new_array;
	_capacity = other._capacity;
	_size = other._size;
}
//constructor with initializer list done
//pretty much same case as copy constructor
//but with initializer list for other not a vector
template<typename _Ty>
vector<_Ty>::vector(const std::initializer_list<_Ty>& other)
	:_array{}, _size{}, _capacity{}
{
	static_assert(std::is_copy_constructible_v<_Ty>, "the type must"
		"be copy constructible in order to use this func");
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type"
		"must be destructible without throwing");
	if (other.size() == 0)return;
	_Ty** new_array = new _Ty * [other.size()];
	const _Ty* ptr{ other.begin() };
	std::size_t i{};
	try {
		for (; i < other.size(); i++) {
			new_array[i] = new _Ty(*ptr);
			ptr++;
		}
	}
	catch (...) {
		for (std::size_t j = 0; j < i; j++) {
			delete new_array[j];
		}
		delete[]new_array;
		throw 1;
	}
	_array = new_array;
	_capacity = _size = other.size();
}
//move constructor func done
//still the values from the other vector
//if we move to ourselves again we take the default state
template<typename _Ty>
vector<_Ty>::vector(vector<_Ty>&& other)noexcept
	:_array{}, _size{}, _capacity{}
{
	std::swap(_array, other._array);
	std::swap(_capacity, other._capacity);
	std::swap(_size, other._size);
}
//capacity func done
//gives the capacity of the vector
template<typename _Ty>
std::size_t vector<_Ty>::capacity()const noexcept {
	return _capacity;
}
//empty func done
//gives if the vector currently has any elements
//or not
template<typename _Ty>
bool vector<_Ty>::empty()const noexcept {
	return _size == 0;
}
//size func done
//gives the size of the vector 
template<typename _Ty>
std::size_t vector<_Ty>::size()const noexcept {
	return _size;
}
//push_back func done
//the _Ty type must be copy constructible because we are going to make
//a copy
template<typename _Ty>
void vector<_Ty>::push_back(const _Ty& data) {
	static_assert(std::is_copy_constructible_v<_Ty>, "the type must be copy constructible");
	push_back_element(data);
}
//push_back func done
//the _Ty type must be move constructible because we are going to make
//a move
template<typename _Ty>
void vector<_Ty>::push_back(_Ty&& data) {
	static_assert(std::is_move_constructible_v<_Ty>, "the type must be move constructible");
	push_back_element(std::move(data));
}
//emplace_back func done
//same case as push back but this time we construct the element
//in place 
template<typename _Ty>
template<class ..._Valty>
void vector<_Ty>::emplace_back(_Valty&&..._Val) {
	if (_size == _capacity) {
		std::size_t new_capacity = _capacity * 2 + 1;
		_Ty** new_array = new _Ty * [new_capacity] {}; // zero-initialize

		for (std::size_t i = 0; i < _size; ++i) {
			new_array[i] = _array[i]; // move pointers
		}
		if (_array != nullptr) {
			delete[] _array; // delete old pointer array (not the objects)
		}
		_array = new_array;
		_capacity = new_capacity;
	}
	_array[_size] = new _Ty(std::forward<_Valty>(_Val)...); // allocate new object
	_size++;
}
//pop_back func done
//we pop one element from the top of the vector like a static stack
//because its an array of pointers the pointers not used are all nullptr
template<typename _Ty>
void vector<_Ty>::pop_back()noexcept {
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the"
		"type must be destructible without throwing");
	if (_size > 0) {
		_size--;
		delete _array[_size];
		_array[_size] = nullptr;
		return;
	}//pop_back doesn't shrink capacity for simplicity
	//if you wanna shrink capacity use shrink to fit 
}
//show func done
//just prints the contents of the array
//use this func only if the _Ty type is printable with cout
template<typename _Ty>
void vector<_Ty>::show()const {
	for (std::size_t i = 0; i < _size; i++) {
		std::cout << *_array[i] << '\n';
	}
}
//operator []func done 
//gives the element from that index of the vector
//but with no checks in release
template<typename _Ty>
const _Ty& vector<_Ty>::operator [](const std::size_t index) const&
{
	assert(index < _size);
	return *_array[index];
}
//operator []func done 
//gives the element from that index of the vector
//but with no checks in release
template<typename _Ty>
_Ty& vector<_Ty>::operator [](const std::size_t index)& {
	assert(index < _size);
	return *_array[index];
}
//operator []func done 
//gives the element from that index of the vector
//but with no checks in release
template<typename _Ty>
_Ty&& vector<_Ty>::operator [](const std::size_t index)&&
{
	assert(index < _size);
	return std::move(*_array[index]);
}
//operator []func done 
//gives the element from that index of the vector
//but with no checks in release
template<typename _Ty>
const _Ty&& vector<_Ty>::operator [](const std::size_t index)const&& {
	assert(index < _size);
	return std::move(*_array[index]);
}
//at func done
//same thing as operator [] but this time we check for valid index
//invalid indexes raise exceptions
template<typename _Ty>
const _Ty& vector<_Ty>::at(const std::size_t index) const&
{
	if (index < _size) {
		return *_array[index];
	}
	throw out_of_bounds{ "tried to access invalid pos" };
}
//at func done
//same thing as operator [] but this time we check for valid index
//invalid indexes raise exceptions
template<typename _Ty>
_Ty& vector<_Ty>::at(const std::size_t index)& {
	if (index < _size) {
		return *_array[index];
	}
	throw out_of_bounds{ "tried to access invalid pos" };
}
//at func done
//same thing as operator [] but this time we check for valid index
//invalid indexes raise exceptions 
template<typename _Ty>
_Ty&& vector<_Ty>::at(const std::size_t index)&&
{
	if (index < _size) {
		return std::move(*_array[index]);
	}
	throw out_of_bounds{ "tried to access invalid pos" };
}
//at func done
//same thing as operator [] but this time we check for valid index
//invalid indexes raise exceptions
template<typename _Ty>
const _Ty&& vector<_Ty>::at(const std::size_t index)const&& {
	if (index < _size) {
		return std::move(*_array[index]);
	}
	throw out_of_bounds{ "tried to access invalid pos" };
}
//destructor func done
//deletes the array first the objects pointed by the pointers and then
//the array of pointers
template<typename _Ty>
vector<_Ty>::~vector()noexcept {
	static_assert(std::is_nothrow_destructible_v<_Ty>,
		"the type must be destructible without throwing");
	if (_array != nullptr) {
		for (std::size_t i = 0; i < _size; i++) {
			delete _array[i];
		}
		delete[]_array;
	}
	_array = nullptr;
	_capacity = _size = 0;
}
//clear func done
//deletes all the elements that the pointers point and we have zero
//size no capacity is modified
template<typename _Ty>
void vector<_Ty>::clear()noexcept {
	static_assert(std::is_nothrow_destructible_v<_Ty>,
		"the type must be destructible without throwing");
	for (std::size_t i = 0; i < _size; i++) {
		delete _array[i];
		_array[i] = nullptr;
	}
	_size = 0;
}
//front func done
//the first element of the array
//with no checks on release 
template<typename _Ty>
const _Ty& vector<_Ty>::front() const&
{
	assert(!empty());
	return *_array[0];
}
//front func done
//the first element of the array
//with no checks on release 
template<typename _Ty>
_Ty& vector<_Ty>::front()& {
	assert(!empty());
	return *_array[0];
}
//front func done
//the first element of the array
//with no checks on release 
template<typename _Ty>
_Ty&& vector<_Ty>::front()&&
{
	assert(!empty());
	return std::move(*_array[0]);
}
//front func done
//the first element of the array
//with no checks on release 
template<typename _Ty>
const _Ty&& vector<_Ty>::front()const&& {
	assert(!empty());
	return std::move(*_array[0]);
}
//back func done
//the last element of the array
//with no checks on release
template<typename _Ty>
const _Ty& vector<_Ty>::back() const&
{
	assert(!empty());
	return *_array[_size - 1];
}
//back func done
//the last element of the array
//with no checks on release
template<typename _Ty>
_Ty& vector<_Ty>::back()& {
	assert(!empty());
	return *_array[_size - 1];
}
//back func done
//the last element of the array
//with no checks on release
template<typename _Ty>
_Ty&& vector<_Ty>::back()&&
{
	assert(!empty());
	return std::move(*_array[_size - 1]);
}
//back func done
//the last element of the array
//with no checks on release
template<typename _Ty>
const _Ty&& vector<_Ty>::back()const&& {
	assert(!empty());
	return std::move(*_array[_size - 1]);
}
//swap func done
//swaps contnets of two vectors we check to not swap into ourselves
template<typename _Ty>
void vector<_Ty>::swap(vector<_Ty>& other)noexcept {
	if (this != &other) {
		std::swap(_array, other._array);
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);
	}
}
//reserve func done
//reservers capacity for the vector pretty much increases the number
//of elements that a vector can hold
template<typename _Ty>
void vector<_Ty>::reserve(const std::size_t new_capacity) {
	//if the new_capacity is less than _capacity this func has not result
	//we create the new vector and just copy pointers those reallocations
	//are guaranteed to be fast because we only copy pointers so is the same
	//cost always
	if (new_capacity > _capacity) {
		_Ty** new_array = new _Ty * [new_capacity] {};
		for (std::size_t i = 0; i < _size; i++) {
			new_array[i] = _array[i];
		}
		if (_array != nullptr) {
			delete[]_array;
		}
		_array = new_array;
		_capacity = new_capacity;
	}
	return;
}
//shrink_to_fit func done
//just makes capacity equal to size simple if the capacity equals to
//size nothing happens
template<typename _Ty>
void vector<_Ty>::shrink_to_fit() {
	if (_size == _capacity) return;
	if (_size == 0) {
		delete[] _array;
		_array = nullptr;
		_capacity = 0;
		return;
	}
	_Ty** new_array = new _Ty * [_size] {};
	for (std::size_t i = 0; i < _size; i++) {
		new_array[i] = _array[i];
	}
	delete[]_array;
	_array = new_array;
	_capacity = _size;
}
//resize func done
//if the size=new_size this func does nothing
//if the size>new_size just pop elements
//if the size<new_size we reallocate if the new_size>capacity 
//and then we push the elements
//or we just push if new_size<=capacity
//this func might modifie the state of the vector
//because we might be able to reserve but not push any of the elements
//or push some of them this changes of the vector
//in the end you might have some extra elements pushed into the vector
template<typename _Ty>
void vector<_Ty>::resize(const std::size_t new_size) {
	static_assert(std::is_default_constructible_v<_Ty>, "the type must"
		"be default constructible");
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type must"
		"be destructible without throwing");
	if (_size > new_size) {//smaller size,use pop_back
		std::size_t iterations{ _size - new_size };
		for (std::size_t i = 0; i < iterations; i++) {
			_size--;
			delete _array[_size];
			_array[_size] = nullptr;
		}
	}
	else if (_size < new_size) {//bigger size,use push_back
		std::size_t iterations{ new_size - _size };
		if (new_size > _capacity) {
			reserve(new_size);
		}
		for (std::size_t i = 0; i < iterations; i++) {
			_array[_size] = new _Ty{}; // allocate new object
			_size++;
		}
	}
}
//resize func done
//because we might be able to reserve but not push any of the elements
//or push some of them this changes of the vector
//in the end you might have some extra elements pushed into the vector
template<typename _Ty>
void vector<_Ty>::resize(const std::size_t new_size, const _Ty& value) {
	static_assert(std::is_copy_constructible_v<_Ty>, "the type must"
		"be copy constructible");
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type must"
		"be destructible without throwing");
	if (_size > new_size) {//smaller size,use pop_back
		std::size_t iterations{ _size - new_size };
		for (std::size_t i = 0; i < iterations; i++) {
			_size--;
			delete _array[_size];
			_array[_size] = nullptr;
		}
	}
	else if (_size < new_size) {//bigger size,use push_back
		std::size_t iterations{ new_size - _size };
		if (new_size > _capacity) {
			reserve(new_size);
		}
		for (std::size_t i = 0; i < iterations; i++) {
			_array[_size] = new _Ty(value); // allocate new object
			_size++;
		}
	}
}
//copy operator func done
template<typename _Ty>
vector<_Ty>& vector<_Ty>::operator =(const vector<_Ty>& other)& {
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type"
		"must be destructible without throwing");
	if (other._capacity == 0) {
		this->~vector();
		return *this;
	}
	_Ty** new_array = new _Ty * [other._capacity] {};
	std::size_t i{};
	try {
		for (; i < other._size; i++) {
			new_array[i] = new _Ty(*other._array[i]);
		}
	}
	catch (...) {
		for (std::size_t j = 0; j < i; j++) {
			delete new_array[j];
		}
		delete[]new_array;
		throw 1;
	}
	this->~vector();
	_array = new_array;
	_capacity = other._capacity;
	_size = other._size;
	return *this;

}
//assign func done
template<typename _Ty>
void vector<_Ty>::assign(const std::size_t count, const _Ty& value) {
	static_assert(std::is_copy_constructible_v<_Ty>, "the type must"
		"be copy assignable");
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type"
		"must be destructible without throwing");
	if (count == 0) {
		clear();
		return;
	}
	_Ty** new_array = new _Ty * [count] {};
	std::size_t i{};
	try {
		for (; i < count; i++) {
			new_array[i] = new _Ty(value);
		}
	}
	catch (...) {
		for (std::size_t j = 0; j < i; j++) {
			delete new_array[j];
		}
		delete[]new_array;
		throw 1;
	}
	this->~vector();
	_array = new_array;
	_capacity = _size = count;

}
//move operator func done
template<typename _Ty>
vector<_Ty>& vector<_Ty>::operator =(vector<_Ty>&& other) & noexcept {
	this->~vector();
	std::swap(_array, other._array);
	std::swap(_size, other._size);
	std::swap(_capacity, other._capacity);
	return *this;
}
// Returns a pointer to the internal array of pointers.
// The vector retains ownership of the objects.
// Users may read or modify the objects via the pointers, but must NOT delete them.
template<typename _Ty>
_Ty** vector<_Ty>::data()noexcept {
	return _array;
}
//implement one insert 
template<typename _Ty>
void vector<_Ty>::insert(const std::size_t index, const _Ty& value) {
	static_assert(std::is_copy_constructible_v<_Ty>, "the type"
		"must be copy constructible");
	insert_element(index, value);
}
template<typename _Ty>
void vector<_Ty>::insert(const std::size_t index, _Ty&& value) {
	static_assert(std::is_move_constructible_v<_Ty>, "the type"
		"must be move constructible");
	insert_element(index, std::move(value));
}
template<typename _Ty>
template<class..._Valty>
void vector<_Ty>::emplace(const std::size_t index, _Valty&&..._Val) {
	if (index >= _size) {
		emplace_back(std::forward<_Valty>(_Val)...);
		return;
	}
	if (_size == _capacity) {//realloc
		_Ty** new_array = new _Ty * [2 * _capacity + 1] {};
		for (std::size_t i = 0; i < _size; i++) {
			new_array[i] = _array[i];
		}
		delete[]_array;
		_array = new_array;
		_capacity = _capacity * 2 + 1;
	}
	_Ty* ptr{ new _Ty(std::forward<_Valty>(_Val)...) };
	for (std::size_t i = _size; i > index; i--) {
		_array[i] = _array[i - 1];
	}
	_array[index] = ptr;
	_size++;
}
//prosoxh stous invalid iteratos se pointers pou den yparxoun pleon
template<typename _Ty>
void vector<_Ty>::erase(const std::size_t index)noexcept {
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type"
		"must be destructible without throwing");
	if (index >= _size) return;
	delete _array[index];
	//apo ekei kai pera shift
	for (std::size_t i = index; i < _size - 1; i++) {
		_array[i] = _array[i + 1];
	}
	_array[_size - 1] = nullptr;
	_size--;
}
//begin func done
template<typename _Ty>
vector<_Ty>::iterator vector<_Ty>::begin()noexcept {
	return { 0,this };
}
//end func done
template<typename _Ty>
vector<_Ty>::iterator vector<_Ty>::end()noexcept {
	return { _size,this };
}
//cbegin func done
template<typename _Ty>
vector<_Ty>::const_iterator vector<_Ty>::cbegin()noexcept {
	return { 0,this };
}
//cend func done
template<typename _Ty>
vector<_Ty>::const_iterator vector<_Ty>::cend()noexcept {
	return { _size,this };
}
//operator = func with initializer_list
template<typename _Ty>
vector<_Ty>& vector<_Ty>::operator=(const std::initializer_list<_Ty>& other)& {
	static_assert(std::is_copy_constructible_v<_Ty>, "the type must"
		"be copy constructible in order to use this func");
	static_assert(std::is_nothrow_destructible_v<_Ty>, "the type"
		"must be destructible without throwing");
	if (other.size() == 0) {
		this->~vector();
		return *this;
	}
	_Ty** new_array = new _Ty * [other.size()] {};
	const _Ty* ptr{ other.begin() };
	std::size_t i{};
	try {
		for (; i < other.size(); i++) {
			new_array[i] = new _Ty(*ptr);
			ptr++;
		}
	}
	catch (...) {
		for (std::size_t j = 0; j < i; j++) {
			delete new_array[j];
		}
		delete[]new_array;
		throw 1;
	}
	this->~vector();
	_array = new_array;
	_capacity = _size = other.size();
	return *this;
}
//-----------------------
//		IMPLEMENTATION END
//-----------------------

_PANAGIOTIS_END
