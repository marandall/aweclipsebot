#include "stdafx.h"						// standard header
#include "Beacons.h"					// beacons
#include "Helpers_XML.h"				// for datakeypair
#include "FileLoader.h"					// for filereader / writer

namespace Beacons    
{
	// this is where we store our beacons class
	Kernel	g_Beacons;

	Beacon& Kernel::operator[](size_t Index)
	{
		size_t index = 0;
		
		// return this position
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
			if (index++ == Index)
				return *jpPtr;

		//  return the front
		return list.front();
	}

	Beacon* Kernel::Search(LPCSTR Target) 
	{ 
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
		{
			if (jpPtr->getName().CompareNoCase(Target) == 0)
				return &(*jpPtr);
		}
		return NULL;
	}

	bool Kernel::Insert(int Owner, LPCSTR Name, LPCSTR Location) 
	{
		if (Search(Name) != NULL) return false;			// avoid duplicate names
		Beacon beacon(Owner, Name, Location);
		list.push_back(beacon);
		return true;
	}

	void Kernel::Erase() 
	{
		list.clear();
	}

	Kernel::iterator Kernel::erase(Kernel::iterator Itr)
	{
		_umbra_isa.signalInvalidated();
		return list.erase(Itr);
	}
	
	void Kernel::Delete(LPCSTR KeyId) 
	{
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); )
		{
			if (jpPtr->getName().CompareNoCase(KeyId) == 0)
				jpPtr = erase(jpPtr);
			else
				jpPtr++;
		}
	}

	bool Kernel::load(String Path, bool ClearList) 
	{
		// do we require a reset or are we merging
		if (ClearList == true)
			Erase();
		
		/* open file for reading */
		FileReader reader(Path);
		if (!reader.IsOK())
			return false;

		/* parse beacons */
		for (size_t i = 0; i < reader.size(); i++) 
		{
			/* load via xml line */
			Helpers::XML::DataKeyset ksKeys;
			if (Helpers::XML::ParseSingleLine(reader[i], ksKeys) == true) 
			{
				Beacon beacon(atoi(ksKeys["owner"]), ksKeys["name"], ksKeys["coords"]);
				list.push_back(beacon);
			}
		}
		return true;
	}

	bool Kernel::save(LPCTSTR FilePath) 
	{
		/* open file for writing */
		FileWriter writer(FilePath);
		if (!writer.IsOK())
			return false;
	
		/* write each beacon entry to the coords file */
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
		{	
			Beacon& beacon = *jpPtr;
			String line;
			line.Format("<point owner=\"%d\" name=\"%s\" coords=\"%s\" />\n", 
				beacon.getOwner(), beacon.getName(), beacon.getLocation());
			writer.WriteLine(line);
		}
		
		return true;
	}

	int Kernel::RemoveBy(int Citizen)
	{
		/* delete all jump points by user */
		int counter = 0;
		
		// delete by a user
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); )
		{
			if (jpPtr->getOwner() == Citizen)
			{
				jpPtr = erase(jpPtr);
				counter++;
			}
			else
			{
				jpPtr++;
			}
		}

		return counter;
	}

	int	Kernel::RemoveProximity(const AW::Location& Position, int Range) 
	{
		/* delete all jump points by proximity */
		int counter = 0;
		
		// delete by a user
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); )
		{
			if (jpPtr->distanceTo(Position) < Range)
			{
				jpPtr = erase(jpPtr);
				counter++;
			}
			else
			{
				jpPtr++;
			}
		}

		return counter;
	}

	bool Kernel::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		// get an iterator
		//

		UMBRA_FUNCTION_BEGIN(getIterator) {
			// code information break
			UMBRA_FUNCTION_CS() {
				// create iterator type defenition
				UMBRA_FUNCTION_RET( (new ScriptIterator(*this, _umbra_isa, list.begin(), list.end()))->setUmbraGarbageCollected() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// lookup routines
		//

		UMBRA_FUNCTION_BEGIN(getClosestTo) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Position, Umbra::VT_CLASS);
			UMBRA_FUNCTION_CC(Position, AW::Location);	
			// code block
			UMBRA_FUNCTION_CS() {
				Beacon* pBeacon = getBeaconByProximity(*pPosition, "");
				UMBRA_FUNCTION_RET( pBeacon ? *pBeacon : Umbra::Variant() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(search) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Name, Umbra::VT_STRING);
			// code block
			UMBRA_FUNCTION_CS() {
				Beacon* pBeacon = Search(vaName.toString().c_str());
				UMBRA_FUNCTION_RET( pBeacon ? *pBeacon : Umbra::Variant() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// erasure routines
		//

		UMBRA_FUNCTION_BEGIN(deleteInRadius) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Position, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, Radius,   Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CC(Position, AW::Location);	
			// code block
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( RemoveProximity(*pPosition, vaRadius.toInt32()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(deleteByCitizen) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Citizen, Umbra::VT_INTEGER);
			// code block
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( RemoveBy(vaCitizen.toInt32()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(deleteById) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Name, Umbra::VT_STRING);
			// code block
			UMBRA_FUNCTION_CS() {
				Delete(vaName.toString().c_str());
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(deleteIterator) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Iterator, Umbra::VT_CLASS);
			UMBRA_FUNCTION_CC(Iterator, ScriptIterator);	
			// code block
			UMBRA_FUNCTION_CS() {
				iterator er = erase(pIterator->active_iterator);
				UMBRA_FUNCTION_RET( (new ScriptIterator(*this, _umbra_isa, er, list.end()))->setUmbraGarbageCollected() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// not handled
		//

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	Beacon* Kernel::FirstFirstCloserThan(const AW::Location& lcPos, int Range)
	{
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
			if (jpPtr->distanceTo(lcPos) < Range * 100)
				return &(*jpPtr);
		return NULL;
	}

	Beacon*	Kernel::getBeaconByProximity(const AW::Location& Pos, String Prefix)
	{
		double max_dist = 0xFFFFFFFF;
		Beacon* nearest = 0;

		// iterate the list
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
		{
			double dist = jpPtr->distanceTo(Pos);
			if (dist < max_dist && jpPtr->getName().Left(Prefix.GetLength()) == Prefix)
			{
				max_dist = dist;
				nearest = &(*jpPtr);
			}
		}

		// the nearest
		return nearest;
	}

	int Kernel::SearchOwner(int Owner, std::vector<Beacon*>& Array)
	{
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
			if (jpPtr->getOwner() == Owner)
				Array.push_back(&(*jpPtr));
		return 0;
	}

	int Kernel::GetUserBeaconCount(int Citizen)
	{
		int count = 0;
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
			if (jpPtr->getOwner() == Citizen)
				count++;
		return count;
	}

	void Kernel::deleteByAutoId(int AutoID)
	{
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++)
		{
			if (jpPtr->getAutoNumber() == AutoID)
			{
				erase(jpPtr);
				return;
			}
		}
	}

	Beacon*	Kernel::getByAutoID(int AutoID)
	{
		for (List::iterator jpPtr = list.begin(); jpPtr != list.end(); jpPtr++) {
			if (jpPtr->getAutoNumber() == AutoID) 	{
				return &(*jpPtr);
			} }
		return 0;
	}

	//
	// Processing List Operations
	//

	void Kernel::transferOwnership(ProcessingList& ProcList, int NewOwner)
	{
		for (List::iterator jp = list.begin(); jp != list.end(); jp++)
			if (ProcList.includes(jp->getAutoNumber()) == true)
				jp->owner = NewOwner;
		return;
	}

	void Kernel::deleteByProcessingList(ProcessingList& ProcList)
	{
		for (List::iterator jp = list.begin(); jp != list.end(); )
			if (ProcList.includes(jp->getAutoNumber()) == true)
				jp = erase(jp);
			else
				jp++;
		return;
	}

}