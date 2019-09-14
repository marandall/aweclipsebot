#pragma once


// TextBox

class TextBox : public CEdit
{
	DECLARE_DYNAMIC(TextBox)

public:
	TextBox();
	virtual ~TextBox();

	static const int	TBF_STRING = 1;
	static const int	TBF_FLOAT  = 2;
	static const int	TBF_INT	   = 3;

protected:
	DECLARE_MESSAGE_MAP()
	int			format_type;

public:
	CStringA	GetText();
	int			getInt() { return atoi(GetText()); }
	CStringA	getString() { return GetText(); }
	TextBox&	operator=(const char* Text);

public: // overloaded functions for getting and setting different types
	void OverloadedSet(CStringA Value);
	void OverloadedSet(float Value);
	void OverloadedSet(int Value);
	void OverloadedGet(CStringA& Value);
	void OverloadedGet(float& Value);
	void OverloadedGet(int& Value);
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
};


