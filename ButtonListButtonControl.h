#pragma once
#include "listener.h"

// ButtonListButtonControl

class ButtonListButtonControl : 
	public CButton
{
	DECLARE_DYNAMIC(ButtonListButtonControl)

public:
	ButtonListButtonControl();
	virtual ~ButtonListButtonControl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
};


