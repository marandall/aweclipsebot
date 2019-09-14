#pragma once

#include <vector>
#include <list>
#include <map>

using namespace std;

namespace AW 
{
	template<class T, int start_size = 2>
	class VectorP
	{
	public:
		VectorP()			{ m_vect.reserve(start_size); };
		~VectorP()			{ RemoveAll(); };

	private:
		std::vector<T*>			m_vect;

		/* operators to get a specific locations */
	public:
		T*		first		(void) const	{ return m_vect[0];					}
		T*		last		(void) const	{ return m_vect[m_vect.size()-1];	}
		
		/* access operators */
	public:
		T*		operator[]	(size_t pos) const { return m_vect[pos]; }
		T*		At			(size_t pos) const { return m_vect[pos]; }

		/* attribute getting */
	public:
		size_t	size		(void) const	{ return m_vect.size();						}
		int		getSize		(void) const	{ return static_cast<int>(m_vect.size());	}
		int		GetCount	(void) const	{ return static_cast<int>(m_vect.size());	}

		/* general manipulation operators for removal */
	public:
		int		RemoveAll	(void)		{ while (m_vect.size()) RemoveTail(); return getSize();					}
		int		DeleteAll	(void)		{ int st = static_cast<int>(m_vect.size()); for(int i = 0; i < st; i++) DeleteTail(); return getSize();	}
		void	DeleteTail	(void)		{ T* ptr = last();  m_vect.pop_back(); delete ptr;						}
		T*		RemoveTail	(void)		{ T* ptr = last();  m_vect.pop_back(); return ptr;						}
		void	pop_back	(void)		{ m_vect.pop_back();													}
		void	DeleteHead	(void)		{ T* ptr = first(); m_vect.erase(m_vect.begin()); delete ptr;			}
		T*		PopHead		(void)		{ T* ptr = first(); m_vect.erase(m_vect.begin()); return ptr;			}
		T*		RemoveAt	(size_t pos){ T* ptr = m_vect[pos]; m_vect.erase(m_vect.begin() + pos); return ptr; }
		void	DeleteAt	(size_t pos){ T* ptr = m_vect[pos]; m_vect.erase(m_vect.begin() + pos); delete ptr; }
		

		/* general manipulation operators for adding */
		size_t	push_back	(T* ptr)	{ m_vect.push_back(ptr); return m_vect.size();				}
		size_t	push_front	(T* ptr)	{ m_vect.insert(m_vect.begin(), ptr); return m_vect.size()	}
	};

	template<class T, int start_size = 2>
	class VectorPNC
	{
	public:
		VectorPNC()			{ m_vect.reserve(start_size); };
		~VectorPNC()		{ RemoveAll(); };

	private:
		std::vector<T*>			m_vect;

		/* operators to get a specific locations */
	public:
		T*		first		(void) const	{ return m_vect[0];					}
		T*		last		(void) const	{ return m_vect[m_vect.size()-1];	}
		
		/* access operators */
	public:
		T*&		operator[]	(size_t pos) { return m_vect[pos]; }
		T*		At			(size_t pos) const { return m_vect[pos]; }

		/* attribute getting */
	public:
		size_t	size		(void) const	{ return m_vect.size();						}
		int		getSize		(void) const	{ return static_cast<int>(m_vect.size());	}
		int		GetCount	(void) const	{ return static_cast<int>(m_vect.size());	}

		/* general manipulation operators for removal */
	public:
		int		RemoveAll	(void)		{ while (m_vect.size()) RemoveTail(); return getSize();					}
		int		DeleteAll	(void)		{ int st = static_cast<int>(m_vect.size()); for(int i = 0; i < st; i++) DeleteTail(); return getSize();	}
		void	DeleteTail	(void)		{ T* ptr = last();  m_vect.pop_back(); delete ptr;						}
		T*		RemoveTail	(void)		{ T* ptr = last();  m_vect.pop_back(); return ptr;						}
		void	pop_back	(void)		{ m_vect.pop_back();													}
		void	DeleteHead	(void)		{ T* ptr = first(); m_vect.erase(m_vect.begin()); delete ptr;			}
		T*		PopHead		(void)		{ T* ptr = first(); m_vect.erase(m_vect.begin()); return ptr;			}
		T*		RemoveAt	(size_t pos){ T* ptr = m_vect[pos]; m_vect.erase(m_vect.begin() + pos); return ptr; }
		void	DeleteAt	(size_t pos){ T* ptr = m_vect[pos]; m_vect.erase(m_vect.begin() + pos); delete ptr; }
		

