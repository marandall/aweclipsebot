#pragma once
#include "umbra_classlib.h"
#include "umbra_variantmap.h"
#include "umbra_hostfunction.h"
#include <map>

namespace Umbra
{
	namespace HostObjects
	{
		// 
		// Each definition of a class
		//

		class ClassInfo : public RemoteClass, public u_ptrbase, public Umbra::INewInstanceCreator
		{
		public: // class function members
			typedef std::map<Umbra::string, FunctionPtr> FunctionMap;
			FunctionMap					functions;
			FunctionMap					static_functions;

		public: // the variables for the string
			std::vector<std::string>	vars;
			VariantMap					static_members;
			bool						initialised;

		public: // constructor
			ClassInfo()
			{
				initialised = false;
			}

		public: // generate a new clas
			Umbra::RemoteClass* doUmbraNewInstance(Host* HostPtr);
			bool doUmbraProperty(Variant& V, PropertyRequest& R);
			bool doUmbraFunction(Variant& V, FunctionRequest& R);
			bool doStaticInitialise(Umbra::Host* HostPtr, Umbra::CallStackMonitor* Csm);
		};
		
		// reference of class info
		typedef Umbra::u_ptr<ClassInfo> ClassInfoPtr;

		//
		// Each instance of a class
		//

		class ClassInstance : public RemoteClass, public u_ptrbase
		{
		public: // the list of available functions
			Variant class_info;

		public: // property information on a per-class basis
			VariantMap vars;

		public: // accessor methods
			bool doUmbraFunction(Variant& V, FunctionRequest& R);
			bool doUmbraProperty(Variant& V, PropertyRequest& R);
		};

		// reference of class info
		typedef Umbra::u_ptr<ClassInstance> ClassInstancePtr;

		//
		// ClassCollection
		//

		class MapReflector : public RemoteClass
		{
		public: // list of classes
			VariantMap* map;

		public: // accessor methods
			bool doUmbraProperty(Variant& V, PropertyRequest& R);
		};
	}
}