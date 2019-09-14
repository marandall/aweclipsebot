#pragma once

#include "AWSDK_Support.h"
#include "umbra/umbra_remoteclass.h"
#include "scripting_iterator.h"
#include <vector>

using namespace std;

class Avatar;
class AvatarList;
class Avatars;

#define FOR_EACH_OF_AVATARS			for (size_t _ix = 0; _ix < avatars.size(); _ix++) {	\
										Avatar& User = avatars[_ix];
#define FOR_EACH_INLINE_AVATAR		Avatar* pUser = NULL;								\
									for (size_t _ix = 0; _ix < avCol.size(); _ix++) {		\
										pUser = avCol[_ix];
#define END_EACH_AVATAR				}

class Avatars :
	public Umbra::RemoteClass
{
public:
	Avatars(void);
	~Avatars(void);

	/* create, find and ultimatly destroy a session */
	Avatar*		getSession	(long session) ;
	Avatar*		getCitizen	(long citizen) ;
	Avatar*		getName	(const char* name) ;
	Avatar*		GetByKey	(const char* Key, Avatar* pLocal = NULL) ;
	Avatar*		getByAuto(int AutoId);

	int				getWildcards(const char* Wildcards, AvatarList& list) ;

	/* create and delete methods */
	Avatar*		create(long session, const char* name);
	bool		remove(Avatar* av);

	/* clear all of the avatars */
	int			Clear		(const char* lpReason);
	void		ClearFlags	(void);

	/* methods for updating all of the avatar rights */
	void		ResetRights();

public: // type defenitions
	typedef		std::vector<Avatar*>	VectorType;

protected: // signalling
	Umbra::IteratorSignalAgent _umbra_isa;

public: // pointer iterator
	class iterator
	{
	protected:
		friend class Avatars;
		VectorType::iterator	itr;

	public: // constructor
		iterator(VectorType::iterator Itr) { itr = Itr; }
		iterator() { }
		Avatar& operator*()		{ return *(*itr); }
		Avatar* operator->()	{ return (*itr);  } 
		bool operator==(iterator& P) { return itr == P.itr; }
		bool operator!=(iterator& P) { return itr != P.itr; }
		iterator operator++(int)  { iterator i2 = itr; itr++; return i2; }
	};

public: // collection
	VectorType	avCol;
	typedef		iterator		iterator2;
	iterator	begin()			{ return iterator(avCol.begin()); }
	iterator	end()			{ return iterator(avCol.end());	 }

	Avatar&		operator[]		(size_t position) { return *avCol[position];	}
	size_t		size			(void)   { return avCol.size();		}
	size_t		getSize		(void)	 { return size();			}

	/***************************************************************
	* PURPOSE:	To change the user counts in the GUI
	* FILE:		awAvatars_UserCounts.cpp
	****************************************************************/
	int		IncrementTypes	(int);
	int		DecrementTypes	(int);
	int		getUserCount	(int);
	void	DetectRemote	(void);
	int		ClearTypes();
	void	UpdateTypes();
	bool	SessionExists		(long);
	int		getCount()			{ return (int)(avCol.size()); }
	int		getTouristCount();
	int		getCitizenCount();
	int		getBotCount();

private:
	int		user_counts[3];

public: // status count
	static Avatars& getShared()		{ static Avatars av; return av; }

public: // umbra interfacing systems
	bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
	bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest&);
};

class AvatarList
{
public:
	~AvatarList();

	AW::VectorPNC<Avatar, 10>	vCol;

	bool		AddSingle			(Avatar* pUser);
	bool		FindSingle			(Avatar* pUser) const;
	Avatar*&	operator[]			(size_t index) ;
	size_t		size				(void) const { return vCol.size(); }
	UINT		GetCount			(void) const;
	UINT		AbsorbSessions		(vector<int>& session_list, Avatars& avatars);

	void		SortByName			(void);
	void		SortBySession		(void);
	void		SortByCitizen		(void);
	void		SortByPrivilege		(void);
	void		SortByIP			(void);
	void		SortByDNS			(void);
	void		SortByType			(void);
};