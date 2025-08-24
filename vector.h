#include<iostream>
#include<new>
#include<algorithm>
#include<type_traits>
#include<cstdlib>
#include<initializer_list>
#include<utility>
#include<cassert>
#include<functional>
#include"Header.h"
_PANAGIOTIS_BEGIN
template<typename _Ty>
class vector {
private:
	std::size_t _capacity;
	std::size_t _size;
	_Ty** _array;
public:
	//default constructor done
	vector()noexcept :_capacity{}, _size{}, _array{}{}
	//done
	vector(const std::size_t capacity) :
		_size{}, _capacity{ capacity > 0 ? capacity : 1 }
	{
		_array = new _Ty * [capacity > 0 ? capacity : 1]{};
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
		_array[_size] = new(std::nothrow) _Ty(data); // allocate new object
		if (_array[_size] == nullptr)return;
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
		_array[_size] = new(std::nothrow) _Ty(std::move(data)); // allocate new object
		if (_array[_size] == nullptr)return;
		_size++;
	}
	//
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
		_array[_size] = new(std::nothrow) _Ty(std::forward<_Valty>(_Val)...); // allocate new object
		if (_array[_size] == nullptr)return;
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
		}
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
	//reserve func
	void reserve(const std::size_t capacity) {
		if (capacity <= _capacity)return;

	}
};

_PANAGIOTIS_END



