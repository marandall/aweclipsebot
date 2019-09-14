#pragma once

// forward references
class ColourButton;
class ComboBox;
class OutputFormatCtrl;
class TextBox;

namespace AW
{
	class Colour;
}

// data exchange, integer source
class DataExchanger
{
public:
	bool exchange_mode;
	static const bool FROM_CONTROL	= true;
	static const bool TO_CONTROL	= false;

public:
	DataExchanger(bool Mode) { exchange_mode = Mode; }

public:
	// integer to types
	void exchange(int&, CString&);
	void exchange(int&, bool&);
	void exchange(int&, TextBox&);
	void exchange(int&, ComboBox&);
	void exchange(int&, ColourButton&);
	void exchange(int&, CButton&);

	// floats to types
	void exchange(float&, CString&);
	void exchange(float&, TextBox&);

	// boolean to types
	void exchange(bool&, CString&);
	void exchange(bool&, bool&);
	void exchange(bool&, CButton&);

	// colours to types
	void exchange(AW::Colour&, ColourButton&);

	// string to types
	void exchange(CString&, TextBox&);

	// messageinfo to types
	void exchange(MessageInfo&, OutputFormatCtrl&);
};