		/* general manipulation operators for adding */
		size_t	push_back	(T* ptr)	{ m_vect.push_back(ptr); return m_vect.size();				}
		size_t	push_front	(T* ptr)	{ m_vect.insert(m_vect.begin(), ptr); return m_vect.size()	}
	};

	class SplitString
	{
	public:
		/* split the items up */
		SplitString()	{};
		
		/* terminator pops everything from the vector */
		~SplitString()	
		{
			while (m_ptr.size())
			{
				delete m_ptr.back();
				m_ptr.pop_back();			
			}
		};

		/* the routine to parse them */
		size_t split(const char* pInput, const char* pToken, int count)
		{
			char*	p		= const_cast<char*>(pInput);
			int		len		= (int)strlen(pInput);
			int		tlen	= (int)strlen(pToken);
			char*	start	= const_cast<char*>(pInput);
			int		c		= 0;

			/* loops while the counter is less than the length of the string */
			for(;;)
			{
				/* is it the start of our key? */
				char *ptr = forward(p, pToken);

				if (c == count -1 && count != -1)
					ptr = NULL;

				/* check if pointer is null, in which case we go from our current pointer to the end */
				if (ptr == NULL) {
					pushptr(p, pInput + len);					// push from current position to end of line
					c++;										// final run
					break;										// jump to return statement
				} else {
					pushptr(p, ptr );							// push from current pointer to found pointer
					p = ptr + tlen;								// set our pointer ahead by length of token
					c++;										// increment the counter used to limit count
				}
			}
			
			return c;
		}

		SplitString(const char* pInput, const char* pToken, int count) { split(pInput, pToken, count); }

		CStringA operator[](size_t index)
		{
			if (index < m_ptr.size() && index >= 0)
				return m_ptr[index];
			else
				return "";
		}

		int Count()	{
			return (int)m_ptr.size();
		}
		size_t size() { return m_ptr.size(); }

	private:

		// -------------------------------------------------------------------
		// Purpose: To find the next occurance of the string, if not available it returns void
		// is a modifier on strstr to later introduce binary compatability
		// -------------------------------------------------------------------
		char* forward(const char* source, const char* search)
		{
			return (char*)strstr((const char*)source, (const char*)search);
		}

		// ------------------------------------------------------------------------------------------
		// Purpose: Calculates the number of bytes between 2 pointers and then copies
		// everything in between them, finishing off with a null char
		// ------------------------------------------------------------------------------------------
		void pushptr(const char* src, const char* end)
		{
			// TODO: Fix to make variable length and pointerable
			int		len		= (int)(end - src);
			char	*ptr	= new char[len + 1];

			/* for through and copy each item */
			for (int i = 0; i < len; i++)
				*(ptr + i) = *(i + src);

			/* null terminate */
			*(ptr + len) = '\0';

			/* push onto pointer */
			m_ptr.push_back(ptr);
		}

		/* vector used for storing */
		vector<char*>		m_ptr;
	};

	class KnownList
	{
	private:
		vector<String> m_vCol;

	public:
		/* add an item onto the list */
		int	push_back(LPCSTR str) 
		{
			m_vCol.push_back(String(str));
			return (int)m_vCol.size();
		}

		String toString(LPCSTR final_term)
		{
			/* special case for one */
			if (m_vCol.size() == 1) {
				return m_vCol[0];
			}

			/* buffer needed */
			String	buffer;

			if (m_vCol.size() == 0)
				return "";

			/* for all other cases */
			buffer = m_vCol[0];
			for (size_t i = 1; i < m_vCol.size() - 1; i++) 
				buffer.AppendFormat(", %s", m_vCol[i]);
			buffer.AppendFormat(" %s %s", final_term, m_vCol[m_vCol.size() - 1]);

			/* string completed, return the end */
			return buffer;
		}

		void SortZA()
		{	
			for (size_t pass = 1; pass < m_vCol.size(); pass++) {
				for (size_t i = 0; i < m_vCol.size() - pass; i++) {
					if (m_vCol[i].Compare(m_vCol[i+1]) < 0) {
						CString temp	= m_vCol[i];	
						CString& lhs	= m_vCol[i];
						lhs				= m_vCol[i+1];
						CString& rhs	= m_vCol[i+1];
						rhs				= temp;	
					}
				}
			}
		}

