#include "stdafx.h"
#include "ColourButton.h"
#include "colours.h"

/* converts a number into a string */
CStringA ToString(int Number) 
{
	CStringA log; log.Format("%d", (int)Number); 
	return log;

	if (Number == 0)
	{
		return "0";
	}
	else
	{
		CStringA log, group; 
		String comma = "";
		String none  = "";

		while (Number != 0)
		{
			group.Format("%.3d", Number % 1000);
			Number/= 1000;

			log = group + comma + log;
			comma = ",";
		}

		log.TrimLeft("0");
		return log;
	}
}

CStringA ToString(size_t Number) 
{
	CStringA log; log.Format("%d", (int)Number); return log;
}

CStringA ToString(float Number) 
{
	CStringA log; log.Format("%f", Number); 
	log.TrimRight("0.");
	return log;
}



CStringW ToStringW(int Number) 
{
	CStringW log; log.Format(L"%d", Number); return log;
}

CString	ToStringT(int Number) 
{
	CString log; log.Format(_T("%d"), Number); return log;
}

String UnsignedIpToString(unsigned int address)
{
	String addr;
	addr.Format("%d.%d.%d.%d", (BYTE)(address &  0xFF), (BYTE)(address >> 8),
			(BYTE)(address >> 16), (BYTE)(address >> 24));
	return addr;
}

bool ConvBool(BOOL T)
{
	return T == TRUE;
}

BOOL ConvBool(bool T)
{
	return T ? TRUE : FALSE;
}

int Make_CMF(bool Bool, bool Italic, bool Supressed)
{
	return (Bool ? CM_BOLD : 0) | (Italic ? CM_ITALIC : 0) | (Supressed ? CM_SUPPRESSED : 0);
}



namespace Conversion
{
	String temp;

	// convert from one string to return the same string
	String ToString(String Type)
	{
		return Type;
	}

	String ToString(int Type)
	{
		temp.Format("%d", Type);
		return temp;
	}
	
	String ToString(float Type)
	{
		temp.Format("%f", Type);
		return temp;
	}

	String ToString(AW::Colour Type)
	{
		temp.Format("%x", Type.value);
		return temp;
	}

	String ToStringTSB(tsb_t Type)
	{
		if (Type == B_EMPTY) return "default";
		else if (Type == B_TRUE) return "true";
		else if (Type == B_FALSE) return "false";
		else return "unknown";
	}


	// convert backwards
	void StringToType(String Source, String& Type)
	{
		Type = Source;
	}

	void StringToType(String Source, float& Type)
	{
		Type = static_cast<float>(atof(Source));
	}

	void StringToType(String Source, int& Type)
	{
		Type = static_cast<int>(atoi(Source));
	}

	void StringToType(String Source, AW::Colour& Type)
	{
		Type.value = ColourTable::FromString(Source);
	}

	void StringToTypeTSB(String Source, tsb_t& Type)
	{
		if (Source == "yes" || Source == "on" || Source == "1") Type = B_TRUE;
		else if (Source == "no" || Source == "off" || Source == "0") Type = B_FALSE;
		else Type = B_EMPTY;
	}


	// convert string to type explicit
	float ToFloat(String Source)
	{
		return static_cast<float>(atof(Source));
	}

	int	ToInt(String Source)
	{
		return static_cast<int>(atoi(Source));
	}

	AW::Colour ToColour(String Source)
	{
		AW::Colour clr;
		clr.value = ColourTable::FromString(Source);
		return clr;
	}

	tsb_t ToTSB(String Source)
	{
		if (Source == "yes" || Source == "on" || Source == "1") return B_TRUE;
		else if (Source == "no" || Source == "off" || Source == "0") return B_FALSE;
		else return B_EMPTY;
	}

	bool ToBool(String Source)
	{
		return (Source == "yes" || Source == "on" || Source == "1");
	}
}





void DEX_Exchange(bool FromRight, CString& LHS, CString& RHS)
{
	if (FromRight)	LHS = RHS;
	else			RHS = LHS;
}

void DEX_Exchange(bool FromRight, int& LHS, int& RHS)
{
	if (FromRight)	LHS = RHS;
	else			RHS = LHS;
}

void DEX_Exchange(bool FromRight, float& LHS, float& RHS)
{
	if (FromRight)	LHS = RHS;
	else			RHS = LHS;
}

void DEX_Exchange(bool FromRight, int& LHS, CComboBox& RHS)
{
	if (FromRight) {
		LHS = (int)RHS.GetItemData(RHS.GetCurSel());
	} else {
		for (int i = 0; i < RHS.GetCount(); i++) {
			if (RHS.GetItemData(i) == LHS) {
				RHS.SetCurSel(i);
				return;
			}
		}
	}
}

void DEX_Exchange(bool FromRight, COLORREF& LHS, ColourButton& RHS)
{
	if (FromRight) {
		LHS = RHS.getColour();
	} else {
		RHS = LHS;
	}
}

void DEX_Exchange(bool FromRight, bool& LHS, BOOL& RHS)
{
	if (FromRight) {
		LHS = ConvBool(RHS);
	} else {
		RHS = ConvBool(LHS);
	}
}