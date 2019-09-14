#pragma once
#include "tsb.h"
#include "awsdk_colours.h"

/* converts a number into a string */
CString		ToString(int Number);
CString		ToString(size_t Number);
CString		ToString(float Number);
CStringW	ToStringW(int Number);
CString		ToStringT(int Number);

/* converts bools into binary range */
int Make_CMF(bool Bool, bool Italic, bool Supressed = false);

/* convert time into current field type */
String getTime(const char* Format = NULL);
String MakeGuiTime(__time64_t Timestamp, const char* Format = NULL);

/* network conversions */
String UnsignedIpToString(unsigned int IP);

/* bool conversion */
bool ConvBool(BOOL T);
BOOL ConvBool(bool T);

namespace Conversion
{
	// convert from one string to return the same string
	String ToString(String Type);
	String ToString(int Type);	
	String ToString(float Type);
	String ToString(AW::Colour Type);
	String ToStringTSB(tsb_t Type);

	// convert backwards
	void StringToType(String Source, String& Type);
	void StringToType(String Source, float& Type);
	void StringToType(String Source, int& Type);
	void StringToType(String Source, AW::Colour& Type);
	void StringToType(String Source, tsb_t& Type);

	// convert string to type explicit
	float		ToFloat(String Source);
	int			ToInt(String Source);
	AW::Colour	ToColour(String Source);
	tsb_t		ToTSB(String Source);
	bool		ToBool(String Source);
}