		void SortAZ()
		{	
			for (size_t pass = 1; pass < m_vCol.size(); pass++) {
				for (size_t i = 0; i < m_vCol.size() - pass; i++) {
					if (m_vCol[i].Compare(m_vCol[i+1]) > 0) {
						CString temp	= m_vCol[i];	
						CString& lhs	= m_vCol[i];
						lhs				= m_vCol[i+1];
						CString& rhs	= m_vCol[i+1];
						rhs				= temp;	
					}
				}
			}
		}

		void SortZAnc()
		{	
			for (size_t pass = 1; pass < m_vCol.size(); pass++) {
				for (size_t i = 0; i < m_vCol.size() - pass; i++) {
					if (m_vCol[i].CompareNoCase(m_vCol[i+1]) < 0) {
						CString temp	= m_vCol[i];	
						CString& lhs	= m_vCol[i];
						lhs				= m_vCol[i+1];
						CString& rhs	= m_vCol[i+1];
						rhs				= temp;	
					}
				}
			}
		}

		void SortAZnc()
		{	
			for (size_t pass = 1; pass < m_vCol.size(); pass++) {
				for (size_t i = 0; i < m_vCol.size() - pass; i++) {
					if (m_vCol[i].CompareNoCase(m_vCol[i+1]) > 0) {
						CString temp	= m_vCol[i];	
						CString& lhs	= m_vCol[i];
						lhs				= m_vCol[i+1];
						CString& rhs	= m_vCol[i+1];
						rhs				= temp;	
					}
				}
			}
		}

		String operator[](int i)
		{
			return m_vCol[i];
		}

		int	Clear(void)
		{
			m_vCol.clear();
			return 0;
		}

		int GetCount(void)
		{
			return (int)m_vCol.size();
		}

		size_t size(void) { return m_vCol.size(); }
	};

	template<typename T>
	class CSingleListT
	{
	private:
		vector<T>	m_vector;

	public:
		T operator[](size_t size)
		{
			return m_vector[size];
		}

		bool contains(T value)
		{
			for (size_t i = 0; i < m_vector.size(); i++) {
				if (m_vector[i] == value)
					return true;
			}
			return false;
		}

		bool insert(T value)
		{
			if (!contains(value)) {
				m_vector.push_back(value);
				return true;
			}
			else {
				return false;
			}
		}

		size_t size()
		{
			return m_vector.size();
		}
	};

	const bool	WRITE_TO_INI	= false;
	const bool	READ_FROM_INI	= true;

	class EnhancedINI
	{
	public:
		struct Section	
		{
			String name;
			std::map<String, String> keys;
			void InsertKey(const char* ID, const char* Data);

			String& operator[](const char* ID) { return keys[ID]; }
		};

		std::map<String, Section> list;
		
	public:
		typedef std::map<String, Section> SectionMap;
		typedef std::map<String, String>	 KeyMap;


	public: // file i/o methods
		String	getFileData();
		bool	Load(const char* FilePath);
		bool	Save(const char* FilePath);

	protected:
		Section& getSection(const char* Name);
		Section* getSectionRaw(const char* Name);
	
	public:
		Section& operator[](const char* Name) { return getSection(Name); }
		bool	KeyExists(const char* Sect, const char* KeyId) const;
		
	public:
		String	Read(const char* Sect, const char* KeyID, const char* Default = "") const;
		int		Read(const char* Sect, const char* KeyID, int Default) const;

	public:
		void	Write(const char* Sect, const char* KeyID, const char* Text);
		void	Write(const char* Sect, const char* KeyID, int IntNum);
		void	Write(const char* Sect, const char* KeyID, time_t IntNum);
		void	Write(const char* Sect, const char* KeyID, float Num);
		void	Write(const char* Sect, const char* KeyID, bool BoolNum);

		bool	Exchange(bool Read, const char* Sect, const char* KeyId, String&  Value);
		bool	Exchange(bool Read, const char* Sect, const char* KeyId, int&	 Value);
		bool	Exchange(bool Read, const char* Sect, const char* KeyId, time_t& Value);
		bool	Exchange(bool Read, const char* Sect, const char* KeyId, float&  Value);
		bool	Exchange(bool Read, const char* Sect, const char* KeyId, bool&   Value);
		bool	Exchange(bool Read, const char* Sect, const char* KeyId, DWORD&  Value);

	public: // key writing methods
		int		getKeyInt(const char* SectionID, const char* KeyID) const;
		String	getKey(const char* SectionID, const char* KeyID) const;
	};
}