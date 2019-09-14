#pragma once
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_classlib.h"
#include "property_filters.h"
#include "property_editors.h"
#include "property_object.h"
#include "awsdk_objects_v3.h"

namespace ScriptingHost_ClassLib
{
	namespace App
	{
		// Write Format
		class D_LogStyle : public Umbra::RemoteClass
		{
		protected: // handler
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				// output target constnats
				UMBRA_PROPERTY_R(MAIN,			OT_MAIN);
				UMBRA_PROPERTY_R(DEBUG,			OT_DEBUG);
				UMBRA_PROPERTY_R(TECH,			OT_TECH);

				// the target
				UMBRA_PROPERTY_R(ITALIC,		MF_ITALIC);
				UMBRA_PROPERTY_R(BOLD,			MF_BOLD);
				UMBRA_PROPERTY_R(INC_TIME,		OM_TIME);
				UMBRA_PROPERTY_R(FORE_BOLD,		MF_FBOLD);
				UMBRA_PROPERTY_R(BOLD_ITALIC,	MF_BOLD | MF_ITALIC);

				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED;  
			}
		};

		//
		// Known List
		//

		class D_KnownList : public Umbra::RemoteClass, public Umbra::INewInstanceCreator
		{
			RemoteClass* doUmbraNewInstance(Umbra::Host* HostPtr)
			{
				return new KnownList();
			}
		};

		//
		// Object Data
		//

		//
		// hud tray icon
		//

		class D_HeadsUpTrayIcon : public Umbra::RemoteClass, public Umbra::INewInstanceCreator
		{
		public:
			RemoteClass* doUmbraNewInstance(Umbra::Host* HostPtr);

		public: // supportive functions
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);			
			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		};

		//
		// hud tray icon
		//

		class D_Interface : public Umbra::RemoteClass
		{
		public: // supportive functions
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);			
			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		};

		//
		// specialist cryptology provider
		//

		class D_Crypto : public Umbra::RemoteClass
		{
		public: // supportive functions
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);	
			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		};
		
		//
		// property namespace
		//

#define AS_CLASS_CREATOR(Name, Class)																						\
		class Name : public RemoteClass, public Umbra::INewInstanceCreator					\
		{																																						\
		public:																																			\
			RemoteClass* doUmbraNewInstance(Umbra::Host* HostPtr)											\
			{																																					\
				return new Class;																												\
			}																																					\
		};																																					\
		Name _##Name;

		class PropertyNamespace : public Umbra::RemoteClass
		{
			AS_CLASS_CREATOR(InequalityFilter, Property::Filters::Inequality);
			AS_CLASS_CREATOR(MoverBoundsFilter, Property::Filters::MoverBounds);
			AS_CLASS_CREATOR(ParticleBoundsFilter, Property::Filters::ParticleBounds);
			AS_CLASS_CREATOR(RectangleFilter, Property::Filters::SelectionRect);
			AS_CLASS_CREATOR(StringFinderFilter, Property::Filters::StringFinder);
			AS_CLASS_CREATOR(TypeFinderFilter, Property::Filters::TypeFinder);
			AS_CLASS_CREATOR(UserTypeFilter, Property::Filters::UserType);
			AS_CLASS_CREATOR(ZoneBoundsFilter, Property::Filters::ZoneBounds);
			AS_CLASS_CREATOR(ZoneFilter, Property::Filters::Zones);

			// object types
			AS_CLASS_CREATOR(Object, ObjectData);
			AS_CLASS_CREATOR(CameraInfo, ::AW::Object_Camera);
			AS_CLASS_CREATOR(ZoneInfo, ::AW::Object_Zone);
			AS_CLASS_CREATOR(ParticleInfo, ::AW::Object_Particle);
			AS_CLASS_CREATOR(MoverInfo, ::AW::Object_Mover);

			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(InequalityFilter,			_InequalityFilter);
				UMBRA_PROPERTY_C(MoverBoundsFilter,			_MoverBoundsFilter);
				UMBRA_PROPERTY_C(ParticleBoundsFilter,	_ParticleBoundsFilter);
				UMBRA_PROPERTY_C(RectangleFilter,				_RectangleFilter);
				UMBRA_PROPERTY_C(StringFinderFilter,		_StringFinderFilter);
				UMBRA_PROPERTY_C(TypeFinderFilter,			_TypeFinderFilter);
				UMBRA_PROPERTY_C(UserTypeFilter,				_UserTypeFilter);
				UMBRA_PROPERTY_C(ZoneBoundsFilter,			_ZoneBoundsFilter);
				UMBRA_PROPERTY_C(ZoneFilter,						_ZoneFilter);

				// special types
				UMBRA_PROPERTY_C(Object, _Object);
				UMBRA_PROPERTY_C(CameraInfo, _CameraInfo);
				UMBRA_PROPERTY_C(ZoneInfo, _ZoneInfo);
				UMBRA_PROPERTY_C(ParticleInfo, _ParticleInfo);
				UMBRA_PROPERTY_C(MoverInfo, _MoverInfo);

				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED;  
			}
		};

		//
		// namespaces
		//
		
		class Namespace : public Umbra::RemoteClass
		{
		public: // namespaces
			D_LogStyle				_log_style;
			D_KnownList				_known_list;
			D_Crypto					_crypto;
			PropertyNamespace	_property;
			D_Interface				_interface;

			std::map<String, Umbra::Variant> store;

		protected: // handler
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

		public: // static interface
			static Namespace& getInstance() { static Namespace s; return s; }
		};
	}
}