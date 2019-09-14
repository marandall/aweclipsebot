#pragma once

template <class T>
class ur_ptr
{
protected:
	T* _ptr;

public: // constructor
	ur_ptr(T*)
	{
		_ptr = T;
	}

	ur_ptr()
	{
		_ptr = null;
	}

	ur_ptr& operator=(T* Tr)
	{
		if (_ptr) 
			_ptr->doUmbraDeleteRef();
		_ptr = Tr;
		_ptr->doUmbraAddRef();
	}

	ur_ptr& operator=(const ur_ptr<T>& T)
	{
		if (_ptr) 
			_ptr->doUmbraDeleteRef();
		_ptr = T->_ptr;
		_ptr->doUmbraAddRef();
	}

public: // operators
	T* operator->()
	{
		return _ptr;
	}

	T& operator*()
	{
		return *_ptr;
	}
};