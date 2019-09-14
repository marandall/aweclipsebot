#pragma once
#include "aw.h"
#include "awsdk_location.h"
#include <vector>

#define MAX_OBJECT_TYPE	6

namespace AW
{
	class Bot;
	class SplitString;

	//  cit       num      x      y    z        yaw  t r ! ml dl  al len str
	// 334303 1167539915 24800 -90150  23400   -900  0 0 0 12  0  23   0 wall001l.rwxcreate texture woodpost
	// 360975 1167539926 26010    465 -15350      0  0 0 3 0   0   0 218 cdcccc3dcdcccc3dcdcccc3dcdccccbdcdccccbdcdccccbd000000004000000000000000000000004000000000000000000000803f00000000000000000000803f00000000000000000000803f0000803f0000803f0000803f0000803f0000803f000040400000404000004040000000400000004000000040cdcc4c3ecdcc4c3ecdcc4c3ecdcc4c3ecdcc4c3ecdcc4c3e140000005a0000006400bc020000000000002c010000c8000000ffa600000080ff00cdcc4c3d020100000f0000000000000000000000000000635f736f6674313a635f736f66743100
	//  cit       num      x      y    z        yaw  t r ! ml dl  al len str
	
	/*
		Version Information

	*/
	class ObjectData : 
		public Location
	{
	private:
		void	__reconstruct();
	public:
		ObjectData() { __reconstruct(); }

	public:
		int		type;		// the type of object
		int		owner;		// citizen number of builder
		int		id;			// the id number of the object
		int		timestamp;	// the 32 bit timestamp
		int		number;		// the 32 bit object number
		String	model, action, description;
		std::vector<char>	binary_array;	// data for the object, dependant on type
		String	name;

	public: // read access
		int		getOwner		() const { return owner;		}
		int		getId			() const { return id;			}
		int		getTimestamp	() const { return timestamp;	}
		int		getNumber		() const { return number;		}
		int		getType		() const { return type;			}
		String	getModel		() const { return model;		}
		String	getDescription	() const { return description;	}
		String	getAction		() const { return action;		}
		String	getName			() const { return name;			}

	public: // type checks
		bool	isV3			() const { return getType() == AW_OBJECT_TYPE_V3;			}
		bool	isZone			() const { return getType() == AW_OBJECT_TYPE_ZONE;		}
		bool	isParticle		() const { return getType() == AW_OBJECT_TYPE_PARTICLES;	}
		bool	isMover			() const { return getType() == AW_OBJECT_TYPE_MOVER;		}
		bool	isCamera		() const { return getType() == AW_OBJECT_TYPE_CAMERA;		}

	public: // move
		void	move			(const Location& LC) { (dynamic_cast<AW::Location&>(*this)) = LC; }

	public: // get pointers to base data
		aw_object_data_zone*		ToZoneData();
		aw_object_data_particles*	ToParticlesData();
		aw_object_data_camera*		ToCameraData();
		aw_object_data_mover*		ToMoverData();

	public: // establish the name
		String	EstablishName();
		void	UpdateName() { name = EstablishName(); }

	public: // sdk transactions
		void	from_sdk	(Bot& Client);
		void	PrepChange	(Bot& Client);
		void	to_sdk		(Bot& Client);

	protected: // internal transactions
		bool	PropConvert_ReadVersion4_0(SplitString& Data, int Offset);	
		String	PropConvert_WriteVersion4_0();

	public: // stream transactions
		bool	PropConvert_ReadVersion4(String Line);
		String	PropConvert_WriteVersion4();

	public: // umbra enhancements
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest&);
	};


	typedef std::vector<char> ObjectBinary;

	// ObjectOptions
	class ObjectOptions
#ifdef _AWSDK_USE_UMBRA
		: public Umbra::RemoteClass
#endif
	{
	public:
		virtual void UpdateStructure() { }
		virtual void UpdateClass() { }

	public:
		ObjectBinary _store;
	};

	class Object_V3 : 
		public ObjectOptions
	{
	public:
		ObjectBinary _store;
	};

	class Object_Particle : 
		public ObjectOptions
	{
	public: 
		String	assetlist;
		String	name;

	public: // contructor
		Object_Particle(ObjectBinary* Bin = 0) 
		{ 
			if (Bin)
			{
				_store = *Bin; 
				UpdateClass();
			}
			else
			{
				_store.resize(sizeof(aw_object_data_particles), 0);
			}
		}

	public: // low level access of structures
		aw_object_data_particles& getStruct();
		void	UpdateStructure();
		void	UpdateClass();

#ifdef _AWSDK_USE_UMBRA
	public:
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest&);
#endif
	};

	class Object_Zone : 
		public ObjectOptions
	{
	public: // access variables
		String name;
		String footstep;
		String ambient;
		String camera;
		String targetcur;
		String voip_rights;

	public: // contructor
		Object_Zone(ObjectBinary* Bin = 0) 
		{ 
			if (Bin)
			{
				_store = *Bin; 
				UpdateClass();
			}
			else
			{
				_store.resize(sizeof(aw_object_data_zone), 0);
			}
		}

	public: // low level access of structures
		aw_object_data_zone& getStruct();
		void	UpdateStructure();
		void	UpdateClass();

#ifdef _AWSDK_USE_UMBRA
	public:
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest&);
#endif
	};

	class Object_Mover : 
		public ObjectOptions
	{
	public: // access variables
		String	name;         // length of 1st string in str_data
		String	seq;          // length of 2nd string in str_data
		String	script;       // length of 3rd string in str_data
		String	sound;        // length of 4rd string in str_data
		String	waypoints;    // length of 5th string in str_data
		String	bump_name;    // length of 6th string in str_data

	public: // contructor
		Object_Mover(ObjectBinary* Bin = 0) 
		{ 
			if (Bin)
			{
				_store = *Bin; 
				UpdateClass();
			}
			else
			{
				_store.resize(sizeof(aw_object_data_mover), 0);
			}
		}

	public: // low level access of structures
		aw_object_data_mover& getStruct();
		void	UpdateStructure();
		void	UpdateClass();
	};

	class Object_Camera : 
		public ObjectOptions
	{
	public: // access variables
		String	name;         // length of 1st string in str_data

	public: // contructor
		Object_Camera(ObjectBinary* Bin = 0) 
		{ 
			if (Bin)
			{
				_store = *Bin; 
				UpdateClass();
			}
			else
			{
				_store.resize(sizeof(aw_object_data_camera), 0);
			}
		}

	public: // low level access of structures
		aw_object_data_camera& getStruct();
		void	UpdateStructure();
		void	UpdateClass();
	};

}