#pragma once
#include <vector>

using namespace std;

class SecurityRightsDialog;

namespace DynamicRights
{
	//
	// @ SingleRight
	// String setter mechanism
	//
	//

	class SingleRight
		: public Umbra::RemoteClass
	{
	public:
		String key_name;
		String getName() const { return key_name; }

	public:
		SingleRight(String Text) { key_name = Text; }
		SingleRight() { }
	};

	//
	// @ Permission
	// An effective gained right including who it was from
	//
	//

	class Permission 
		: public Umbra::RemoteClass
	{
	public:
		String	key_id;								// the identification of the user
		bool	enabled;							// if the right is granted by the user
		String	inherit_from;						// list of where the right is inherited from
	public:
		Permission() { }
		Permission(String KeyId, bool Enabled, String InheritFrom);
	};

	//
	//
	//
	//
	//

	class Permissions
	{
		friend class SecurityListDialog;

	protected:
		vector<Permission> list;

	public:
		Permission& operator[](size_t Index);				// operator to get the permission key
		bool		operator[](const char* Right) const;	// operator to get the rights key
		bool		HasRight(const char* Right) const;
		size_t		size	(void) const;					// how many rights are granted
		void		Permit	(const char* Right, String GrantedBy = "")	;// permit a user to have a right
		void		Deny	(const char* Right);						// deny a right to this set
		bool		setPerm	(const char* Right, String GrantedBy, bool Grant = false);	// explicitly set a permission
		void		Merge	(Permissions& Permset, String Source);		// permission set copy

	public: // umbra operations
		bool		doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
		bool		doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	//
	//
	//
	//

	class User
		: public Permissions
		, public Umbra::RemoteClass

	{
	public:
		String	user_id;							// the user id
		String	full_id;							// a full text, such as a citizen name
		String	description;						// description of why the person has an account
		int		user_uid;
	
	public: // constructors and read access
		User(String UserId, String FullID, String Description);
		User() { }
		String	getUserId() const { return user_id; }
		String	getFullId() const { return full_id; }
		String	getDescription() const { return description; }

	public: // umbra operations
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
			// reflection
			UMBRA_PROPERTY_REDIRECT(Permissions);

			// readwrite properties
			UMBRA_PROPERTY_RW(id,			user_id);
			UMBRA_PROPERTY_RW(name,			full_id);
			UMBRA_PROPERTY_RW(description,	description);
			
			// no such property
			return R.NOT_HANDLED;
		}

		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			UMBRA_FUNCTION_REDIRECT(Permissions);
			return R.NOT_HANDLED;
		}
	};

	class KnownRights : public Umbra::RemoteClass
	{
	private:
		vector<String>	list;
	public:
		String&		 operator[](size_t Index);		// index access method
		size_t		 size() const;					// size return
		void		 clear();						// clear all known rights
	public:
		bool		 Exists(const char* Id);		// does a right exist
		bool		 Insert(const char* Id);		// inserts a key into the list
		bool		 Remove(const char* Id);		// removes a key from the list
	};

	//
	//
	//
	//
	//

	class KnownUsers
		: public vector<User>
		, public Umbra::RemoteClass
	{
	public:
		bool	Exists(const char* UserId) const;	// does a particular user exist
		User*	Find(int UserId);					// looks up a specific key
		User*	Find(String UserId);					// lookup by ID
		bool	Delete(String UserId);				// delete a particular user
	};

	//
	//
	//
	//
	//

	class Provider : public Umbra::RemoteClass
	{
		friend class SecurityRightsDialog;

	public:
		KnownRights	known_rights;					// a list of known rights for the bot
		KnownUsers	known_users;					// a list of users known to the provider
		bool	CreateDefaultRights();
		bool	CreateDefaultUsers();

	public:
		bool	UserExists(const char* UserId);		// check if a user exists
		bool	RightExists(const char* RightId);	// check if a right exists
		User*	getUser(const char* Index);			// return the user
		bool	addUser(User& Usr);
		bool	addGroup(String GrKey, String GrName, String GrDescription = "");

	public:
		bool	loadClassic(const char* Path);
		bool	Load(const char* Path);				// read the keys from the provider
		bool	Save(const char* Path);				// write the keys to the provider file

	public: // umbra operations
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	extern Provider	g_Provider;
}