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
#include"Header.h"
_PANAGIOTIS_BEGIN
template<typename _Ty>
class vector {
private:
	std::size_t _capacity;
	std::size_t _size;
	_Ty** _array;
	class vector_iterator {
	private:
		std::size_t _index;
		vector<_Ty>* _owner;
		friend class vector;
		vector_iterator(const std::size_t index,vector<_Ty>* const owner)
			noexcept :_index{ index }, _owner{ owner }{ }
	public:
		vector_iterator()
			noexcept:_index{},_owner{}{ }
		vector_iterator(const vector_iterator& other)noexcept = default;
		vector_iterator(vector_iterator&& other)noexcept = default;
		vector_iterator operator=(const vector_iterator& other)noexcept {
			_index = other._index;
			_owner = other._owner;
			return *this;
		}
		vector_iterator operator=(vector_iterator&& other)noexcept {
			_index = other._index;
			_owner = other._owner;
			return *this;
		}
		vector_iterator operator ++()noexcept{
			if (_owner != nullptr &&_index<_owner->_size) {
				_index++;
			}
			return { _index,_owner };
		}
		vector_iterator operator++(int)noexcept {
			vector_iterator tmp{ _index,_owner };
			if (_owner != nullptr && _index<_owner->_size) {
				_index++;
			}
			return tmp;
		}
		bool operator!=(const vector_iterator& other)const noexcept {
			return _index != other._index || other._owner != _owner;
		}
		//
		bool operator ==(const vector_iterator& other)const noexcept {
			return _index==other._index &&other._owner==_owner;
		}
		const _Ty& operator *()const& {
			if (_owner != nullptr && _index < _owner->_size) {
				return *_owner->_array[_index];
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		_Ty& operator *()& {
			if (_owner != nullptr && _index < _owner->_size) {
				return *_owner->_array[_index];
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		const _Ty&& operator *()const&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::move(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		_Ty&& operator *()&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::move(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		vector_iterator operator+=(const std::size_t counter)noexcept {
			if (_owner != nullptr && _index + counter <=_owner->_size) {
				_index += counter;
			}
			return { _index,_owner };
		}
		const _Ty* operator->()const& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		_Ty* operator->()& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		const _Ty* operator->()const&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		_Ty* operator->()&& {
			if (_owner != nullptr && _index < _owner->_size) {
				return std::addressof(*_owner->_array[_index]);
			}
			throw out_of_bounds{ "tried to access an invalid index" };
		}
		vector_iterator operator+(const std::size_t counter)const noexcept {
			vector_iterator curr{ _index,_owner };
			if (curr._owner != nullptr && curr._index + counter <=_owner->_size) {
				curr._index += counter;
			}
			return curr;
		}
		vector_iterator operator--()noexcept {
			if (_owner != nullptr && _index > 0 && _index <=_owner->_size) {
				_index--;
			}
			return { _index,_owner };
		}
		vector_iterator operator--(int)noexcept {
			vector_iterator tmp{ _index,_owner };
			if (_owner != nullptr && _index > 0 && _index <=_owner->_size) {
				_index--;
			}
			return tmp;
		}
		vector_iterator operator-(const std::size_t counter)const noexcept {
			vector_iterator curr{ _index,_owner };
			if (_owner != nullptr&&counter<=curr._index &&
				curr._index-counter>0 &&curr._index-counter<=_owner->_size) {
				curr._index -= counter;
			}
			return curr;
		}
		vector_iterator operator-=(const std::size_t counter)noexcept {
			if (_owner != nullptr && counter <=_index &&
				_index-counter > 0 && _index-counter <=_owner->_size) {
				_index -= counter;
			}
			return { _index,_owner };
		}
		_Ty& operator [](const std::size_t index)& {
			if (_owner != nullptr && _index + index < _owner->_size) {
				return *_owner->_array[_index + index];
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		_Ty&& operator [](const std::size_t index)&&{
			if (_owner != nullptr && _index + index < _owner->_size) {
				return std::move(*_owner->_array[_index + index]);
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		const _Ty& operator [](const std::size_t index)const & {
			if (_owner != nullptr && _index + index < _owner->_size) {
				return *_owner->_array[_index + index];
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		const _Ty&& operator [](const std::size_t index)const && {
			if (_owner != nullptr && _index + index < _owner->_size) {
				return std::move(*_owner->_array[_index + index]);
			}
			throw out_of_bounds{ "tried to access invalid index" };
		}
		~vector_iterator()noexcept {
			_index = 0;
			_owner = nullptr;
		}
	};

public:
	using iterator = vector_iterator;
	//default func constructor done
	vector()noexcept :_capacity{}, _size{}, _array{}{}
	//constructor func done
	vector(const std::size_t capacity) :
		_size{}, _capacity{ capacity > 0 ? capacity : 1 }
	{
		_array = new _Ty * [capacity > 0 ? capacity : 1]{};
	}
	//copy constructor func done
	vector(const vector<_Ty>& other) :_capacity{}, _size{}, _array{}
	{
		static_assert(std::is_copy_constructible_v<_Ty>, "the type"
			"must be copy constructible");
		static_assert(std::is_nothrow_destructible_v<_Ty>, "the type"
			"must be destructible without throwing");
		if (other._capacity == 0)return;
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
		_array = new_array;
		_capacity = other._capacity;
		_size = other._size;
		
	}
	//constructor with initializer_list func done
	vector(const std::initializer_list<_Ty>&other)
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
	vector(vector<_Ty>&& other)noexcept 
		:_array{}, _size{}, _capacity{}
	{
		std::swap(_array, other._array);
		std::swap(_capacity, other._capacity);
		std::swap(_size, other._size);
	}
	//capacity func done
	std::size_t capacity()const noexcept {
		return _capacity;
	}
	bool empty()const noexcept {
		return _size == 0;
	}
	//size func done 
	std::size_t size()const noexcept {
		return _size;
	}
	//push_back func done
	void push_back(const _Ty& data) {
		static_assert(std::is_copy_constructible_v<_Ty>, "the type must be copy constructible");
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
		_array[_size] = new _Ty(data); // allocate new object
		_size++;
	}
	//push back func done
	void push_back(_Ty&& data) {
		static_assert(std::is_move_constructible_v<_Ty>, "the type must be move constructible");
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
		_array[_size] = new _Ty(std::move(data)); // allocate new object
		_size++;
	}
	//emplace back func done
	template<class ..._Valty>
	void emplace_back(_Valty&&..._Val) {
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
	void pop_back()noexcept {
		static_assert(std::is_nothrow_destructible_v<_Ty>, "the"
			"type must be destructible without throwing");
		if (_size > 0) {
			_size--;
			delete _array[_size];
			_array[_size] = nullptr;
			return;
		}//make pop back to shrink capacity when not used 
		//some number of capacity
	}
	//
	void show() const{
		for (std::size_t i = 0; i < _size; i++) {
			std::cout << *_array[i] << '\n';
		}
	}
	//operator []func done 
	const _Ty& operator [](const std::size_t index) const &
	{
		assert(index < _size);
		return *_array[index];
	}
	//operator []func done 
	_Ty& operator [](const std::size_t index)& {
		assert(index < _size);
		return *_array[index];
	}
	//operator []func done 
	_Ty&& operator [](const std::size_t index)&&
	{
		assert(index < _size);
		return std::move(*_array[index]);
	}
	//operator []func done 
	const _Ty&& operator [](const std::size_t index)const&& {
		assert(index < _size);
		return std::move(*_array[index]);
	}
	//at func done 
	const _Ty& at(const std::size_t index) const&
	{
		if (index < _size) {
			return *_array[index];
		}
		throw out_of_bounds{"tried to access invalid pos"};
	}
	//at func done 
	_Ty& at(const std::size_t index)& {
		if (index < _size) {
			return *_array[index];
		}
		throw out_of_bounds{ "tried to access invalid pos" };
	}
	//at func done 
	_Ty&& at(const std::size_t index)&&
	{
		if (index < _size) {
			return std::move(*_array[index]);
		}
		throw out_of_bounds{ "tried to access invalid pos" };
	}
	//at func done 
	const _Ty&& at(const std::size_t index)const&& {
		if (index < _size) {
			return std::move(*_array[index]);
		}
		throw out_of_bounds{ "tried to access invalid pos" };
	}
	//destructor done
	~vector()noexcept {
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
	void clear()noexcept {
		static_assert(std::is_nothrow_destructible_v<_Ty>,
			"the type must be destructible without throwing");
		for (std::size_t i = 0; i < _size; i++) {
			delete _array[i];
			_array[i] = nullptr;
		}
		_size = 0;
	}
	//front func done
	const _Ty& front() const&
	{
		assert(!empty());
		return *_array[0];
	}
	//front func done 
	_Ty& front()& {
		assert(!empty());
		return *_array[0];
	}
	//front func done
	_Ty&& front()&&
	{
		assert(!empty());
		return std::move(*_array[0]);
	}
	//front func done
	const _Ty&& front()const&& {
		assert(!empty());
		return std::move(*_array[0]);
	}
	//back func done
	const _Ty& back() const&
	{
		assert(!empty());
		return *_array[_size-1];
	}
	//back func done 
	_Ty& back()& {
		assert(!empty());
		return *_array[_size-1];
	}
	//back func done
	_Ty&& back()&&
	{
		assert(!empty());
		return std::move(*_array[_size-1]);
	}
	//back func done
	const _Ty&& back()const&& {
		assert(!empty());
		return std::move(*_array[_size-1]);
	}
	//swap func done
	void swap(vector<_Ty>&other)noexcept {
		if (this != &other) {
			std::swap(_array, other._array);
			std::swap(_size, other._size);
			std::swap(_capacity, other._capacity);
		}
	}
	//reserve func done
	void reserve(const std::size_t new_capacity) {
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
	//shring_to_fit func done
	void shrink_to_fit() {
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
	void resize(const std::size_t new_size) {
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
	void resize(const std::size_t new_size,const _Ty&value) {
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
	vector<_Ty>& operator =(const vector<_Ty>& other) {
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
	void assign(const std::size_t count, const _Ty& value) {
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
			for (std::size_t j = 0; j< i; j++) {
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
	vector<_Ty>& operator =(vector<_Ty>&& other)&noexcept {
		this->~vector();
		std::swap(_array, other._array);
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);
		return *this;
	}
	// Returns a pointer to the internal array of pointers.
	// The vector retains ownership of the objects.
	// Users may read or modify the objects via the pointers, but must NOT delete them.
	_Ty** data()noexcept {
		return _array;
	}
	void insert(const std::size_t index,const _Ty& value) {
		static_assert(std::is_copy_constructible_v<_Ty>, "the type"
			"must be copy constructible");
		if (index >= _size) {
			push_back(value);
			return;
		}
		if (_size == _capacity) {//realloc
			_Ty** new_array= new _Ty * [2*_capacity+1] {};
			for (std::size_t i = 0; i < _size; i++) {
				new_array[i] = _array[i];
			}
			delete[]_array;
			_array = new_array;
			_capacity =_capacity*2+1;
		}
		_Ty* ptr{ new _Ty(value) };
		for (std::size_t i = _size; i > index; i--) {
			_array[i] = _array[i - 1];
		}
		_array[index] = ptr;
		_size++;
	}
	void insert(const std::size_t index,  _Ty&& value) {
		static_assert(std::is_move_constructible_v<_Ty>, "the type"
			"must be move constructible");
		if (index >= _size) {
			push_back(std::move(value));
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
		_Ty* ptr{ new _Ty(std::move(value)) };
		for (std::size_t i = _size; i > index; i--) {
			_array[i] = _array[i - 1];
		}
		_array[index] = ptr;
		_size++;
	}
	iterator begin()noexcept {
		return { 0,this };
	}
	iterator end()noexcept {
		return { _size,this };
	}
};

_PANAGIOTIS_END



