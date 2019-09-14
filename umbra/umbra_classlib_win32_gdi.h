#pragma once

#include "umbra_remoteclass.h"
#include "umbra_classlib.h"


// UMBRA_NAMESPACE: Win32.Gdi
namespace Umbra { namespace ClassLib { namespace Win32 { namespace Gdi {

	//
	// DEF_CLASS: Bitmap
	//   Info:    Umbra representation of the Gdiplus::Bitmap class
	//   Sealed:  Yes
	//

	class BitmapDef : public Umbra::RemoteClass
	{
	public: // interface access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	// DEF_CLASS: Color
	//   Info:    Umbra representation of the Gdiplus::Color class
	//   Sealed:  Yes
	//

	class ColorDef : public Umbra::RemoteClass
	{
	public: // interface access
		// void doUmbraExtensibleDefSetup();
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	// DEF_CLASS: Pen
	//   Info:    Umbra representation of the Gdiplus::Pen class
	//   Sealed:  Yes
	//

	class PenDef : public Umbra::RemoteClass
	{
	public: // interface access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	// ENU_CLASS: FontStyle
	//   Info:    Umbra representation of the Gdiplus::FontStyle enumeration
	//   Sealed:  Yes
	//

	class FontStyleDef : public Umbra::RemoteClass
	{
	public: // interface access
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	//
	// ENU_CLASS: GenericFontFamily
	//   Info:    Umbra representation of the Gdiplus::GenericFontFamily enumeration
	//   Sealed:  Yes
	//

	class GenericFontFamilyDef : public Umbra::RemoteClass
	{
	public: // interface access
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	//
	// ENU_CLASS: StringAlignment
	//   Info:    Umbra representation of the Gdiplus::StringAlignment enumeration
	//   Sealed:  Yes
	//

	class StringAlignmentDef : public Umbra::RemoteClass
	{
	public: // interface access
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	//
	// ENU_CLASS: StringTrimming
	//   Info:    Umbra representation of the Gdiplus::StringTrimming enumeration
	//   Sealed:  Yes
	//

	class StringTrimmingDef : public Umbra::RemoteClass
	{
	public: // interface access
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	//
	// ENU_CLASS: StringFormat
	//   Info:    Umbra representation of the Gdiplus::StringFormat enumeration
	//   Sealed:  Yes
	//

	class StringFormatDef : public Umbra::RemoteClass
	{
	public: // interface access
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	//
	// IMP_CLASS: Bitmap
	//   Info:    Umbra representation of the Gdiplus::Bitmap
	//   Exposes: 
	//

	class BitmapImpl : public Umbra::RemoteClass
	{
	public: // impl information
		Gdiplus::Bitmap* _pBitmap;

	public: // destruction
		BitmapImpl() { _pBitmap = 0; }
		~BitmapImpl() { if (_pBitmap) delete _pBitmap; }

	public: // interface access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	//
	//
	//
	//

	class SolidBrushDef : public Umbra::RemoteClass, Umbra::INewInstanceCreator
	{
	public: // interface access
		RemoteClass* doUmbraNewInstance(Host*);
	};

	//
	// IMP_CLASS: Brush
	//   Info:    Umbra representation of the Gdiplus::Brush
	//   Exposes: 
	//

	class BrushImpl : public Umbra::RemoteClass
	{
	public: // impl information
		Gdiplus::Brush* brush;

	public: // destruction
		BrushImpl(Umbra::Host* Owner, bool AutoTidy);
		~BrushImpl() { if (brush) delete brush; }
	};

	//
	// IMP_CLASS: RectF
	//   Info:    Umbra representation of the Gdiplus::RectF
	//   Exposes: 
	//

	class RectFImpl : public Umbra::RemoteClass
	{
	public: // impl information
		Gdiplus::RectF rect;

	public: // destruction
		RectFImpl(Umbra::Host* Owner, bool AutoTidy);

	public: // interface access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	// DEF_CLASS: RectF
	//   Info:    Umbra representation of the Gdiplus::RectF class
	//   Sealed:  Yes
	//

	class RectFDef : public Umbra::RemoteClass, Umbra::INewInstanceCreator
	{
	public: // interface access
		RemoteClass* doUmbraNewInstance(Host*);
	};

	//
	// IMP_CLASS: Graphics
	//   Info:    Umbra representation of the Gdiplus::Graphics
	//   Exposes: 
	//

	class GraphicsImpl : public Umbra::RemoteClass
	{
	public: // the bitmap link
		Umbra::Variant		_bitmap;
		Gdiplus::Graphics*	_graphics;

	public: // destruction
		GraphicsImpl() { _graphics = 0; }
		~GraphicsImpl() { if (_graphics) delete _graphics; }

	public: // interface access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	// IMP_CLASS: Pen
	//   Info:    Umbra representation of the Gdiplus::Pen
	//   Exposes: 
	//

	class PenImpl : public Umbra::RemoteClass
	{
	public: // the pen object
		Gdiplus::Pen* pen;

	public: // interface access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	//
	//
	//
	//

	class TextSettingsImpl : public Umbra::RemoteClass
	{
	public: // the pen object
		Gdiplus::Font*	font;
		Gdiplus::Brush*	brush;

	public: // the vertical and horizontal alignment
		int		v_align;
		int		h_align;

	public: // interface access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	// Win32.Gdi
	//

	class NamespaceDef : public Umbra::RemoteClass
	{
	protected: // creation classes that are not constants
		static RectFDef					_rectf_def;
		static SolidBrushDef			_solid_brush_def;
		static ColorDef					_color_def;
		static BitmapDef				_bitmap_def;
		static PenDef					_pen_def;

	protected: // certain enumeration constants are shared between applications
		static FontStyleDef				_font_style;
		static GenericFontFamilyDef		_generic_family;
		static StringAlignmentDef		_string_alignment;
		static StringTrimmingDef		_string_trimming;
		static StringFormatDef			_string_format;

	public: // static instance
		static NamespaceDef& getInst() { static NamespaceDef ns; return ns; }
	};

} } } }