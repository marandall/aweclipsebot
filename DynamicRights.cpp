#include "stdafx.h"						// include stdafx
#include "DynamicRights.h"				// using the dynamic rights
#include "xml_docparser.h"				// include XML parser
#include "autonumber.h"
#include "serializer.h"

namespace DynamicRights 
{
	Provider g_Provider;				// actual provider for this class

	//
	//
	//
	//
	//

	User::User(String UserId, String FullId, String Description)
	{
		user_id = UserId;
		full_id	= FullId;
		description = Description;

		static int user_key = 0;
		user_uid = (++user_key);
	}

	Permission::Permission(String KeyId, bool Enabled, String InheritFrom)
	{
		key_id		= KeyId;
		enabled		= Enabled;
		inherit_from= InheritFrom;
	}

	//
	//
	//
	//
	//

	bool Permissions::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// length only
		UMBRA_PROPERTY_R(count,		(int)size());

		// nothing found
		return R.NOT_HANDLED;
	}

	bool Permissions::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// check if a right exists
		UMBRA_FUNCTION_BEGIN(grant) {
			UMBRA_FUNCTION_DA(0, RightID,  Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				Permit(vaRightID.toString().c_str());
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// insert a right exists
		UMBRA_FUNCTION_BEGIN(revoke) {
			UMBRA_FUNCTION_DA(0, RightID,  Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				Deny(vaRightID.toString().c_str());
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

			// insert a right exists
		UMBRA_FUNCTION_BEGIN(index) {
			UMBRA_FUNCTION_DA(0, Index,  Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CS() {
				size_t index = vaIndex.toInt32();
				if (index < size())
				{
					UMBRA_FUNCTION_RET( list[index] );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothign there
		return R.NOT_HANDLED;
	}

	bool Permissions::operator[](const char* Right) const
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			if (list[i].key_id == Right)
			{
				return true;
			}
		}
		return false;
	}

	Permission& Permissions::operator[](size_t I)
	{
		return list[I];
	}

	size_t Permissions::size() const
	{
		return list.size();
	}

	bool Permissions::HasRight(const char* Right) const
	{
		return operator[](Right);
	}

	void Permissions::Permit(const char* Right, String GrantedBy)
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			if (list[i].key_id == Right) 
			{
				list[i].enabled = true;
				list[i].inherit_from = list[i].inherit_from + GrantedBy + ", ";
				return;
			}
		}

		Permission perm(Right, true, GrantedBy + ", ");
		list.push_back(perm);
	}
	
	void Permissions::Deny(const char* Right)
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			if (list[i].key_id.CompareNoCase(Right) == 0) 
			{
				list.erase(list.begin() + i);
				return;
			}
		}
	}
	
	bool Permissions::setPerm(const char* Right, String GrantedBy, bool Grant)
	{
		if (Grant)
		{
			Permit(Right, GrantedBy);
		}
		else 
		{
			Deny(Right);
		}
		return true;
	}

	void Permissions::Merge(Permissions& Permset, String InheritedFrom)
	{
		for (size_t i = 0; i < Permset.size(); i++)
		{
			if (Permset[i].enabled)
			{
				Permit(Permset[i].key_id, InheritedFrom);
			}
		}
	}

	//
	//
	//
	//
	//

	bool Provider::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// user counts
		UMBRA_PROPERTY_R(rights_count,	known_rights);
		UMBRA_PROPERTY_R(user_count,	known_users);

		// nothing handled
		return R.NOT_HANDLED;
	}
	
	bool Provider::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// check if a right exists
		UMBRA_FUNCTION_BEGIN(rightExists) {
			UMBRA_FUNCTION_DA(0, RightID,  Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( known_rights.Exists(vaRightID.toString().c_str()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// insert a right exists
		UMBRA_FUNCTION_BEGIN(addRight) {
			UMBRA_FUNCTION_DA(0, RightID,  Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( known_rights.Insert(vaRightID.toString().c_str()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// remove a right
		UMBRA_FUNCTION_BEGIN(deleteRight) {
			UMBRA_FUNCTION_DA(0, RightID,  Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( known_rights.Remove(vaRightID.toString().c_str()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// user control
		UMBRA_FUNCTION_BEGIN(addUser) {
			UMBRA_FUNCTION_DA(0, Id,   Umbra::VT_STRING);
			UMBRA_FUNCTION_OA(1, Name, Umbra::VT_STRING, "");
			UMBRA_FUNCTION_OA(2, Desc, Umbra::VT_STRING, "");
			UMBRA_FUNCTION_CS() {
				User ku;
				ku.user_id	= vaId.toString().c_str();
				ku.full_id	= vaName.toString().c_str();
				ku.description = vaDesc.toString().c_str();

				// does this user exist already
				if (known_users.Find(ku.user_id) != 0)
				{
					UMBRA_FUNCTION_RET( Umbra::Variant() );
				}
				else
				{
					if (addUser(ku) == true)
					{
						UMBRA_FUNCTION_RET( *known_users.Find(ku.user_id) );
					}
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(addGroup) {
			UMBRA_FUNCTION_DA(0, Id,   Umbra::VT_STRING);
			UMBRA_FUNCTION_OA(1, Name, Umbra::VT_STRING, "");
			UMBRA_FUNCTION_OA(2, Desc, Umbra::VT_STRING, "");
			UMBRA_FUNCTION_CS() {
				// create a group
				bool ag = addGroup(vaId.toString().c_str(),
										vaName.toString().c_str(),
										vaDesc.toString().c_str());
				if (ag)
				{
					String s2 = String("@") + vaId.toString().c_str();
					UMBRA_FUNCTION_RET( *getUser(s2) );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// user control
		UMBRA_FUNCTION_BEGIN(deleteUser) {
			UMBRA_FUNCTION_DA(0, Id, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( known_users.Delete(vaId.toString().c_str()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// user control
		UMBRA_FUNCTION_BEGIN(getUser) {
			UMBRA_FUNCTION_DA(0, Id, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				User* pUser = getUser(vaId.toString().c_str());

				UMBRA_FUNCTION_RET( pUser ? *pUser : Umbra::Variant() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// default control - erase the user list and add more
		UMBRA_FUNCTION_BEGIN(doUserRestore) {
			UMBRA_FUNCTION_CS() {
				known_users.clear();
				CreateDefaultUsers();
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// default control - erase the known rights list and add more
		UMBRA_FUNCTION_BEGIN(doUserRestore) {
			UMBRA_FUNCTION_CS() {
				known_rights.clear();
				CreateDefaultRights();
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// access an index
		UMBRA_FUNCTION_BEGIN(getUserIndex) {
			UMBRA_FUNCTION_DA(0, Index, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CS() {
				// return index check
				size_t index = (size_t)vaIndex.toInt32();
				UMBRA_FUNCTION_RET( (index < known_users.size()) ? known_users[index] : Umbra::Variant() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// access an index
		UMBRA_FUNCTION_BEGIN(getRightIndex) {
			UMBRA_FUNCTION_DA(0, Index, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CS() {
				// return index check
				size_t index = (size_t)vaIndex.toInt32();
				UMBRA_FUNCTION_RET( (index < known_rights.size()) ? (const char*)known_rights[index] : Umbra::Variant() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothing handled
		return R.NOT_HANDLED;
	}

	String& KnownRights::operator[](size_t Index)
	{
		return list[Index];
	}
	
	size_t KnownRights::size() const
	{
		return list.size();
	}
	
	void KnownRights::clear()
	{
		list.clear();
	}

	bool KnownRights::Exists(const char* Id) 
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			if (list[i] == Id)
			{
				return true;
			}
		}
		return false;
	}
	
	bool KnownRights::Insert(const char* Id)
	{
		if (Exists(Id))
		{
			return false;										// return false if key already exists
		}
		
		list.push_back(Id);									//	  push it to the end of the rights list
		return true;
	}
	
	bool KnownRights::Remove(const char* Id)
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			if (list[i] == Id) 
			{
				list.erase(list.begin() + i);
				return true;
			}
		}
		return false;
	}

	//
	//
	//
	//
	//

	bool KnownUsers::Exists(const char* UserId) const
	{
		for (size_t i = 0; i < size(); i++)
			if (operator[](i).user_id == UserId)
				return true;
		return false;
	}

	User* KnownUsers::Find(int UserId)
	{
		for (size_t i = 0; i < size(); i++)
			if (operator[](i).user_uid == UserId)
				return &operator[](i);
		return NULL;
	}

	User* KnownUsers::Find(String UserId)
	{
		for (size_t i = 0; i < size(); i++)
			if (operator[](i).user_id == UserId)
				return &operator[](i);
		return NULL;
	}

	bool KnownUsers::Delete(String UserId)
	{
		for (size_t i = 0; i < size(); i++)
			if (operator[](i).user_id == UserId) {
				erase(begin() + i);
				return true;
			}
		return false;
	}

	//
	//
	//
	//
	//

	bool Provider::UserExists(const char* UserId)
	{
		for (size_t i = 0; i < known_users.size(); i++)
			if (known_users[i].getUserId().CompareNoCase(UserId) == 0)
				return true;
		return false;
	}

	bool Provider::RightExists(const char* RightId)
	{
		for (size_t i = 0; i < known_rights.size(); i++)
			if (known_rights[i].CompareNoCase(RightId) == 0)
				return true;
		return false;
	}

	bool Provider::addUser(User& Usr)
	{
		if (getUser(Usr.user_id)) return false;
		Usr.user_uid = AutoNumber::getNext();
		known_users.push_back(Usr);
		return true;
	}

	
	bool Provider::addGroup(String GrKey, String GrName, String GrDescription)
	{
		known_rights.Insert(String("groups/") + GrKey);
		known_rights.Insert(String("groups_ctrl/") + GrKey);

		// user name
		String name = String("@") + GrKey;
		if (known_users.Exists(name) == false)
		{
			User user(name, GrName, GrDescription);
			user.Permit("system/enabled", "[DRS Provider]");
			addUser(user);
		}
		else
		{
			return false;
		}

		// is ok
		return true;
	}

	User* Provider::getUser(const char* Index)
	{
		return known_users.Find(String(Index));
	}

	bool Provider::CreateDefaultRights()
	{
		/* core system rights */
		known_rights.Insert("system");
		known_rights.Insert("system/administrator");
		known_rights.Insert("system/enabled");
		known_rights.Insert("system/privileged");

		/* eject rights */
		known_rights.Insert("eject");
		known_rights.Insert("eject/eject");
		known_rights.Insert("eject/info");
		known_rights.Insert("eject/teleport");
		
		/* core rights */
		known_rights.Insert("core");
		known_rights.Insert("core/simple");
		known_rights.Insert("core/info");
		known_rights.Insert("core/move");
		known_rights.Insert("core/access");
		
		/* communications rights */
		known_rights.Insert("comms");
		known_rights.Insert("comms/say");
		known_rights.Insert("comms/broadcast");
		known_rights.Insert("comms/private");

		/* object and query related */
		known_rights.Insert("world");
		known_rights.Insert("world/io");
		known_rights.Insert("world/rights");
		known_rights.Insert("world/attributes");

		/* chat relay */
		known_rights.Insert("relay");
		known_rights.Insert("relay/use");
		known_rights.Insert("relay/moderator");
		known_rights.Insert("relay/administrator");
		known_rights.Insert("relay/create_channel");

		/* add the beacon rights */
		known_rights.Insert("jumpgate");
		known_rights.Insert("jumpgate/create");
		known_rights.Insert("jumpgate/use");
		known_rights.Insert("jumpgate/manage");

		/* global command service rights */
		known_rights.Insert("gcs");					
		known_rights.Insert("gcs/info");			// coords, myprivs
		known_rights.Insert("gcs/teleport");		// join
		known_rights.Insert("gcs/boost");			// boost, thrust
		known_rights.Insert("gcs/building");		// clickmode etc

		// access control groupsthey 
		known_rights.Insert("groups");
		known_rights.Insert("groups_ctrl");
		return true;
	}

	bool Provider::CreateDefaultUsers()
	{
		if (!known_users.Exists("//owner")) {
			User user("//owner", "Owner Account", "Rights granted to the same citizenship as the bot");
			user.Permit("system/enabled");
			user.Permit("system/administrator");
			addUser(user);
		}
		
		if (!known_users.Exists("//ownerpriv")) {
			User user("//ownerpriv", "Owners Privilege Account", "Rights granted to the same privileges as the bot");
			user.Permit("system/enabled");
			user.Permit("core/info");
			user.Permit("core/simple");
			addUser(user);
		}

		if (!known_users.Exists("//everyone")) {
			User user("//everyone", "All Users", "Rights granted to everyone");
			addUser(user);
		}

		if (!known_users.Exists("//ps_group")) {
			User user("//ps_group", "PS Users", "Rights granted to PS users");
			addUser(user);
		}

		if (!known_users.Exists("//eject_group")) {
			User user("//eject_group", "Eject Users", "Rights granted to eject users");
			addUser(user);
		}

		if (!known_users.Exists("@tourists")) {
			User user("@tourists", "All Tourists", "Rights granted to all tourists");
			addUser(user);
		}

		if (!known_users.Exists("@citizens")) {
			User user("@citizens", "All Citizens", "Rights granted to all citizens");
			addUser(user);
		}

		if (!known_users.Exists("@bots")) {
			User user("@bots", "All Bots", "Rights granted to all bots");
			addUser(user);
		}

		return true;
	}


	bool Provider::loadClassic(const char* Path)				// read the keys from the provider
	{
		known_rights.clear();
		known_users.clear();
		/*
			<?xml version="1.0" ?>
			<drm>
				<permissions>
					<perm>system</perm>
					<perm>system/enabled</perm>
				</permissions>
				<users>
					<user id="#334303" name="strike rapier" description="test account" />
						<list>
							<lx>system/enabled</lx>
						</list>
					</user>
				</users>
			</drm>
		*/
		String logText;
		XML::File xml;
		xml.LoadFile(Path);
		XML::Node* pNode = xml.root_record.getNode("drm");
		if (!pNode) 
		{
			logText.Format("DRM Load could not find root node 'drm'");
			WriteMsg(logText, RGB_RED, OT_DEBUG | OM_TIME);

			/* template load */
			CreateDefaultRights();
			CreateDefaultUsers();
			return false;
		}

		/* find permissions node */
		CreateDefaultRights();
		XML::Node* pPermissions = pNode->getNode("permissions");
		if (pPermissions) 
		{
			CreateDefaultRights();
			for (size_t i = 0; i < pPermissions->fields.size(); i++) 
			{
				String text = pPermissions->fields[i]->data;
				known_rights.Insert(text);
			}
		}


		/* find users node */
		XML::Node* pUsers = pNode->getNode("users");
		if (pUsers) 
		{
			for (size_t i = 0; i < pUsers->records.size(); i++) 
			{
				XML::Node* pUser = pUsers->records[i];
				if (pUser->getType() != "user")
					continue;
				XML::Node& node = *pUser;
				String user_id	= node["@id"];
				String user_name	= node["@name"];
				String user_desc = node["@description"];
				if (UserExists(user_id)) {
					logText.Format("DRM Load found duplicate user: '%s'", user_id);
					WriteMsg(logText, RGB_RED, OT_DEBUG | OM_TIME);
					continue;
				}
				/* create the user and copy to the constructor */
				User user(user_id, user_name, user_desc);
				if (node.NodeExists("list")) 
				{
					XML::Node* pList = node.getNode("list");
					for (size_t i = 0; i < pList->fields.size(); i++)
						user.Permit(pList->fields[i]->data);
				}
				addUser(user);
			}

			// default users
			CreateDefaultUsers();
		}
		else 
		{
			CreateDefaultUsers();
			logText.Format("DRM Load could not find root node 'users'");
			WriteMsg(logText, RGB_RED, OT_DEBUG | OM_TIME);
			return false;
		}

		return true;
	}

	bool Provider::Load(const char* Path)
	{
		// reset parameters
		known_rights.clear();
		known_users.clear();

		// new method uses serialization handler
		Serializer writer;
		String key, user_key;

		// load the file
		writer.Load(Path);

		// check type
		if (writer["__header"]["contents"].Compare("evo2/dynamic_rights") != 0)
		{
		}

		// read the permissions count
		int perm_count = atoi(writer["permissions"]["count"]);
		for (int i = 0; i < perm_count; i++)
		{
			key.Format("perm_%d", (int)i);
			known_rights.Insert(writer["permissions"][key]);
		}

		// read the users count
		int user_count = atoi(writer["users"]["count"]);
		for (int i = 0; i < user_count; i++)
		{
			// write a user key
			user_key.Format("user_%d", (int)i);

			// read in standard parameters
			User usr(writer[user_key]["id"], writer[user_key]["name"], writer[user_key]["desc"]);
			
			// read each permission
			int count = atoi(writer[user_key]["count"]);
			for (int j = 0; j < count; j++)
			{
				// read in permission
				key.Format("right_%d", j);
				usr.Permit(writer[user_key][key]);
			}
			
			addUser(usr);
		}

		// defaults
		CreateDefaultRights();
		CreateDefaultUsers();

		// misc return
		return 0;
	}

	bool Provider::Save(const char *Path)
	{
		// new method uses serialization handler
		Serializer writer;
		String key, user_key;

		// header information
		writer["__header"]["contents"] = "evo2/dynamic_rights";
		writer["__header"]["version"]  = "1";

		// add each node
		writer["permissions"]["count"] = ToString(known_rights.size());
		for (size_t i = 0; i < known_rights.size(); i++)
		{
			key.Format("perm_%d", (int)i);
			writer["permissions"][key] = known_rights[i];
		}

		// header of users
		writer["users"]["count"] = ToString(known_users.size());
		
		// write each user
		for (size_t i = 0; i < known_users.size(); i++) 
		{
			// standard key format
			user_key.Format("user_%d", (int)i);
			User& user = known_users[i];	

			// standard key format
			writer[user_key]["id"]	 = user.user_id;
			writer[user_key]["name"] = user.full_id;
			writer[user_key]["desc"] = user.description;

			// add the rights list
			writer[user_key]["count"] = ToString(user.size());
			int key_count = 0;
			for (size_t j = 0; j < user.size(); j++)
			{
				if (user[j].enabled)
				{
					key.Format("right_%d", key_count++);
					writer[user_key][key] = user[j].key_id;
				}
			}
		}

		/* save to file */
		return writer.Save(Path);
	}
}