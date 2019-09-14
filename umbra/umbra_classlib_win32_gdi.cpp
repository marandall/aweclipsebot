#include "stdafx.h"
#include "umbra_classlib_win32_gdi.h"
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;
using namespace Umbra;

// force linkage with gdiplus
#pragma comment(lib, "gdiplus.lib")

namespace Umbra { namespace ClassLib { namespace Win32 { namespace Gdi {
	
	//
	// static classes for enumeration
	//

	FontStyleDef			NamespaceDef::_font_style;
	GenericFontFamilyDef	NamespaceDef::_generic_family;
	StringAlignmentDef		NamespaceDef::_string_alignment;
	StringTrimmingDef		NamespaceDef::_string_trimming;
	StringFormatDef			NamespaceDef::_string_format;

	//
	// Defenition Classes
	//

	RectFDef				NamespaceDef::_rectf_def;
	SolidBrushDef			NamespaceDef::_solid_brush_def;
	ColorDef				NamespaceDef::_color_def;
	BitmapDef				NamespaceDef::_bitmap_def;
	PenDef					NamespaceDef::_pen_def;

	/*void NamespaceDef::doUmbraExtensibleDefSetup()
	{
		/*
		_children["Bitmap"]				= _bitmap_def;
		_children["Color"]				= _color_def;
		_children["Pen"]				= _pen_def;
		_children["RectF"]				= _rectf_def;
		_children["SolidBrush"]			= _solid_brush_def;

		// enumeration classes
		_children["FontStyle"]			= _font_style;
		_children["GenericFontFamily"]	= _generic_family;
		_children["StringAlignment"]	= _string_alignment;
		_children["StringTrimming"]		= _string_trimming;
		_children["StringFormat"]		= _string_format;
		
	}*/

	//
	//
	//
	//
	//

	bool BitmapDef::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		//
		//

		UMBRA_FUNCTION_BEGIN(createXY) 
		{
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Width,  Umbra::VT_INTEGER);
			UMBRA_FUNCTION_DA(1, Height, Umbra::VT_INTEGER);
	
			// code information break
			UMBRA_FUNCTION_CS()
			{
				// create a new object
				BitmapImpl* pImpl = new BitmapImpl();
				pImpl->setUmbraGarbageCollected();

				// copy integer
				pImpl->_pBitmap = new Bitmap(vaWidth.toInt32(), vaHeight.toInt32());

				// return the object
				UMBRA_FUNCTION_RET(pImpl->getUmbraRemoteClass());
			}				
			UMBRA_FUNCTION_CE();
		} 
		UMBRA_FUNCTION_END();

		//
		// Function was not handled
		//

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//

	bool BitmapImpl::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		//
		//

		UMBRA_FUNCTION_BEGIN(getGraphics) 
		{
			// code information break
			UMBRA_FUNCTION_CS()
			{
				// access the graphics object
				Gdiplus::Graphics* pGraphics = new Graphics(_pBitmap);

				// new graphics object
				GraphicsImpl* pImpl = new GraphicsImpl();
				pImpl->setUmbraGarbageCollected();
				pImpl->_bitmap		= getUmbraRemoteClass();
				pImpl->_graphics	= pGraphics;

				// return the object
				UMBRA_FUNCTION_RET(pImpl->getUmbraRemoteClass());
			}				
			UMBRA_FUNCTION_CE();
		} 
		UMBRA_FUNCTION_END();

		//
		// Function was not handled
		//

		return Umbra::FunctionRequest::NOT_HANDLED;

	}

	//
	//
	//
	//
	//

	BrushImpl::BrushImpl(Umbra::Host* Owner, bool AutoTidy) 
	{ 
		brush = 0;

		// register as managed resource
		if (AutoTidy) setUmbraGarbageCollected();
	}

	RemoteClass* SolidBrushDef::doUmbraNewInstance(Host* HostPtr)
	{
		//
		// Creates a new solid brush based upon a color and returns an integer
		//

		return new BrushImpl(HostPtr, false);
	}

	//
	//
	// Class: RectF (impl)
	//
	//

	RemoteClass* RectFDef::doUmbraNewInstance(Host* HostPtr)
	{	
		// not possible
		return new RectFImpl(HostPtr, true);
	}

	RectFImpl::RectFImpl(Umbra::Host* Owner, bool AutoTidy)
	{ 
		// register as managed resource
		if (AutoTidy) setUmbraGarbageCollected();
	}

