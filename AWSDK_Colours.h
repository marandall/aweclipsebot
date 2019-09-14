#pragma once

namespace AW
{
	// represents a colour code
	class Colour 
	{
	public:
		int			value;
		COLORREF	getColour() { return value; }
		Colour& operator=(COLORREF c) { value = c; return *this; } 
		bool operator==(const Colour &other) const {return false; }

	public: // the components
		int			GetRed			(void)		{ return GetRValue(value); }
		int			GetGreen		(void)		{ return GetGValue(value); }
		int			GetBlue			(void)		{ return GetBValue(value); }

	public: // get colour
		static int	LookupString	(const char* X);
	};

	// wraps a colour triplet so it can be used in direct data exchange
	class ColourTripletWrapper
	{
	public:	
		Colour* pClr;
		ColourTripletWrapper& operator()(Colour& Clr)
		{
			pClr = &Clr;
			return *this;
		}
	};
}

inline bool umbra_property_exchangefunc(Umbra::Variant& V, AW::Colour& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = (int)X.getColour();
	else if (Mode == Umbra::EXM_UPDATE_DATA)	X = V.toInt32();
	else return false;
	return true;
}