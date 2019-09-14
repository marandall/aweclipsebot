#pragma once
#include "umbra_classlib.h"
#include <map>
#include <list>

namespace Umbra { namespace ClassLib { namespace Usl {

	//
	// System.Collections.Vector
	//

	class ArrayIterator;

	class ArrayImpl : public RemoteClass
	{
	public: // define a structure of keys and maps
		struct ItemEntry
		{
			Variant key;
			Variant value;
		};

	public: // the structure information
		typedef std::list<ItemEntry>	ItemEntryList;
		typedef ItemEntryList::iterator	iterator;

	public: // the map of elements
		typedef std::map<Umbra::Variant, ItemEntry*> ItemEntryPtrMap;
		typedef ItemEntryPtrMap::iterator entryptr_iterator;

	protected: // defenitions for the information
		ItemEntryList	_list;
		ItemEntryPtrMap	_ptr_map;

	public: // statistical information
		size_t size()
		{
			return _list.size();
		}

	public: 
		// item data searching
		ItemEntry* search(Umbra::Variant Key)
		{
			for (iterator itr = _list.begin(); itr != _list.end(); itr++)
				if (itr->key == Key)
					return &*itr;
			return 0;
		}

		ItemEntry* searchIndex(int IndexKey)
		{
			size_t i = 0;
			for (iterator itr = _list.begin(); itr != _list.end(); itr++)
				if (i++ == IndexKey)
					return &*itr;
			return 0;
		}

		// appending a key
		bool insert(Umbra::Variant Key, Umbra::Variant Value)
		{
			// check if a key exists
			ItemEntry* ie_ptr = search(Key);
			if (ie_ptr)
			{
				ie_ptr->value = Value;
			}
			else
			{
				// create associated key
				ItemEntry ie;
				ie.key		= Key;
				ie.value	= Value;

				// insert into item set
				_list.push_back(ie);
				ItemEntry& ieBack = _list.back();
			}

			// item has inserted ok
			return true;
		}

		// deleteItem
		//  @ Removes an item based upon ite key
		bool deleteItem(Umbra::Variant Key)
		{
			for (iterator itr = _list.begin(); itr != _list.end(); itr++)
			{
				if (itr->key == Key)
				{
					_list.erase(itr);
					return true;
				}
			}

			// could not remove
			return false;
		}

		// deleteIndex 
		//  @ Removes an item based on its position in the index
		bool deleteIndex(int Index)
		{
			size_t i = 0;
			for (iterator itr = _list.begin(); itr != _list.end(); itr++)
			{
				if (i == Index)
				{
					_list.erase(itr);
					return true;
				}
				else
				{
					i++;
				}
			}

			// could not remove
			return false;
		}

	public: // property access from system
		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// [add]
			// @about    adds a new item or updates an old one
			// @returns  true on success or false otherwise
			// @arg1     Must be an integer or string
			//
			UMBRA_FUNCTION_BEGIN(add) {
				UMBRA_FUNCTION_DA(0, K1, VT_ANY);
				UMBRA_FUNCTION_DA(1, V1, VT_ANY);
				UMBRA_FUNCTION_CS() {
					if (vaK1.IsInteger() == true || vaK1.IsString() == true)
					{
						UMBRA_FUNCTION_RET( insert(vaK1, vaV1) );
					}
					else
					{
						UMBRA_FUNCTION_RET( false); 
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();
			
			// [get]
			// @about    finds the current value of an any-type index
			// @returns  returns [ref] value of given key or null
			//
			UMBRA_FUNCTION_BEGIN(get) {
				UMBRA_FUNCTION_DA(0, Index, VT_ANY);
				UMBRA_FUNCTION_CS() {
					ItemEntry* ie_ptr = search(vaIndex);
					if (ie_ptr)
					{
						UMBRA_FUNCTION_RET( &ie_ptr->value.getVM() );
					}
					else
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// [at]
			// @about    finds the current value of an any-type index
			// @returns  returns [ref] value of given key or null
			//
			UMBRA_FUNCTION_BEGIN(at) {
				UMBRA_FUNCTION_DA(0, Index, VT_INTEGER);
				UMBRA_FUNCTION_CS() {
					ItemEntry* ie_ptr = searchIndex(vaIndex.toInt32());
					if (ie_ptr) {
						UMBRA_FUNCTION_RET( &ie_ptr->value.getVM() );
					}	else {
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// [erase]
			// @about    deletes a certain key based on key parameter
			// @returns  true if key deleted
			//
			UMBRA_FUNCTION_BEGIN(erase) {
				UMBRA_FUNCTION_DA(0, Index, VT_ANY);
				UMBRA_FUNCTION_CS() {
					this;
					UMBRA_FUNCTION_RET( deleteItem(vaIndex) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// [eraseIndex]
			// @about    deletes a certain integer index in the list, key param
			// @returns  true if deleted or false otherwise 
			//
			UMBRA_FUNCTION_BEGIN(eraseIndex) {
				UMBRA_FUNCTION_DA(0, Index, VT_INTEGER);
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( deleteIndex(vaIndex.toInt32()) );
				} UMBRA_FUNCTION_CE();

			} UMBRA_FUNCTION_END();
		}
	};

	class ArrayClass : public RemoteClass, public INewInstanceCreator
	{
	public: // the instance creator interface
		RemoteClass* doUmbraNewInstance(Host* HostPtr)
		{
			return new ArrayImpl();
		}
	};

} } }