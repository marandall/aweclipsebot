#include "stdafx.h"

// the classes
#include "umbra_classlib_system.h"
#include "umbra_classlib_math.h"
#include "umbra_classlib_usl_vector.h"

#include "umbra_typedef.h"
#include "umbra_rtstring.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "umbra_host.h"

// debug code using CRT.
#ifdef _DEBUG

#endif

// start of namespace
namespace Umbra { namespace ClassLib {

// ClassLib.MathDef
const double MathDef::E        = 2.71828182845904523536;
const double MathDef::LOG2E    = 1.44269504088896340736;
const double MathDef::LOG10E   = 0.434294481903251827651;
const double MathDef::LN2      = 0.693147180559945309417;
const double MathDef::LN10     = 2.30258509299404568402;
const double MathDef::PI       = 3.14159265358979323846;
const double MathDef::PI_2     = 1.57079632679489661923;
const double MathDef::PI_4     = 0.785398163397448309616;
const double MathDef::SQRT2    = 1.41421356237309504880;
const double MathDef::SQRT1_2  = 0.707106781186547524401;

//
//
// System namespace function
//
//

void SystemDef::doUmbraExtensibleDefSetup()
{
}

bool SystemDef::doUmbraFunction(Variant& V, FunctionRequest& R)
{
	//
	// Text Printing / Debug
	//

	// print single piece of text
	UMBRA_FUNCTION_BEGIN(print)
		UMBRA_FUNCTION_DA(0, Text, Umbra::VT_ANY);
		UMBRA_FUNCTION_CS() {
			printf( "%s", vaText.toString().c_str() );
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	// print single line of text
	UMBRA_FUNCTION_BEGIN(printLn)
		UMBRA_FUNCTION_DA(0, Text, Umbra::VT_ANY);

		// code block
		UMBRA_FUNCTION_CS() {
			Variant v2 = vaText;
			string s = vaText.toString().c_str();
			printf( "%s\n", s.c_str() );
         		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	// print single line of text
	UMBRA_FUNCTION_BEGIN(trace)
		UMBRA_FUNCTION_DA(0, Text, Umbra::VT_ANY);

		// code block
		UMBRA_FUNCTION_CS() {
			if (R.host)	R.host->irep_trace->onTrace(vaText.toString());
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	// print single line of text
	UMBRA_FUNCTION_BEGIN(describeVar)
		UMBRA_FUNCTION_DA(0, V, Umbra::VT_ANY);

		// code block
		UMBRA_FUNCTION_CS() {
			string debug;
			if (vaV.IsNull() == true)
			{
				debug = "{null}";
			}
			else if (vaV.IsInteger())
			{
				debug = "{int64:";
				debug.append(vaV.toString());
				debug.append("}");	
			}
			else if (vaV.IsDouble())
			{
				debug = "{double:";
				debug.append(vaV.toString());
				debug.append("}");	
			}
			else if (vaV.IsString() == true)
			{
				debug = "{string:";
				debug.append(vaV.toString());
				debug.append("}");	
			}
			else if (vaV.IsRemote() == true)
			{
				debug = "{class:";
				debug.append(vaV.toString());
				debug.append("}");	
			}
			UMBRA_FUNCTION_RET( debug.c_str() );
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	//
	// Time Access
	//

	// second counter
	UMBRA_FUNCTION_BEGIN(getTime)
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( time(0) );
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	// tick counter 
	UMBRA_FUNCTION_BEGIN(getTickTimer)
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( (int)GetTickCount() );
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	//
	// Casting Datatypes
	//

	// cast to integer
	UMBRA_FUNCTION_BEGIN(toInteger)
		UMBRA_FUNCTION_DA(0, Var, Umbra::VT_ANY);
		
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET(vaVar.toInt64());
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	// cast to double
	UMBRA_FUNCTION_BEGIN(toDouble)
		UMBRA_FUNCTION_DA(0, Var, Umbra::VT_ANY);
		UMBRA_FUNCTION_RET(vaVar.toDouble());
	UMBRA_FUNCTION_END();

	// cast to string
	UMBRA_FUNCTION_BEGIN(toString)
		UMBRA_FUNCTION_DA(0, Var, Umbra::VT_ANY);
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET(vaVar.toString().c_str());
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	// cast to string
	UMBRA_FUNCTION_BEGIN(toTypeId)
		UMBRA_FUNCTION_DA(0, Var, Umbra::VT_ANY);
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( vaVar.getType() );
		} UMBRA_FUNCTION_CE()
	UMBRA_FUNCTION_END();

	//
	// create a new class
	//
	
	// create a new class until 'new' is created.
	UMBRA_FUNCTION_BEGIN(allocNew)
	{
		// the argument list
		UMBRA_FUNCTION_DA(0, Var, Umbra::VA_CLASS);
		// code block
		UMBRA_FUNCTION_CS() {
		} UMBRA_FUNCTION_CE()
	}
	UMBRA_FUNCTION_END();

	//
	// there is no such function
	//
	
	return FunctionRequest::NOT_HANDLED;
}


//
//
// RUNTIME STRING
//
//

bool RuntimeString::doUmbraFunction(Variant& V, FunctionRequest& R)
{
	// 
	// Copy the class to a new string
	//
	
	UMBRA_FUNCTION_BEGIN(clone) {
		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( data.c_str() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(copy) {
		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( data.c_str() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();
	
	// substring
	UMBRA_FUNCTION_BEGIN(substr) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Start,  Umbra::VT_INTEGER);
		UMBRA_FUNCTION_OA(1, Length, Umbra::VT_INTEGER,  -1);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( data.substr(vaStart.toInt32(), vaLength.toInt32() == -1 ? std::string::npos : vaLength.toInt32()).c_str() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Compare
	//

	UMBRA_FUNCTION_BEGIN(compare) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( vaText.toString().compare(data) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Compare Ignoring Case
	//

	UMBRA_FUNCTION_BEGIN(compareNoCase) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( _stricmp(vaText.toString().c_str(), data.c_str()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Find one string within another
	//

	UMBRA_FUNCTION_BEGIN(find) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
		UMBRA_FUNCTION_OA(1, Start, Umbra::VT_INTEGER, 0);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( (int)data.find(vaText.toString(), (size_t)vaStart.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// turns into a lowercase
	//

	UMBRA_FUNCTION_BEGIN(toLower) {
		// code information break
		UMBRA_FUNCTION_CS() {
			std::string s(data);
			for (size_t i = 0; i < data.size(); i++)
				s[i] = tolower((int)data[i]);
			UMBRA_FUNCTION_RET( s );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// turns into a uppercase
	//

	UMBRA_FUNCTION_BEGIN(toUpper) {
		// code information break
		UMBRA_FUNCTION_CS() {
			std::string s(data);
			for (size_t i = 0; i < data.size(); i++)
				s[i] = toupper((int)data[i]);
			UMBRA_FUNCTION_RET( s );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Take the left hand portion of the string
	//

	UMBRA_FUNCTION_BEGIN(left) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Length, Umbra::VT_INTEGER);
		UMBRA_FUNCTION_CS() {
			// codeblock
			int len = vaLength.toInt32();
			if (len < 0)
			{
				UMBRA_FUNCTION_RET( "" );
			}
			else if (len > (int)data.length())
			{
				UMBRA_FUNCTION_RET( data.c_str() );
			}
			else
			{
				UMBRA_FUNCTION_RET( string(data.c_str(), len).c_str() );
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Take the right hand portion of the string
	//

	UMBRA_FUNCTION_BEGIN(right) {				
		UMBRA_FUNCTION_DA(0, Length, VT_INTEGER);
		UMBRA_FUNCTION_CS() {
			V = string(data.c_str(), vaLength.toInt32()).c_str();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Trim the string left
	//

	UMBRA_FUNCTION_BEGIN(trimLeft) {
		UMBRA_FUNCTION_CS() {
			// trace while there are no items
			size_t pos = 0;
			while (pos < data.length() && (data[pos] == ' ' ||  data[pos] == '\t' || 
										   data[pos] == '\r' || data[pos] == '\n'))
			{
				pos++;
			}
			
			// copy byte back
			data = (data.c_str() + pos);
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Trim the string right
	//

	UMBRA_FUNCTION_BEGIN(trimRight) {
		UMBRA_FUNCTION_CS() {
			// trace while there are no items
			int pos = (int)data.length();
			while (pos >= 0 && (data[pos] == ' ' ||  data[pos] == '\t' || 
								data[pos] == '\r' || data[pos] == '\n'))
			{
				pos--;
			}
			
			// copy byte back
			data = std::string(data.c_str(), pos);
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Reverse the string
	//

	UMBRA_FUNCTION_BEGIN(reverse) {
		// create a copy string of equal length
		std::string result = data;

		// reverse everything
		for (size_t i = 0; i < data.length(); i++)
			result[data.length() - i] = data[i];

		// copy byte back
		V = result.c_str();
	} UMBRA_FUNCTION_END();

	//
	// Split into an array
	//

	UMBRA_FUNCTION_BEGIN(splitToArray) 
	{
		UMBRA_FUNCTION_DA(0, Token, VT_STRING);
		UMBRA_FUNCTION_DA(1, Limit, VT_INTEGER);

		UMBRA_FUNCTION_CS();
		{
			// the string to search for
			std::string keyset = vaToken.toString();

			// new variant collection
			Umbra::ClassLib::Usl::VectorImpl* pImpl = new ClassLib::Usl::VectorImpl(0, true);

			// begin parsing op
			size_t pos   = 0;
			size_t start = 0;
			while (true)
			{
				pos = data.find(keyset, start);
				if (pos == std::string::npos)
				{
					std::string part(data, start);
					pImpl->arr.push_back( part.c_str() );
					break;
				}
				else
				{
					// the item we use
					std::string part = std::string(data, start, pos);
					pImpl->arr.push_back( part.c_str() );

					start = pos + keyset.length();
				}
			}

			// return the position
			UMBRA_FUNCTION_RET( *pImpl );
		}
		UMBRA_FUNCTION_CE();
	} 
	UMBRA_FUNCTION_END();

	//
	// there is no such function
	//
	
	return false;
}

// end of namespace
} }