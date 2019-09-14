#pragma once

class u_ptrbase
{
protected:
	int		_count;

public: // constructor and deconstructor
	u_ptrbase() {
		_count = 0;
	}

	u_ptrbase(const u_ptrbase& B) {
		_count = 0;
	}

	virtual	~u_ptrbase() { 
		_count = 0;
	}

public: // attaching methods
	u_ptrbase* attachUptr() {
		_count++;
		return dynamic_cast<u_ptrbase*>(this);
	}

	void detatchUptr() {
		_count--;
		if (_count == 0)
			delete this;
	}
};

template<class T>
class u_ptr
{
protected: // pointer to objects
	u_ptrbase* ptr;

public: // constructors
	u_ptr(u_ptrbase* P) {
		ptr = P->attachUptr();
	}

	u_ptr(const u_ptr<T>& Up) {
		ptr = (const_cast<u_ptr<T>*>(&Up))->ptr->attachUptr();
	}

	u_ptr() {
		ptr = 0;
	}

	~u_ptr() {
		if (ptr) ptr->detatchUptr();
	}

public: // assignment
	u_ptr<T>& operator=(const u_ptr<T>& Up) {
		if (ptr) ptr->detatchUptr();
		ptr = (const_cast<u_ptr<T>*>(&Up))->ptr->attachUptr();
		return *this;
	}

public: // operators
	T& operator*() {
		return *dynamic_cast<T*>(ptr);
	}

	T* operator->() {
		return dynamic_cast<T*>(ptr);
	}

public: // checking methods
	bool valid() { return ptr != 0; }
};