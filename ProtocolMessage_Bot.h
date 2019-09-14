#pragma once
#include <list>
#include <vector>

class MessageTypes
{
public: // application states
	static const int SERVER_ONLINE									= 1;
	static const int APPLICATION_STARTED						= 10;
	static const int APPLICATION_SHUTTING_DOWN			= 11;

public: // messaging information
	static const int UPDATES = 10;
	static const int UPDATE_PROFILE_ACTIVATED				= (UPDATES + 1);
	static const int UPDATES_INFO										= (UPDATES + 2);

public: // commands
	static const int COMMAND = 1000;
	static const int COMMAND_SHUT_DOWN							= (COMMAND + 1);
	static const int COMMAND_RELOAD									= (COMMAND + 2);

public: // command types 
	struct AppLoad
	{
		enum
		{
			WINDOW_HANDLE	= 1,
			EXE_PATH,
			VERSION_NUMBER,
			VERSION_ID,
			PROCESS_ID
		};
	};

	struct UpdateData
	{
		enum
		{
			PROFILE = 1,
			BOT_NAME,
			WORLD,
			CONNECTION
		};
	};
};

class ProtocolMessage
{
public: // message specifications
	// [32_length][16_typelen][type][body]
	// 0xFFFFFFFF 0xFFFFFF [..] [..]

	typedef std::vector<char> Packet;

public: // message section defenition
	struct MessagePart
	{
		int type;
		std::string body;
	};

public: // message set
	typedef std::list<MessagePart> PartsList;
	typedef PartsList::iterator iterator;

public: // memory defenition
	PartsList parts;
	int				type;

	int getType() { return type; }

public: // message builder
	void add(int Type, std::string Body)
	{
		MessagePart mp;
		mp.type = Type;
		mp.body = Body;
		parts.push_back(mp);
	}
	
	void add(int Type, int Value)
	{
		char temp[20];
		sprintf_s(temp, sizeof(temp), "%d", Value);
		add(Type, temp);
	}

	void add(int Type, float Value)
	{
		char temp[20];
		sprintf_s(temp, sizeof(temp), "%f", Value);
		add(Type, temp);
	}

	std::string grabString(int Key, std::string Default = "")
	{
		for (iterator pt = parts.begin(); pt != parts.end(); pt++)
			if (pt->type == Key)
				return pt->body;
		return Default;
	}

	int grabInt(int Key, int Default = 0)
	{
		for (iterator pt = parts.begin(); pt != parts.end(); pt++)
			if (pt->type == Key)
				return atoi(pt->body.c_str());
		return Default;
	}

public: // build up a message
	std::vector<char> pack(int Type, const char* Header)
	{
		std::vector<char> built;
		built.resize(4);
		memcpy(&built[0], &Type, sizeof(int));

		// over each part comprise
		for (iterator pt = parts.begin(); pt != parts.end(); pt++)
		{
			int bodyLen = (int)pt->body.size();
			int totLen  = (int)bodyLen;

			// item builder
			std::vector<char> em;
			em.resize(8 + totLen);

			// resize string
			memcpy(&em[0], &pt->type, sizeof(int));
			memcpy(&em[4], &totLen, sizeof(int));
			memcpy(&em[8], pt->body.c_str(), (size_t)bodyLen);

			// assign off end
			built.insert(built.end(), em.begin(), em.end());
		}

		return built;
	}

public: // unpack a message
	bool unpack(std::vector<char> Data, const char* Header)
	{
		// extract the type
		if (Data.size() >= 4)
			memcpy(&type, &Data[0], 4);

		// length operator
		size_t start = 4;
		while (start < Data.size())
		{
			int len = 0;

			// create registered message part
			MessagePart mp;
			memcpy(&mp.type, &Data[start], sizeof(int));
			memcpy(&len, &Data[start + sizeof(int)], sizeof(int));
			mp.body = std::string(&Data[start + sizeof(int) * 2], len);			
			
			// section jump
			parts.push_back(mp);
			start+= len + sizeof(int) * 2;
		}

		return true;
	}
};