	bool RectFImpl::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		UMBRA_FUNCTION_BEGIN(__construct) {
			// argument requirements
			UMBRA_FUNCTION_DA(0, Left,	Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(1, Top,	Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(2, Width, Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(3, Height,Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS()	{
				// create a new copy
				RectFImpl* pImpl = new RectFImpl(0, true);
				pImpl->rect = Gdiplus::RectF(vaLeft.toFloat(), vaTop.toFloat(),vaWidth.toFloat(), vaHeight.toFloat());

				// return the class
				UMBRA_FUNCTION_RET(*pImpl);
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Trims around the edge of the item
		//

		UMBRA_FUNCTION_BEGIN(trimEdges) {
			// argument requirements
			UMBRA_FUNCTION_DA(0, Top,	Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(1, Left,  Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(2, Right, Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(3, Bottom,Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS()	{
				// take copy
				REAL top	= vaTop.toFloat();
				REAL left	= vaLeft.toFloat();
				REAL right	= vaRight.toFloat();
				REAL bottom	= vaBottom.toFloat();
	
				// modify the rectangle
				rect.X+=		left;
				rect.Y-=		top;
				rect.Width-=	left - right;
				rect.Height-=	top - bottom;

				// return itself
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Trims around the edge of the item
		//

		UMBRA_FUNCTION_BEGIN(trimLeft) {
			// argument requirements
			UMBRA_FUNCTION_DA(0, Pixels, Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS()	{
				// take copy
				REAL pixels	= vaPixels.toFloat();
				
				// modify the rectangle
				rect.X+= pixels;
				rect.Width-= pixels;

				// return itself
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Trims the right hand edge
		//

		UMBRA_FUNCTION_BEGIN(trimRight) {
			// argument requirements
			UMBRA_FUNCTION_DA(0, Pixels, Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS()	{
				// take copy
				REAL pixels	= vaPixels.toFloat();
				
				// modify the rectangle
				rect.Width-= pixels;

				// return itself
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Trims the top hand edge
		//

		UMBRA_FUNCTION_BEGIN(trimTop) {
			// argument requirements
			UMBRA_FUNCTION_DA(0, Pixels, Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS()	{
				// take copy
				REAL pixels	= vaPixels.toFloat();
				
				// modify the rectangle
				rect.Y+= pixels;
				rect.Height-= pixels;

				// return itself
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Trims the top hand edge
		//

		UMBRA_FUNCTION_BEGIN(trimBottom) {
			// argument requirements
			UMBRA_FUNCTION_DA(0, Pixels, Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS()	{
				// take copy
				REAL pixels	= vaPixels.toFloat();
				
				// modify the rectangle
				rect.Height-= pixels;

				// return itself
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Trims the top hand edge
		//

		UMBRA_FUNCTION_BEGIN(clone) {
			// code information break
			UMBRA_FUNCTION_CS()	{
				// create a new copy
				RectFImpl* pImpl = new RectFImpl(0, true);
				pImpl->rect = rect;

				// return the new copy
				UMBRA_FUNCTION_RET( *pImpl );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Function was not handled
		//

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//

	bool ColorDef::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		// fromRGB - Creates a color code class from an RGB byte
		//

		UMBRA_FUNCTION_BEGIN(fromRGB) 
		{
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Red,   Umbra::VT_INTEGER);
			UMBRA_FUNCTION_DA(1, Green, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_DA(2, Blue,  Umbra::VT_INTEGER);

			// code information break
			UMBRA_FUNCTION_CS()
			{
				// return the colour object
				UMBRA_FUNCTION_RET( (int)Color(vaRed.toInt32(), vaGreen.toInt32(), vaBlue.toInt32()).ToCOLORREF() );
			}				
			UMBRA_FUNCTION_CE();
		} 
		UMBRA_FUNCTION_END();

		//
		// fromRGBA - Creates a color code class from an RGBA byte
		//

		UMBRA_FUNCTION_BEGIN(fromRGB) 
		{
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Red,   Umbra::VT_INTEGER);
			UMBRA_FUNCTION_DA(1, Green, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_DA(2, Blue,  Umbra::VT_INTEGER);
			UMBRA_FUNCTION_DA(3, Alph,  Umbra::VT_INTEGER);

			// code information break
			UMBRA_FUNCTION_CS()
			{
				// return the colour code as an ARGB
				UMBRA_FUNCTION_RET( (int)Color(vaRed.toInt32(), vaGreen.toInt32(), 
					vaBlue.toInt32(), vaAlph.toInt32()).ToCOLORREF() );
			}				
			UMBRA_FUNCTION_CE();
		} 
		UMBRA_FUNCTION_END();

		//
		// Function was not handled
		//

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//

	bool GraphicsImpl::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		// line from one point to another
		//

		UMBRA_FUNCTION_BEGIN(drawLine) {
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Pen, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, X1,  Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(2, Y1,  Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(3, X2,  Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(4, Y2,  Umbra::VT_NUMBER);

			// Pen object is required
			UMBRA_FUNCTION_CC(Pen, PenImpl);

			// code information break
			UMBRA_FUNCTION_CS()
			{
				Gdiplus::REAL x1 = vaX1.toFloat();
				Gdiplus::REAL y1 = vaY1.toFloat();
				Gdiplus::REAL x2 = vaX2.toFloat();
				Gdiplus::REAL y2 = vaY2.toFloat();

				UMBRA_FUNCTION_RET(_graphics->DrawLine(pPen->pen, 
					vaX1.toFloat(), vaY1.toFloat(), vaX2.toFloat(), vaY2.toFloat()) );
			}				
			UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(drawRectangle) {
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Pen, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, X1,  Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(2, Y1,  Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(3, X2,  Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(4, Y2,  Umbra::VT_NUMBER);

			// Pen object is required
			UMBRA_FUNCTION_CC(Pen, PenImpl);

			// code information break
			UMBRA_FUNCTION_CS()
			{
				UMBRA_FUNCTION_RET(_graphics->DrawRectangle(pPen->pen, 
					vaX1.toFloat(), vaY1.toFloat(), vaX2.toFloat(), vaY2.toFloat()) );
			}				
			UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(fillRectangle) {
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Brush, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, RectF,	Umbra::VT_CLASS);
			
			// setup options are required
			UMBRA_FUNCTION_CC(Brush, BrushImpl);
			UMBRA_FUNCTION_CC(RectF, RectFImpl);

			// code information break
			UMBRA_FUNCTION_CS()
			{
				UMBRA_FUNCTION_RET( _graphics->FillRectangle(pBrush->brush, pRectF->rect) );
			}
			UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(fillPie) {
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Brush, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, RectF,	Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(2, Start,	Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(3, Sweep,	Umbra::VT_NUMBER);
			
			// setup options are required
			UMBRA_FUNCTION_CC(Brush, BrushImpl);
			UMBRA_FUNCTION_CC(RectF, RectFImpl);

			// code information break
			UMBRA_FUNCTION_CS()
			{
				UMBRA_FUNCTION_RET( _graphics->FillPie(pBrush->brush, pRectF->rect, 
					vaStart.toFloat(),vaSweep.toFloat()) );
			}
			UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(fillEllipse) {
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Brush, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, RectF,	Umbra::VT_CLASS);
			
			// setup options are required
			UMBRA_FUNCTION_CC(Brush, BrushImpl);
			UMBRA_FUNCTION_CC(RectF, RectFImpl);

			// code information break
			UMBRA_FUNCTION_CS()
			{
				UMBRA_FUNCTION_RET( _graphics->FillEllipse(pBrush->brush, pRectF->rect) );
			}
			UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Nothing Found
		//

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//

	bool PenImpl::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		// Function was not handled
		//

		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//

	bool TextSettingsImpl::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		//
		//

		UMBRA_FUNCTION_BEGIN(alignLeft) {
			// code information break
			UMBRA_FUNCTION_CS()	{
				h_align = Gdiplus::StringAlignmentNear;
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(alignCenter) {
			// code information break
			UMBRA_FUNCTION_CS() {
				h_align = Gdiplus::StringAlignmentCenter;
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(alignRight) {
			// code information break
			UMBRA_FUNCTION_CS() {
				h_align = Gdiplus::StringAlignmentFar;
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(alignTop) {
			// code information break
			UMBRA_FUNCTION_CS()	{
				v_align = Gdiplus::StringAlignmentNear;
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(alignMiddle) {
			// code information break
			UMBRA_FUNCTION_CS() {
				v_align = Gdiplus::StringAlignmentCenter;
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		//
		//

		UMBRA_FUNCTION_BEGIN(alignBottom) {
			// code information break
			UMBRA_FUNCTION_CS() {
				v_align = Gdiplus::StringAlignmentFar;
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Function was not handled
		//

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//

	bool PenDef::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		//
		//

		UMBRA_FUNCTION_BEGIN(fromColor) {
			// dimension requirements
			UMBRA_FUNCTION_DA(0, Col,   Umbra::VT_INTEGER);
			UMBRA_FUNCTION_DA(1, Width, Umbra::VT_DOUBLE);
			
			// code information break
			UMBRA_FUNCTION_CS()
			{
				// create a new pen from the color
				Color c;
				c.SetFromCOLORREF(vaCol.toInt32());

				float width = vaWidth.toFloat();

				// assign pen
				PenImpl* pImpl = new PenImpl();
				pImpl->setUmbraGarbageCollected();
				pImpl->pen = new Gdiplus::Pen(c, width);

				// return the item
				UMBRA_FUNCTION_RET( pImpl->getUmbraRemoteClass() );
			}				
			UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Function was not handled
		//

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//
	
	bool FontStyleDef::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		//
		// read access constants for the 
	    //
		
		UMBRA_PROPERTY_R(REGULAR,				FontStyleRegular);
		UMBRA_PROPERTY_R(BOLD,					FontStyleBold);
		UMBRA_PROPERTY_R(ITALIC,				FontStyleItalic);
		UMBRA_PROPERTY_R(BOLD_ITALIC,			FontStyleBoldItalic);
		UMBRA_PROPERTY_R(UNDERLINE,				FontStyleUnderline);
		UMBRA_PROPERTY_R(STRIKEOUT,				FontStyleStrikeout);

		//
		// No such property
		//

		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool StringFormatDef::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		//
		// read access constants for the class
	    //
		
		UMBRA_PROPERTY_R(DIRECTION_RIGHT_TO_LEFT,	StringFormatFlagsDirectionRightToLeft);
		UMBRA_PROPERTY_R(DIRECTION_VERTICAL,		StringFormatFlagsDirectionVertical);
		UMBRA_PROPERTY_R(NO_FIT_BLACK_BOX,			StringFormatFlagsNoFitBlackBox);
		UMBRA_PROPERTY_R(DISPLAY_FORMAT_CONTROL,	StringFormatFlagsDisplayFormatControl);
		UMBRA_PROPERTY_R(NO_FONT_FALLBACK,			StringFormatFlagsNoFontFallback);
		UMBRA_PROPERTY_R(MEASURE_TRAILING_SPACES,	StringFormatFlagsMeasureTrailingSpaces);
		UMBRA_PROPERTY_R(NO_WRAP,					StringFormatFlagsNoWrap);
		UMBRA_PROPERTY_R(LINE_LIMIT,				StringFormatFlagsLineLimit);
		UMBRA_PROPERTY_R(NO_CLIP,					StringFormatFlagsNoClip);

		//
		// No such property
		//

		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool StringTrimmingDef::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		//
		// read access constants for the class
	    //
		
		UMBRA_PROPERTY_R(NONE,					StringTrimmingNone);
		UMBRA_PROPERTY_R(CHARACTER,				StringTrimmingCharacter);
		UMBRA_PROPERTY_R(WORD,					StringTrimmingWord);
		UMBRA_PROPERTY_R(ELLIPSIS_CHARACTER,	StringTrimmingEllipsisCharacter);
		UMBRA_PROPERTY_R(ELLIPSIS_WORD,			StringTrimmingEllipsisWord);
		UMBRA_PROPERTY_R(ELLIPSIS_PATH,			StringTrimmingEllipsisPath);
		//
		// No such property
		//

		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool StringAlignmentDef::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		//
		// read access constants for the class
	    //
		
		UMBRA_PROPERTY_R(NEAR,					StringAlignmentNear);
		UMBRA_PROPERTY_R(CENTER,				StringAlignmentCenter);
		UMBRA_PROPERTY_R(FAR,					StringAlignmentFar);

		//
		// No such property
		//

		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool GenericFontFamilyDef::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		//
		// read access constants for the class
	    //
	
		UMBRA_PROPERTY_R(SERIF,					GenericFontFamilySerif);
		UMBRA_PROPERTY_R(SANS_SERIF,			GenericFontFamilySansSerif);
		UMBRA_PROPERTY_R(MONOSPACE,				GenericFontFamilyMonospace);

		//
		// No such property
		//

		return Umbra::PropertyRequest::NOT_HANDLED;
	}

} } } }