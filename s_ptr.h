#pragma once

template <class T>
class s_ptr
{
protected: // the reference count of items
	int*	ref_count;
	T*		ptr;

protected: // get the reference item
	int& getRefs()
	{
		// is our pointer to shared data null
		if (ref_count == 0)
		{
			ref_count = new int;
			ref_count = 0;
		}

		// return the shared ptr
		return *ref_count;
	}

public: // constructor of the shared object
	s_ptr<T>(T* Ptr)
	{
		// copy ptr
		ptr = Ptr;
		ref_count = 0;

		// assign references
		getRefs()++;
	}

public: // copy constructor
	s_ptr<T>(const s_ptr<T>& Ptr)
	{
		// copy pointer
		ptr = Ptr.ptr;
		ref_count = &Ptr.getRefs();

		// increment copy constructor
		getRefs()++;
	}

public: // assignment
	s_ptr<T>& operator=(const s_ptr<T>& Ptr)
	{
		// assign this data
	}

public: // deconstructors
	~s_ptr<T>()
	{
		// subtract the references
		if ((--getRefs()) == 0)
		{
			delete ptr;
			delete ref_ptr;
		}
	}

public: // operators
	T* operator->()
	{
		return ptr;
	}

public: // refernece operator
	T& operator*()
	{
		return *ptr;
	}
};