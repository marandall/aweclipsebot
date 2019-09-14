#pragma once
#include "umbra_classlib.h"
#include <vector>
#include "umbra_executioncontroller.h"
#include "umbra_host.h"

namespace Umbra { namespace ClassLib { namespace Usl {

	//
	// System.Collections.Vector
	//

	class VectorImpl : public RemoteClass
	{
	public:
		std::vector<Variant> arr;

	public: // constructor
		VectorImpl(Umbra::Host* HostObject = 0, bool GarbageCollect = false)
		{
			if (GarbageCollect)
				setUmbraGarbageCollected();
		}

		~VectorImpl()
		{
		}

	public: // property access from system
		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			UMBRA_FUNCTION_BEGIN(__construct) {
				// begin the function code
				UMBRA_FUNCTION_CS() {
					arr.reserve(R.args.size());
					for (size_t i = 0; i < R.args.size(); i++)
					{
						arr.push_back(R.args[i]);
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Reflect all functions
			//

			UMBRA_FUNCTION_BEGIN(add) {
				// function header
				UMBRA_FUNCTION_DA(0, Obj, VT_ANY);

				// begin the function code
				UMBRA_FUNCTION_CS() {
					arr.push_back( vaObj );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Extract an item from the index
			//

			UMBRA_FUNCTION_BEGIN(get) {
				// function header
				UMBRA_FUNCTION_DA(0, Index, VT_INTEGER);

				// begin the function code
				UMBRA_FUNCTION_CS() {
					int index = vaIndex.toInt32();

					// return the index
					if (index >= 0 && index < (int)arr.size())
					{
						UMBRA_FUNCTION_RET( &arr[index].getVM() );
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Erases every element in the list
			//

			UMBRA_FUNCTION_BEGIN(clear) {
				// begin the function code
				UMBRA_FUNCTION_CS() {
					arr.clear();
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Removes a given object
			//

			UMBRA_FUNCTION_BEGIN(deleteAt) {
				// function header
				UMBRA_FUNCTION_DA(0, Index, VT_INTEGER);

				// begin the function code
				UMBRA_FUNCTION_CS() {
					int index = vaIndex.toInt32();

					// return the index
					if (index >= 0 && index < (int)arr.size())
						arr.erase(arr.begin() + (size_t)index );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Does this list have this value
			//

			UMBRA_FUNCTION_BEGIN(contains) {
				// function header
				UMBRA_FUNCTION_DA(0, Item, VT_ANY);

				// begin the function code
				UMBRA_FUNCTION_CS() {
					// return the index
					bool found = false;
					for (size_t i = 0; i < arr.size(); i++)
					{
						if (arr[i] == vaItem)
						{
							found = true;
							break;
						}
					}

					// return is found
					UMBRA_FUNCTION_RET( found );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// sorting algorithm
			//

			UMBRA_FUNCTION_BEGIN(sort) {
				// begin the function code
				UMBRA_FUNCTION_CS() {
					for (size_t pass = 1; pass < arr.size(); pass++) 
					{
						for (size_t i = 0; i < arr.size() - pass; i++) 
						{
							if (arr[i] > arr[i+1]) 
							{	
								Variant temp	= arr[i];
								arr[i]		= arr[i+1];
								arr[i+1]	= temp;
							}
						}	
					 }
					UMBRA_FUNCTION_RET( true );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(customSort) {
				// function header
				UMBRA_FUNCTION_DA(0, Function, VT_STRING);
				UMBRA_FUNCTION_OA(1, Param, VT_ANY, Umbra::Variant());
				UMBRA_FUNCTION_OA(2, OrderDesc, VT_BOOLEAN, false);

				// begin the function code
				UMBRA_FUNCTION_CS() {
					// lookup the function
					Umbra::Function* pFunc = R.host->getFunction(vaFunction.toString().c_str());
					if (pFunc == 0)
					{
						UMBRA_FUNCTION_RET( false );
					}
					else
					{
						bool desc = vaOrderDesc.toBool();
						for (size_t pass = 1; pass < arr.size(); pass++) 
						{
							for (size_t i = 0; i < arr.size() - pass; i++) 
							{
								// create an executing pointer for this item
								Umbra::ExecutionController ec;
								ec.host_ptr	= R.host;
								ec.call_stack_monitor = R.call_stack_monitor;

								// execution list
								int ord = R.host->doInlineFunction(*pFunc, &VariantArray(arr[i], arr[i+1], vaParam), ec).toInt32();
								if ((ord > 0 && desc == false) || (ord < 0 && desc == true)) 
								{
									Variant temp	= arr[i];
									arr[i]		= arr[i+1];
									arr[i+1]	= temp;
								}
							}	
						 }
					}
					UMBRA_FUNCTION_RET( true );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();


			//
			// No such function was known
			//

			return R.NOT_HANDLED;
		}

		bool doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			// length
			UMBRA_PROPERTY_R(length, (int)arr.size());

			// unable to determine
			return R.NOT_HANDLED;
		}
	};


	//
	// Defenition for the System.Collections.Vector
	//

	class VectorClass : public RemoteClass, public INewInstanceCreator
	{
	public: // the instance creator interface
		RemoteClass* doUmbraNewInstance(Host* HostPtr)
		{
			return new VectorImpl(HostPtr, true);
		}

		bool doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			// length
			UMBRA_PROPERTY_C(SORT_DESCENDING, true);
			UMBRA_PROPERTY_C(SORT_ASCENDING, false);

			// unable to determine
			return R.NOT_HANDLED;
		}
	};

} } }